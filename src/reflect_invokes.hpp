#ifndef REFLECT_INVOKES_HPP
#define REFLECT_INVOKES_HPP
#include "reflect_information.hpp"
#include "utils.hpp"

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

template<class T, class Tuple, ::std::size_t ...Indices, class... Args>
constexpr decltype (auto) try_invoke_method_impl (T&& obj, Tuple&& tuple, ::std::index_sequence<Indices...>&&, Args&&... args) {
    constexpr auto tmp = reflect::info::MetaClass<::std::decay_t<T>>::methods_metadata;
    return utils::multiple_concat(try_invoke_method_index_impl(::std::forward<T>(obj),::boost::hana::at_c<Indices>(tuple),tmp,::std::forward<Args>(args)...)...);
}

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

}

#endif // REFLECT_INVOKES_HPP
