#ifndef META_UTILS_HPP
#define META_UTILS_HPP
#include "reflect_information.hpp"

namespace reflect {

/*template<class Tp, class Char, class Traits>
decltype(auto) print(Tp const& t, std::basic_ostream<Char,Traits>& out) {
    utils::for_each_tuple_args([](auto&& elem ,auto& out) {
        out << elem << " ";
    },t, out);
    return out;
}

template<class Tp, class Char, class Traits>
decltype(auto) print(Tp&& t, std::basic_ostream<Char,Traits>& out) {
    utils::for_each_tuple_args([](auto&& elem ,auto& out) {
        out << elem << " ";
    },std::forward<Tp>(t), out);
    return out;
}

template<class Tp, class Char, class Traits>
decltype(auto) print_hana_string(Tp const& t, std::basic_ostream<Char,Traits>& out) {
    utils::for_each_tuple_args([](auto&& elem ,auto& out) {
        out << boost::hana::to<const char*>(elem) << " ";
    },t, out);
    return out;
}

template<class Tp, class Char, class Traits>
decltype(auto) print_hana_string(Tp&& t, std::basic_ostream<Char,Traits>& out) {
    utils::for_each_tuple_args([](auto&& elem ,auto& out) {
        out << boost::hana::to<const char*>(elem) << " ";
    },std::forward<Tp>(t), out);
    return out;
}*/

namespace detail {
template <class T, class Obj, typename ::std::enable_if_t<info::is_static_v<::std::decay_t<T>>,bool> = 1>
constexpr decltype(auto) get_variable_impl (T const& value, Obj&) {
    return value();
}

template <class T, class Obj, typename ::std::enable_if_t<!info::is_static_v<::std::decay_t<T>>,bool> = 1>
constexpr decltype(auto) get_variable_impl (T const& value, Obj& p) {
    return value(p);
}

template <class T, class Obj, class... Args, typename ::std::enable_if_t<info::is_static_v<::std::decay_t<T>>,bool> = 1>
constexpr decltype(auto) invoke_method_impl (T& value, Obj&, Args&& ...args) {
    return value(::std::forward<Args>(args)...);
}

template <class T, class Obj, class... Args, typename ::std::enable_if_t<!info::is_static_v<::std::decay_t<T>>,bool> = 1>
constexpr decltype(auto) invoke_method_impl (T& value, Obj& p, Args&& ...args) {
    return value(p,::std::forward<Args>(args)...);
}

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

}

#endif // UTILS_HPP
