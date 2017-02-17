#ifndef REFLECT_METADDATA_HPP
#define REFLECT_METADDATA_HPP
#include "reflect_information.hpp"
#include "utils.hpp"

namespace reflect {

namespace detail {

template<class T, class Index, class TmpTuple, class... Args>
constexpr decltype (auto) check_invoke_method_index_impl () {
    using Obj = decltype (::boost::hana::at(::std::declval<TmpTuple>(),::std::declval<Index>()));
    if constexpr (info::is_suitable_types<typename std::decay_t<Obj>,Args...>())
        return ::boost::hana::make_tuple(true);
    else
        return ::boost::hana::make_tuple(false);
}

template<class T, class Metadata, class... Args, class Tuple, ::std::size_t ...Indices>
constexpr decltype (auto) check_invoke_method_impl (Tuple&& tuple, ::std::index_sequence<Indices...>&&) {
    return utils::multiple_concat(check_invoke_method_index_impl<T,decltype(::boost::hana::at_c<Indices>(tuple)),Metadata,Args...>()...);
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
 * @brief Find object variable index by name in class
 *
 * @param name compile-time string
 * @return boost::hana::size_t if found or boost::hana::nothing if not
 */
constexpr decltype(auto) find_obj_variable_index (T&& name) {
    constexpr auto tmp = utils::copy_tuple_sequence(info::MetaClass<typename ::std::decay_t<Obj>>::vars_names,info::MetaClass<typename ::std::decay_t<Obj>>::vars_obj_indices);
    if constexpr (decltype(::boost::hana::size(utils::find_values(name,tmp)))::value != 0)
        return ::boost::hana::at_c<0>(utils::find_values(name,tmp));
    else return ::boost::hana::nothing;
}

template <class Obj, class T, typename ::std::enable_if_t<info::is_reflected_v<::std::decay_t<Obj>>,bool> = 1>
/**
 * @brief Find static variable index by name in class
 *
 * @param name compile-time string
 * @return boost::hana::size_t if found or boost::hana::nothing if not
 */
constexpr decltype(auto) find_static_variable_index (T&& name) {
    constexpr auto tmp = utils::copy_tuple_sequence(info::MetaClass<typename ::std::decay_t<Obj>>::vars_names,info::MetaClass<typename ::std::decay_t<Obj>>::vars_static_indices);
    if constexpr (decltype(::boost::hana::size(utils::find_values(name,tmp)))::value != 0)
        return ::boost::hana::at_c<0>(utils::find_values(name,tmp));
    else return ::boost::hana::nothing;
}

template <class Obj, class T, typename ::std::enable_if_t<info::is_reflected_v<::std::decay_t<Obj>>,bool> = 1>
/**
 * @brief Find variable index by name in class
 *
 * @param name compile-time string
 * @return boost::hana::size_t if found or boost::hana::nothing if not
 */
constexpr decltype(auto) find_variable_index (T&& name) {
    constexpr auto tmp = info::MetaClass<typename ::std::decay_t<Obj>>::vars_names;
    if constexpr (decltype(::boost::hana::size(utils::find_values(name,tmp)))::value != 0)
        return ::boost::hana::at_c<0>(utils::find_values(name,tmp));
    else return ::boost::hana::nothing;
}

template <class Obj, class T, typename ::std::enable_if_t<info::is_reflected_v<::std::decay_t<Obj>>,bool> = 1>
/**
 * @brief Find object method index by name in class
 *
 * @param name compile-time string
 * @return boost::hana::tuple if find indices or boost::hana::nothing if not
 */
constexpr decltype(auto) find_obj_method_index (T&& name) {
    constexpr auto tmp = utils::copy_tuple_sequence(info::MetaClass<typename ::std::decay_t<Obj>>::methods_names,info::MetaClass<typename ::std::decay_t<Obj>>::methods_obj_indices);
    if constexpr (decltype(::boost::hana::size(utils::find_values(name,tmp)))::value != 0)
        return utils::find_values(name,tmp);
    else return ::boost::hana::nothing;
}

template <class Obj, class T, typename ::std::enable_if_t<info::is_reflected_v<::std::decay_t<Obj>>,bool> = 1>
/**
 * @brief Find static method index by name in class
 *
 * @param name compile-time string
 * @return boost::hana::tuple tuple of indices
 */
constexpr decltype(auto) find_static_method_index (T&& name) {
    constexpr auto tmp = utils::copy_tuple_sequence(info::MetaClass<typename ::std::decay_t<Obj>>::methods_names,info::MetaClass<typename ::std::decay_t<Obj>>::methods_static_indices);
    if constexpr (decltype(::boost::hana::size(utils::find_values(name,tmp)))::value != 0)
        return utils::find_values(name,tmp);
    else return ::boost::hana::nothing;
}

template <class Obj, class T, typename ::std::enable_if_t<info::is_reflected_v<::std::decay_t<Obj>>,bool> = 1>
/**
 * @brief Find method index by name in class
 *
 * @param name compile-time string
 * @return boost::hana::tuple tuple of indices
 */
constexpr decltype(auto) find_method_index (T&& name) {
    constexpr auto tmp = info::MetaClass<typename ::std::decay_t<Obj> >::methods_names;
    if constexpr (decltype(::boost::hana::size(utils::find_values(name,tmp)))::value != 0)
        return utils::find_values(name,tmp);
    else return ::boost::hana::nothing;
}

template<class T, class... Args, class IndTuple>
/**
 * @brief Checks whether the object methods can be invoked with given arguments
 *
 * @param indices tuple(boost::hana::size_t)
 * @param args method arguments
 * @return boost::hana::tuple of bools or boost::hana::nothing if ::boost::hana::nothing as indices or if size of indices == 0
 */
constexpr decltype (auto) check_invoke_obj_method (IndTuple&& indices) {
    if constexpr (::std::is_same_v<::std::decay_t<IndTuple>,decltype(::boost::hana::nothing)>) return ::boost::hana::nothing;
    constexpr ::std::size_t N = decltype(::boost::hana::size(::std::forward<IndTuple>(indices)))::value;
    if constexpr (N != 0) {
        using tmp = decltype(utils::copy_tuple_sequence(info::MetaClass<typename ::std::decay_t<T>>::methods_metadata,
                             info::MetaClass<typename ::std::decay_t<T>>::methods_obj_indices));
        return detail::check_invoke_method_impl<T,tmp,Args...>(::std::forward<IndTuple>(indices),::std::make_index_sequence<N>());
    }
    else return ::boost::hana::nothing;
}

template<class T, class... Args, class IndTuple>
/**
 * @brief Checks whether the static methods can be invoked with given arguments
 *
 * @param indices tuple(boost::hana::size_t)
 * @param args method arguments
 * @return boost::hana::tuple of bools or boost::hana::nothing if ::boost::hana::nothing as indices or if size of indices == 0
 */
constexpr decltype (auto) check_invoke_static_method (IndTuple&& indices) {
    if constexpr (::std::is_same_v<::std::decay_t<IndTuple>,decltype(::boost::hana::nothing)>) return ::boost::hana::nothing;
    constexpr ::std::size_t N = decltype(::boost::hana::size(::std::forward<IndTuple>(indices)))::value;
    if constexpr (N != 0) {
        using tmp = decltype(utils::copy_tuple_sequence(info::MetaClass<typename ::std::decay_t<T>>::methods_metadata,
                             info::MetaClass<typename ::std::decay_t<T>>::methods_static_indices));
        return detail::check_invoke_method_impl<T,tmp,Args...>(::std::forward<IndTuple>(indices),::std::make_index_sequence<N>());
    }
    else return ::boost::hana::nothing;
}

template<class T, class... Args, class IndTuple>
/**
 * @brief Checks whether the methods can be invoked with given arguments
 *
 * @param indices tuple(boost::hana::size_t)
 * @param args method arguments
 * @return boost::hana::tuple of bools or boost::hana::nothing if ::boost::hana::nothing as indices or if size of indices == 0
 */
constexpr decltype (auto) check_invoke_method (IndTuple&& indices) {
    if constexpr (::std::is_same_v<::std::decay_t<IndTuple>,decltype(::boost::hana::nothing)>) return ::boost::hana::nothing;
    constexpr ::std::size_t N = decltype(::boost::hana::size(::std::forward<IndTuple>(indices)))::value;
    if constexpr (N != 0) {
        using tmp = decltype(reflect::info::MetaClass<::std::decay_t<T>>::methods_metadata);
        return detail::check_invoke_method_impl<T,tmp,Args...>(::std::forward<IndTuple>(indices),::std::make_index_sequence<N>());
    }
    else return ::boost::hana::nothing;
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

template <class T, typename ::std::enable_if_t<reflect::info::is_reflected_v<typename std::decay_t<T>>,bool> = 1>
/**
 * @brief Returns the number of all varibles
 *
 * @return std::size_t&&
 *
 */
constexpr decltype(auto) get_variables_count () {
    return decltype(::boost::hana::size(reflect::info::MetaClass<::std::decay_t<T>>::vars_metadata))::value;
}

template <class T, typename ::std::enable_if_t<reflect::info::is_reflected_v<typename std::decay_t<T>>,bool> = 1>
/**
 * @brief Returns the number of object varibles
 *
 * @return std::size_t&&
 *
 */
constexpr decltype(auto) get_variables_obj_count () {
    return decltype(::boost::hana::size(reflect::info::MetaClass<::std::decay_t<T>>::vars_obj_indices))::value;
}

template <class T, typename ::std::enable_if_t<reflect::info::is_reflected_v<typename std::decay_t<T>>,bool> = 1>
/**
 * @brief Returns the number of static varibles
 *
 * @return std::size_t&&
 *
 */
constexpr decltype(auto) get_variables_static_count () {
    return decltype(::boost::hana::size(reflect::info::MetaClass<::std::decay_t<T>>::vars_static_indices))::value;
}

template <class T, typename ::std::enable_if_t<reflect::info::is_reflected_v<typename std::decay_t<T>>,bool> = 1>
/**
 * @brief Returns the number of all methods
 *
 * @return std::size_t&&
 *
 */
constexpr decltype(auto) get_methods_count () {
    return decltype(::boost::hana::size(reflect::info::MetaClass<::std::decay_t<T>>::methods_metadata))::value;
}

template <class T, typename ::std::enable_if_t<reflect::info::is_reflected_v<typename std::decay_t<T>>,bool> = 1>
/**
 * @brief Returns the number of object methods
 *
 * @return std::size_t&&
 *
 */
constexpr decltype(auto) get_methods_obj_count () {
    return decltype(::boost::hana::size(reflect::info::MetaClass<::std::decay_t<T>>::methods_obj_indices))::value;
}

template <class T, typename ::std::enable_if_t<reflect::info::is_reflected_v<typename std::decay_t<T>>,bool> = 1>
/**
 * @brief Returns the number of static methods
 *
 * @return std::size_t&&
 *
 */
constexpr decltype(auto) get_methods_static_count () {
    return decltype(::boost::hana::size(reflect::info::MetaClass<::std::decay_t<T>>::methods_static_indices))::value;
}

}

#endif // REFLECT_METADDATA_HPP
