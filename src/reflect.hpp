#ifndef META_UTILS_HPP
#define META_UTILS_HPP
#include "reflect_information.hpp"

/**
 * @brief todo list for reflection
 * @todo serialization examples (console, json)
 * @todo performance benchmarks
 * @todo compile time benchmarks
 * @todo c++14 port
 * @todo document detail namespace
 * @todo Variable/method flags (public, private, protected)
 * @todo Runtime reflection over static
 * @todo Reflect constructors
 * @todo Signals
 * @todo Slots
 * @todo Move macroses out of class definition
 * @todo Parameters
 * @todo Enums
 * @todo Inheritance
 * @todo Polymorphism
 */
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

template<class T, class Index, class TmpTuple, class... Args>
constexpr decltype (auto) try_invoke_method_index_impl (T&& p, Index&& index, TmpTuple const& tmp, Args&&... args) {
    using Obj = decltype(::boost::hana::at(tmp,index));
    if constexpr (info::is_suitable_types<typename ::std::decay_t<Obj>,Args...>()) {
        if constexpr (::std::is_void_v<typename ::std::decay_t<Obj>::return_type>) {
            invoke_method_impl(::boost::hana::at(tmp,index),::std::forward<T>(p),::std::forward<Args>(args)...);
            return ::boost::hana::make_tuple();
        } else {
            return ::boost::hana::make_tuple(invoke_method_impl(::boost::hana::at(tmp,index),::std::forward<T>(p),::std::forward<Args>(args)...));
        }
    } else {
        return ::boost::hana::make_tuple();
    }
}

template<class T, class Index, class TmpTuple, class... Args>
constexpr decltype (auto) check_invoke_method_index_impl (Index&& index, TmpTuple const& tmp, Args&&...) {
    using Obj = decltype (::boost::hana::at(tmp,index));
    if constexpr (info::is_suitable_types<typename std::decay_t<Obj>,Args...>())
        return ::boost::hana::make_tuple(true);
    else
        return ::boost::hana::make_tuple(false);
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
constexpr decltype (auto) get_method_arg_type_impl (I&& i, J&& j) {
    return ::boost::hana::at(get_types<typename ::std::decay_t<decltype(::boost::hana::at(info::MetaClass<typename ::std::decay_t<T> >::methods_metadata,i))>::arg_types>::value,j);
}

template <class T, std::size_t I, std::size_t J>
constexpr decltype (auto) get_method_arg_type_impl () {
    return ::boost::hana::at_c<J>(get_types<typename ::std::decay_t<decltype(::boost::hana::at_c<I>(info::MetaClass<typename ::std::decay_t<T> >::methods_metadata))>::arg_types>::value);
}

template <class T, class Index>
constexpr decltype (auto) get_method_return_type_impl (Index&& i) {
    return ::boost::hana::type_c<typename ::std::decay_t<decltype(::boost::hana::at(info::MetaClass<typename ::std::decay_t<T> >::methods_metadata,i))>::return_type>;
}

template <class T, std::size_t Index>
constexpr decltype (auto) get_method_return_type_impl () {
    return ::boost::hana::type_c<typename ::std::decay_t<decltype(::boost::hana::at_c<Index>(info::MetaClass<typename ::std::decay_t<T> >::methods_metadata))>::return_type>;
}

template <class T, std::size_t I, std::size_t J>
struct get_method_arg_type_struct {
    using type = typename std::decay_t<decltype(get_method_arg_type_impl<T,I,J>())>::type;
};

template <class T, ::std::size_t I>
struct get_method_return_type_struct {
    using type = typename std::decay_t<decltype(get_method_return_type_impl<T,I>())>::type;
};

}

template <class Obj, class T, typename ::std::enable_if_t<info::is_reflected_v<::std::decay_t<Obj>>,bool> = 1>
/**
 * @brief Find variable by name in class
 *
 * @param name compile-time string
 * @return boost::hana::size_t if found or boost::hana::nothing if not
 */
constexpr decltype(auto) find_variable_name (T&& name) {
    constexpr auto tmp = info::MetaClass<typename ::std::decay_t<Obj>>::vars_names;
    if constexpr (decltype(::boost::hana::size(utils::find_values(name,tmp)))::value != 0)
        return ::boost::hana::at_c<0>(utils::find_values(name,tmp));
    else return ::boost::hana::nothing;
}

template <class Obj, class T, typename ::std::enable_if_t<info::is_reflected_v<::std::decay_t<Obj>>,bool> = 1>
/**
 * @brief Find method by name in class
 *
 * @param name compile-time string
 * @return boost::hana::tuple tuple of indices
 */
constexpr decltype(auto) find_method_name (T&& name) {
    constexpr auto tmp = info::MetaClass<typename ::std::decay_t<Obj> >::methods_names;
    return utils::find_values(name,tmp);
}

template <::std::size_t Index, class T, typename ::std::enable_if_t<info::is_reflected_v<::std::decay_t<T>>,bool> = 1>
/**
 * @brief Get class variable by index
 *
 * @param obj class object
 * @return info::mem_fn_t::return_type or info::static_mem_t::return_type
 */
constexpr decltype(auto) get_variable (const T& obj) {
    constexpr auto tmp = info::MetaClass<typename ::std::decay_t<T> >::vars_metadata;
    return detail::get_variable_impl(::boost::hana::at_c<Index>(tmp),obj);
}

template <class Index, class T, typename ::std::enable_if_t<info::is_reflected_v<::std::decay_t<T>>,bool> = 1>
/**
 * @brief Get class variable by index
 *
 * @param obj class object
 * @param i index of variable
 * @return info::mem_fn_t::return_type or info::static_mem_t::return_type
 */
constexpr decltype(auto) get_variable (T&& obj, Index&& i) {
    constexpr auto tmp = info::MetaClass<typename ::std::decay_t<T> >::vars_metadata;
    return detail::get_variable_impl(::boost::hana::at(tmp,i),::std::forward<T>(obj));
}

template <class T, class Index, class... Args, typename ::std::enable_if_t<info::is_reflected_v<::std::decay_t<T>>,bool> = 1>
/**
 * @brief Invoke class method by index
 *
 * @param obj class object
 * @param i index of variable
 * @param args method arguments
 * @return info::mem_fn_t::return_type or info::static_fn_t::return_type
 */
constexpr decltype(auto) invoke_method (T&& obj, Index&& i, Args&&... args) {
    constexpr auto tmp = info::MetaClass<typename ::std::decay_t<T> >::methods_metadata;
    return detail::invoke_method_impl(::boost::hana::at(tmp,i),::std::forward<T>(obj),::std::forward<Args>(args)...);
}

template <::std::size_t Index ,class T, class... Args, typename ::std::enable_if_t<info::is_reflected_v<::std::decay_t<T>>,bool> = 1>
/**
 * @brief Invoke class method by index
 *
 * @param obj class object
 * @param args method arguments
 * @return info::mem_fn_t::return_type or info::static_fn_t::return_type
 */
constexpr decltype(auto) invoke_method (const T& obj, Args&&... args) {
    constexpr auto tmp = info::MetaClass<typename ::std::decay_t<T> >::methods_metadata;
    return detail::invoke_method_impl(::boost::hana::at_c<Index>(tmp),obj,::std::forward<Args>(args)...);
}

template <::std::size_t Index, class T, typename ::std::enable_if_t<info::is_reflected_v<::std::decay_t<T>>,bool> = 1>
/**
 * @brief Get static variable
 *
 * @return info::static_mem_t::return_type
 * @todo counter for static variables
 *
 */
constexpr decltype(auto) get_static_variable () {
    constexpr auto tmp = info::MetaClass<typename ::std::decay_t<T> >::vars_metadata;
    return ::boost::hana::at_c<Index>(tmp)();
}

template <class T, class Index, typename ::std::enable_if_t<info::is_reflected_v<::std::decay_t<T>>,bool> = 1>
/**
 * @brief Get static variable
 *
 * @param i index of variable
 * @return info::static_mem_t::return_type
 * @todo counter for static variables
 */
constexpr decltype(auto) get_static_variable (Index&& i) {
    constexpr auto tmp = info::MetaClass<typename ::std::decay_t<T> >::vars_metadata;
    return ::boost::hana::at(tmp,i)();
}

template <class T, class Index, class... Args, typename ::std::enable_if_t<info::is_reflected_v<::std::decay_t<T>>,bool> = 1>
/**
 * @brief Invoke static method
 *
 * @param i index of method
 * @param args method arguments
 * @return info::static_fn_t::return_type
 * @todo counter for static methods
 */
constexpr decltype(auto) invoke_static_method (Index&& i, Args&&... args) {
    constexpr auto tmp = info::MetaClass<typename ::std::decay_t<T> >::methods_metadata;
    return ::boost::hana::at(tmp,i)(::std::forward<Args>(args)...);
}

template <std::size_t Index, class T, class... Args, typename std::enable_if_t<info::is_reflected_v<::std::decay_t<T>>,bool> = 1>
/**
 * @brief Invoke static method
 *
 * @param args method arguments
 * @return info::static_fn_t::return_type
 * @todo counter for static methods
 */
constexpr decltype(auto) invoke_static_method (Args&&... args) {
    constexpr auto tmp = info::MetaClass<typename ::std::decay_t<T> >::methods_metadata;
    return ::boost::hana::at_c<Index>(tmp)(::std::forward<Args>(args)...);
}

template<class T, class IndTuple, class... Args>
/**
 * @brief Tries to invoke a methods on the given indices. If the input args can not be converted to the method arguments, the method will be skipped
 *
 * @param obj class object
 * @param indices tuple of indices(boost::hana::size_t)
 * @param args method arguments
 * @return boost::hana::tuple tuple of return values or empty tuple
 */
constexpr decltype (auto) try_invoke_method (T&& obj, IndTuple&& indices, Args&&... args) {

    constexpr ::std::size_t N = decltype(::boost::hana::size(::std::forward<IndTuple>(indices)))::value;
    return detail::try_invoke_method_impl(::std::forward<T>(obj),::std::forward<IndTuple>(indices),::std::make_index_sequence<N>(),::std::forward<Args>(args)...);
}

template<class T, class IndTuple, class... Args>
/**
 * @brief Checks whether the methods can be invoked with given arguments
 *
 * @param indices tuple of indices(boost::hana::size_t)
 * @param args method arguments
 * @return boost::hana::tuple of bools
 */
constexpr decltype (auto) check_invoke_method (IndTuple&& indices, Args&&... args) {

    constexpr ::std::size_t N = decltype(::boost::hana::size(::std::forward<IndTuple>(indices)))::value;
    return detail::check_invoke_method_impl<T>(::std::forward<IndTuple>(indices),::std::make_index_sequence<N>(),::std::forward<Args>(args)...);
}

template <class T, class Index>
/**
 * @brief Returns an argument of given method
 *
 * @param i method index
 * @return boost::hana::tuple of boost::hana::type
 */
constexpr decltype (auto) get_method_arg_types (Index&& i) {
    return detail::get_types<typename ::std::decay_t<decltype(::boost::hana::at(info::MetaClass<typename ::std::decay_t<T> >::methods_metadata,i))>::arg_types>::value;
}

template <class T, std::size_t Index>
/**
 * @brief Returns an argument of given method
 *
 * @return boost::hana::tuple of boost::hana::type
 */
constexpr decltype (auto) get_method_arg_types () {
    return detail::get_types<typename ::std::decay_t<decltype(::boost::hana::at_c<Index>(info::MetaClass<typename ::std::decay_t<T> >::methods_metadata))>::arg_types>::value;
}

template <class T, std::size_t Index>
/**
 * @brief Returns the number of arguments in the method
 *
 * @return std::size_t&&
 *
 */
constexpr decltype (auto) get_methods_args_count () {
    return decltype(::boost::hana::size(typename ::std::decay_t<decltype(::boost::hana::at_c<Index>(info::MetaClass<typename ::std::decay_t<T> >::methods_metadata))>::arg_types{}))::value;
}

template <class T, class Index>
/**
 * @brief Returns the number of arguments in method
 *
 * @param i method index
 * @return std::size_t&&
 *
 */
constexpr decltype (auto) get_methods_args_count (Index&& i) {
    return decltype(::boost::hana::size(typename ::std::decay_t<decltype(::boost::hana::at(info::MetaClass<typename ::std::decay_t<T> >::methods_metadata,i))>::arg_types{}))::value;
}

template <class T, class Index>
/**
 * @brief Returns the return type of the method
 *
 * @param i method index
 * @return boost::hana::type
 */
constexpr decltype (auto) get_method_return_type (Index&& i) {
    return detail::get_method_return_type_impl<T>(std::forward<Index>(i));
}

template <class T, std::size_t Index>
/**
 * @brief Returns the return type of the method
 *
 * @return boost::hana::type
 */
constexpr decltype (auto) get_method_return_type () {
    return detail::get_method_return_type_impl<T,Index>();
}

template< class T, class I, class J >
constexpr decltype(auto) method_arg_type (I&& i, J&& j) {
    return detail::get_method_arg_type_impl<T>(std::forward<I>(i),std::forward<J>(j));
}

template< class T, ::std::size_t I, ::std::size_t J >
/**
 * @brief Type template of J-th argument I-th metod
 *
 */
using method_arg_type_t = typename detail::get_method_arg_type_struct<T,I,J>::type;

template< class T, class I>
constexpr decltype(auto) method_return_type (I&& i) {
    return detail::get_method_return_type_impl<T>(std::forward<I>(i));
}

template< class T, ::std::size_t I>
/**
 * @brief Returns the type template of I-th metod
 * @todo new version of method_return_type_t
 */
using method_return_type_t = typename detail::get_method_return_type_struct<T,I>::type;


template< class T, ::std::size_t I>
/**
 * @brief Type template of I-th variable
 *
 */
using variable_type_t = decltype (get_variable<I>(::std::declval<T>()));
template< class T, class I>
using variable_type_index_t = decltype (get_variable(::std::declval<T>(),::std::declval<I>()));

template <class T>
/**
 * @brief Returns the compile-time string of class name
 *
 * @return boost::hana::string
 *
 */
constexpr decltype(auto) get_class_name () {
    return info::MetaClass<typename ::std::decay_t<T> >::name;
}

template <class T, class I>
/**
 * @brief Returns the compile-time string of variable name
 *
 * @param i variable index
 * @return boost::hana::string
 */
constexpr decltype(auto) get_variable_name (I&& i) {
    constexpr auto tmp = info::MetaClass<typename ::std::decay_t<T> >::vars_names;
    return ::boost::hana::at(tmp,i);
}

template <class T, std::size_t I>
/**
 * @brief Returns the compile-time string of variable name
 *
 * @return boost::hana::string
 *
 */
constexpr decltype(auto) get_variable_name () {
    constexpr auto tmp = info::MetaClass<typename ::std::decay_t<T> >::vars_names;
    return ::boost::hana::at_c<I>(tmp);
}

template <class T, class I>
/**
 * @brief Returns the compile-time string of method name
 *
 * @param i method index
 * @return boost::hana::string
 */
constexpr decltype(auto) get_method_name (I&& i) {
    constexpr auto tmp = info::MetaClass<typename ::std::decay_t<T> >::methods_names;
    return ::boost::hana::at(tmp,i);
}

template <class T, std::size_t I>
/**
 * @brief Returns the compile-time string of method name
 *
 * @return boost::hana::string
 */
constexpr decltype(auto) get_method_name () {
    constexpr auto tmp = info::MetaClass<typename ::std::decay_t<T> >::methods_names;
    return ::boost::hana::at_c<I>(tmp);
}

template <class T>
/**
 * @brief Returns the number of varibles
 *
 * @return std::size_t&&
 *
 */
constexpr decltype(auto) get_variables_count () {
    return decltype(::boost::hana::size(info::MetaClass<typename ::std::decay_t<T> >::vars_metadata))::value;
}

template <class T>
/**
 * @brief Returns the number of methods
 *
 * @return std::size_t&&
 *
 */
constexpr decltype(auto) get_methods_count () {
    return decltype(::boost::hana::size(info::MetaClass<typename ::std::decay_t<T> >::methods_metadata))::value;
}

}

#endif // UTILS_HPP
