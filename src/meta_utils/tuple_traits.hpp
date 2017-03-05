#ifndef TUPLE_TRAITS_HPP
#define TUPLE_TRAITS_HPP

#include <experimental/type_traits>
#include <boost/hana/tuple.hpp>
#include <boost/hana/concat.hpp>
#include <boost/hana/size.hpp>

namespace reflect {

namespace metautils {

/**
 * @brief Concatenating multiple tuples into one
 *
 */
constexpr decltype(auto) multiple_concat () {
    return ::boost::hana::make_tuple();
}

template <class T>
/**
 * @brief Concatenating multiple tuples into one
 *
 * @param value boost::hana::tuple
 */
constexpr decltype(auto) multiple_concat (T&& value) {
    return ::boost::hana::concat(::std::forward<T>(value),multiple_concat());
}

template <class T, class... Args>
/**
 * @brief Concating multiple tuples into one
 *
 * @param value boost::hana::tuple
 * @param args template pack of boost::hana::tuple
 * @return concatenated tuple
 */
constexpr decltype(auto) multiple_concat (T&& value , Args&&... args) {
    return ::boost::hana::concat(::std::forward<T>(value),multiple_concat(::std::forward<Args>(args)...));
}

namespace detail {

template<::std::size_t Index, class A, class B>
/**
 * @brief Compare types index implementation
 *
 * @param A type A
 * @param B type B
 * @return tuple of index if A and B same type or empty tuple
 */
constexpr decltype(auto) compare_types_index_impl(A&&, B&&) {
    if constexpr (::std::experimental::is_same_v<::std::decay_t<A>,::std::decay_t<B>>) return ::boost::hana::tuple_c<std::size_t,Index>;
    else return ::boost::hana::make_tuple();
}

template <class T, class Tp, ::std::size_t... Indices>
/**
 * @brief Find same type of object in tuple implementation
 *
 * @param std::index_sequence<Indices...> index sequence
 * @param value object to find
 * @param tup tuple where to find
 */
constexpr decltype(auto) find_values_args_impl (::std::index_sequence< Indices... >&&, T&& value, Tp&& tup) {
    return metautils::multiple_concat(compare_types_index_impl<Indices>(::std::forward<T>(value),::boost::hana::at_c<Indices>(::std::forward<Tp>(tup)))...);
}

template<::std::size_t Offset, ::std::size_t... Indices>
/**
 * @brief Tuple range of integral constants implementation
 *
 * @param std::index_sequence<Indices...> index sequence
 */
constexpr decltype (auto) generate_tuple_indices_seq_impl(::std::index_sequence<Indices...>&&) {
    return ::boost::hana::tuple_c<std::size_t,Indices + Offset...>;
}

template <::std::size_t... Indices, class Tuple, class IndexTuple>
constexpr decltype (auto) copy_tuple_sequence_impl (::std::index_sequence<Indices...>&&, Tuple&& tup, IndexTuple&& seq_tup) {
    return ::boost::hana::make_tuple(::boost::hana::at(tup,::boost::hana::at_c<Indices>(seq_tup))...);
}

template <::std::size_t... Indices, class Tuple>
constexpr decltype (auto) merge_tuple_of_tuples_impl (::std::index_sequence<Indices...>&&, Tuple&& tup) {
    return multiple_concat(::boost::hana::at_c<Indices>(tup)...);
}

}

template<class A, class B>
/**
 * @brief compare types
 * @return true if types same, othervise false
 */
constexpr decltype(auto) compare_types(A&&, B&&) noexcept {
    if constexpr (::std::experimental::is_same_v<std::decay_t<A>,std::decay_t<B>>) return true;
    else return false;
}

template<class T, class ... Args>
/**
 * @brief Function that finds same type of object in tuple
 *
 * @param value object to find
 * @param tup tuple where to find
 * @return Tuple of integral constants, which are indexes of tuple elements with same type
 */
constexpr decltype(auto) find_value_types (T&& value, ::boost::hana::tuple<Args...>&&  tup) {
    return  detail::find_values_args_impl(::std::make_index_sequence<sizeof...(Args)>(),::std::forward<T>(value),::std::forward<::boost::hana::tuple<Args...>>(tup));
}

template<class T, class ... Args>
/**
 * @brief Function that finds same type of object in tuple
 *
 * @param value Object to find
 * @param tup Tuple where to find
 * @return tuple of integral constants, which are indexes of tuple elements with same type
 */
constexpr decltype(auto) find_value_types (T const& value, ::boost::hana::tuple<Args...> const&  tup) {
    return  detail::find_values_args_impl(::std::make_index_sequence<sizeof...(Args)>(),value,tup);
}

template <class N, class Offset = ::boost::hana::size_t<0>>
/**
 * @brief Generate tuple integral constants from [Offset; N+Offset)
 * @return Ruple of intergral constants
 *
 */
constexpr decltype (auto) generate_tuple_indices() {
    return detail::generate_tuple_indices_seq_impl<::std::decay_t<Offset>::value>(::std::make_index_sequence<::std::decay_t<N>::value>());
}

template <class Tuple, class IndexTuple>
constexpr decltype (auto) copy_tuple_sequence (Tuple&& tup, IndexTuple&& seq_tup) {
    return detail::copy_tuple_sequence_impl(::std::make_index_sequence<decltype(::boost::hana::size(seq_tup))::value>(),std::forward<Tuple>(tup),std::forward<IndexTuple>(seq_tup));
}

template <class Tuple>
constexpr decltype (auto) merge_tuple_of_tuples (Tuple&& tup) {
    return detail::merge_tuple_of_tuples_impl(::std::make_index_sequence<decltype(::boost::hana::size(tup))::value>(),std::forward<Tuple>(tup));
}

}

}

#endif // TUPLE_TRAITS_HPP
