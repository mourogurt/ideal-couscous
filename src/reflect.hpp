#ifndef META_UTILS_HPP
#define META_UTILS_HPP
#include "reflect_information.hpp"

namespace reflect {

namespace detail {

template <class T, class Obj>
constexpr decltype(auto) get_variable_impl (T&& value, Obj&& p) {
    if constexpr (!info::is_static_v<::std::decay_t<T>>) return value(::std::forward<Obj>(p));
    else return value();
}

template <class T, class Obj, class... Args>
constexpr decltype(auto) invoke_method_impl (T&& value, Obj&& p, Args&& ...args) {
    if constexpr (!info::is_static_v<::std::decay_t<T>>)
        return value(::std::forward<Obj>(p),::std::forward<Args>(args)...);
    else return value(::std::forward<Args>(args)...);
}

template <class T, class Obj, class... Args>
constexpr decltype(auto) try_invoke_method_check_void_impl (T&& value, Obj&& p, Args&&... args) {
    if constexpr (::std::is_void_v<typename ::std::decay_t<T>::return_type>) {
        invoke_method_impl(::std::forward<T>(value),::std::forward<Obj>(p),::std::forward<Args>(args)...);
        return ::boost::hana::make_tuple();
    }
    else
        return ::boost::hana::make_tuple(invoke_method_impl(::std::forward<T>(value),::std::forward<Obj>(p),::std::forward<Args>(args)...));
}

template <class T, class Obj, class... Args>
constexpr decltype(auto) try_invoke_method_sfinae_impl (T&& value, Obj&& p, Args&&... args) {

    if constexpr (info::is_suitable_types<typename ::std::decay_t<T>,Args...>())
        return try_invoke_method_check_void_impl(::std::forward<T>(value),::std::forward<Obj>(p),::std::forward<Args>(args)...);
    else
        return ::boost::hana::make_tuple();
}

template <class T, class... Args>
constexpr decltype(auto) check_invoke_method_sfinae_impl (T&&, Args&&...) {
    if constexpr (info::is_suitable_types<typename std::decay_t<T>,Args...>())
        return ::boost::hana::make_tuple(true);
    else
        return ::boost::hana::make_tuple(false);
}

template<class T, class Index, class TmpTuple, class... Args>
constexpr decltype (auto) try_invoke_method_index_impl (T&& obj, Index&& index, TmpTuple const& tmp, Args&&... args) {
    return try_invoke_method_sfinae_impl(::boost::hana::at(tmp,index),::std::forward<T>(obj),::std::forward<Args>(args)...);
}

template<class T, class Index, class TmpTuple, class... Args>
constexpr decltype (auto) check_invoke_method_index_impl (Index&& index, TmpTuple const& tmp, Args&&... args) {
    return check_invoke_method_sfinae_impl(::boost::hana::at(tmp,index),::std::forward<Args>(args)...);
}

template<class T, class Tuple, ::std::size_t ...Indices, class... Args>
constexpr decltype (auto) try_invoke_method_impl (T&& obj, Tuple&& tuple, ::std::index_sequence<Indices...>&&, Args&&... args) {
    constexpr auto tmp = reflect::info::MetaClass<::std::decay_t<T>>::methods_metadata;
    return utils::multiple_concat(try_invoke_method_index_impl(::std::forward<T>(obj),::boost::hana::at_c<Indices>(tuple),tmp,::std::forward<Args>(args)...)...);
}

template<class T, class Tuple, ::std::size_t ...Indices, class... Args>
constexpr decltype (auto) check_invoke_method_impl (Tuple&& tuple, ::std::index_sequence<Indices...>&&, Args&&... args) {
    constexpr auto tmp = reflect::info::MetaClass<::std::decay_t<T>>::methods_metadata;
    return utils::multiple_concat(check_invoke_method_index_impl<T>(::boost::hana::at_c<Indices>(tuple),tmp,::std::forward<Args>(args)...)...);
}


template <class... Args> struct get_types {constexpr static auto value {::boost::hana::tuple_t<Args...>};};
template <class... Args> struct get_types<::boost::hana::tuple<Args...>> {constexpr static auto value {::boost::hana::tuple_t<Args...>};};

template <class T, class I, class J>
constexpr decltype (auto) get_method_arg_type_impl (T&&, I&& i, J&& j) {
    return ::boost::hana::at(get_types<typename ::std::decay_t<decltype(::boost::hana::at(info::MetaClass<typename ::std::decay_t<T> >::methods_metadata,i))>::args_type>::value,j);
}

template <class T, std::size_t I, std::size_t J>
constexpr decltype (auto) get_method_arg_type_impl () {
    return ::boost::hana::at_c<J>(get_types<typename ::std::decay_t<decltype(::boost::hana::at_c<I>(info::MetaClass<typename ::std::decay_t<T> >::methods_metadata))>::args_type>::value);
}

template <class T, class Index>
constexpr decltype (auto) get_method_return_type_impl (T&&, Index&& i) {
    return ::boost::hana::type_c<typename ::std::decay_t<decltype(::boost::hana::at(info::MetaClass<typename ::std::decay_t<T> >::methods_metadata,i))>::return_type>;
}

template <class T, std::size_t Index>
constexpr decltype (auto) get_method_return_type_impl () {
    return ::boost::hana::type_c<typename ::std::decay_t<decltype(::boost::hana::at_c<Index>(info::MetaClass<typename ::std::decay_t<T> >::methods_metadata))>::return_type>;
}

template <class T, class I, class J> struct get_method_arg_type_struct {
    using type = typename decltype(std::decay_t<decltype(get_method_arg_type_impl(::std::declval<T>(),::std::declval<I>(),::std::declval<J>()))>())::type;
};

template <class T, std::size_t I, std::size_t J>
struct get_method_arg_type_struct_2 {
    using type = typename decltype(::std::decay_t<decltype(get_method_arg_type_impl<T,I,J>())>())::type;
};

template <class T, class I>
struct get_method_return_type_struct {
    using type = typename decltype(::std::decay_t<decltype(get_method_return_type_impl(::std::declval<T>(),::std::declval<I>()))>())::type;
};

template <class T, ::std::size_t I>
struct get_method_return_type_struct_2 {
    using type = typename decltype(::std::decay_t<decltype(get_method_return_type_impl<T,I>())>())::type;
};

template <class T, class Index>
constexpr decltype (auto) get_variable_type_impl (T&&, Index&& i) {
    return ::boost::hana::type_c<typename ::std::decay_t<decltype(::boost::hana::at(info::MetaClass<typename ::std::decay_t<T> >::vars_metadata,i))>::return_type>;
}

template <class T, std::size_t Index>
constexpr decltype (auto) get_variable_type_impl () {
    return ::boost::hana::type_c<typename ::std::decay_t<decltype(::boost::hana::at_c<Index>(info::MetaClass<typename ::std::decay_t<T> >::vars_metadata))>::return_type>;
}

template <class T, class I>
struct get_variable_arg_type_struct {
    using type = typename decltype(std::decay_t<decltype(get_variable_type_impl(::std::declval<T>(),::std::declval<I>()))>())::type;
};

template <class T, std::size_t I>
struct get_variable_arg_type_struct_2 {
    using type = typename decltype(::std::decay_t<decltype(get_variable_type_impl<T,I>())>())::type;
};

}

template <class Obj, class T, typename ::std::enable_if_t<info::is_reflected_v<::std::decay_t<Obj>>,bool> = 1>
constexpr decltype(auto) find_variable_name (T&& name) {
    constexpr auto tmp = info::MetaClass<typename ::std::decay_t<Obj>>::vars_names;
    return utils::find_values(name,tmp);
}

template <class Obj, class T, typename ::std::enable_if_t<info::is_reflected_v<::std::decay_t<Obj>>,bool> = 1>
constexpr decltype(auto) find_method_name (T&& name) {
    constexpr auto tmp = info::MetaClass<typename ::std::decay_t<Obj> >::methods_names;
    return utils::find_values(name,tmp);
}

template <::std::size_t Index, class T, typename ::std::enable_if_t<info::is_reflected_v<::std::decay_t<T>>,bool> = 1>
constexpr decltype(auto) get_variable (const T& obj) {
    constexpr auto tmp = info::MetaClass<typename ::std::decay_t<T> >::vars_metadata;
    return detail::get_variable_impl(::boost::hana::at_c<Index>(tmp),obj);
}

template <class Index, class T, typename ::std::enable_if_t<info::is_reflected_v<::std::decay_t<T>>,bool> = 1>
constexpr decltype(auto) get_variable (T&& obj, Index&& i) {
    constexpr auto tmp = info::MetaClass<typename ::std::decay_t<T> >::vars_metadata;
    return detail::get_variable_impl(::boost::hana::at(tmp,i),::std::forward<T>(obj));
}

template <class T, class Index, class... Args, typename ::std::enable_if_t<info::is_reflected_v<::std::decay_t<T>>,bool> = 1>
constexpr decltype(auto) invoke_method (T&& obj, Index&& i, Args&&... args) {
    constexpr auto tmp = info::MetaClass<typename ::std::decay_t<T> >::methods_metadata;
    return detail::invoke_method_impl(::boost::hana::at(tmp,i),::std::forward<T>(obj),::std::forward<Args>(args)...);
}

template <::std::size_t Index ,class T, class... Args, typename ::std::enable_if_t<info::is_reflected_v<::std::decay_t<T>>,bool> = 1>
constexpr decltype(auto) invoke_method (const T& obj, Args&&... args) {
    constexpr auto tmp = info::MetaClass<typename ::std::decay_t<T> >::methods_metadata;
    return detail::invoke_method_impl(::boost::hana::at_c<Index>(tmp),obj,::std::forward<Args>(args)...);
}

template <::std::size_t Index, class T, typename ::std::enable_if_t<info::is_reflected_v<::std::decay_t<T>>,bool> = 1>
constexpr decltype(auto) get_static_variable () {
    constexpr auto tmp = info::MetaClass<typename ::std::decay_t<T> >::vars_metadata;
    return ::boost::hana::at_c<Index>(tmp)();
}

template <class T, class Index, typename ::std::enable_if_t<info::is_reflected_v<::std::decay_t<T>>,bool> = 1>
constexpr decltype(auto) get_static_variable (Index&& i) {
    constexpr auto tmp = info::MetaClass<typename ::std::decay_t<T> >::vars_metadata;
    return ::boost::hana::at(tmp,i)();
}

template <class T, class Index, class... Args, typename ::std::enable_if_t<info::is_reflected_v<::std::decay_t<T>>,bool> = 1>
constexpr decltype(auto) invoke_static_method (Index&& i, Args&&... args) {
    constexpr auto tmp = info::MetaClass<typename ::std::decay_t<T> >::methods_metadata;
    return ::boost::hana::at(tmp,i)(::std::forward<Args>(args)...);
}

template <std::size_t Index, class T, class... Args, typename std::enable_if_t<info::is_reflected_v<::std::decay_t<T>>,bool> = 1>
constexpr decltype(auto) invoke_static_method (Args&&... args) {
    constexpr auto tmp = info::MetaClass<typename ::std::decay_t<T> >::methods_metadata;
    return ::boost::hana::at_c<Index>(tmp)(::std::forward<Args>(args)...);
}

template<class T, class IndTuple, class... Args>
constexpr decltype (auto) try_invoke_method (T&& obj, IndTuple&& indices, Args&&... args) {

    constexpr ::std::size_t N = decltype(::boost::hana::size(::std::forward<IndTuple>(indices)))::value;
    return detail::try_invoke_method_impl(::std::forward<T>(obj),::std::forward<IndTuple>(indices),::std::make_index_sequence<N>(),::std::forward<Args>(args)...);
}

template<class T, class IndTuple, class... Args>
constexpr decltype (auto) check_invoke_method (IndTuple&& indices, Args&&... args) {

    constexpr ::std::size_t N = decltype(::boost::hana::size(::std::forward<IndTuple>(indices)))::value;
    return detail::check_invoke_method_impl<T>(::std::forward<IndTuple>(indices),::std::make_index_sequence<N>(),::std::forward<Args>(args)...);
}

template <class T, class Index>
constexpr decltype (auto) get_method_args_types (T&&, Index&& i) {
    return detail::get_types<typename ::std::decay_t<decltype(::boost::hana::at(info::MetaClass<typename ::std::decay_t<T> >::methods_metadata,i))>::args_type>::value;
}

template <class T, std::size_t Index>
constexpr decltype (auto) get_method_args_types () {
    return detail::get_types<typename ::std::decay_t<decltype(::boost::hana::at_c<Index>(info::MetaClass<typename ::std::decay_t<T> >::methods_metadata))>::args_type>::value;
}

template <class T, std::size_t Index>
constexpr decltype (auto) get_methods_args_count () {
    return ::std::move(decltype(::boost::hana::size(typename ::std::decay_t<decltype(::boost::hana::at_c<Index>(info::MetaClass<typename ::std::decay_t<T> >::methods_metadata))>::args_type{}))::value);
}

template <class T, class Index>
constexpr decltype (auto) get_methods_args_count (Index&& i) {
    return ::std::move(decltype(::boost::hana::size(typename ::std::decay_t<decltype(::boost::hana::at(info::MetaClass<typename ::std::decay_t<T> >::methods_metadata,i))>::args_type{}))::value);
}

template <class T, class Index>
constexpr decltype (auto) get_method_return_type (T&& obj, Index&& i) {
    return detail::get_method_return_type_impl(std::forward<T>(obj),std::forward<Index>(i));
}

template <class T, std::size_t Index>
constexpr decltype (auto) get_method_return_type () {
    return detail::get_method_return_type_impl<T,Index>();
}

template<class T, std::size_t Index>
constexpr decltype (auto) get_variable_type() {
    return detail::get_variable_type_impl<T,Index>();
}

template <class T, class Index>
constexpr decltype (auto) get_variable_type (T&& obj, Index&& i) {
    return detail::get_variable_type_impl(std::forward<T>(obj),std::forward<Index>(i));
}

template< class T, class I, class J >
constexpr auto method_arg_type (I&&, J&&)
 -> typename decltype(detail::get_method_arg_type_struct<T,I,J>{})::type;

template< class T, ::std::size_t I, ::std::size_t J >
using method_arg_type_t = typename detail::get_method_arg_type_struct_2<T,I,J>::type;

template< class T, class I>
constexpr auto method_return_type (I&&)
 -> typename decltype(detail::get_method_return_type_struct<T,I>{})::type;

template< class T, ::std::size_t I>
using method_return_type_t = typename detail::get_method_return_type_struct_2<T,I>::type;

template< class T, class I>
constexpr auto variable_type (I&&)
 -> typename decltype(detail::get_variable_arg_type_struct<T,I>{})::type;

template< class T, ::std::size_t I>
using variable_type_t = typename detail::get_variable_arg_type_struct_2<T,I>::type;

template <class T>
constexpr decltype(auto) get_class_name () {
    return info::MetaClass<typename ::std::decay_t<T> >::name;
}

template <class T, class I>
constexpr decltype(auto) get_variable_name (I&& i) {
    constexpr auto tmp = info::MetaClass<typename ::std::decay_t<T> >::vars_names;
    return ::boost::hana::at(tmp,i);
}

template <class T, std::size_t I>
constexpr decltype(auto) get_variable_name () {
    constexpr auto tmp = info::MetaClass<typename ::std::decay_t<T> >::vars_names;
    return ::boost::hana::at_c<I>(tmp);
}

template <class T, class I>
constexpr decltype(auto) get_method_name (I&& i) {
    constexpr auto tmp = info::MetaClass<typename ::std::decay_t<T> >::methods_names;
    return ::boost::hana::at(tmp,i);
}

template <class T, std::size_t I>
constexpr decltype(auto) get_method_name () {
    constexpr auto tmp = info::MetaClass<typename ::std::decay_t<T> >::methods_names;
    return ::boost::hana::at_c<I>(tmp);
}

template <class T>
constexpr decltype(auto) get_variables_count () {
    return ::std::move(decltype(::boost::hana::size(info::MetaClass<typename ::std::decay_t<T> >::vars_metadata))::value);
}

template <class T>
constexpr decltype(auto) get_methods_count () {
    return ::std::move(decltype(::boost::hana::size(info::MetaClass<typename ::std::decay_t<T> >::methods_metadata))::value);
}

}

#endif // UTILS_HPP
