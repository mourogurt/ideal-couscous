#ifndef META_DETAILS_HPP
#define META_DETAILS_HPP

#include <type_traits>
#include <boost/hana.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>

//FIXME: Remove latter
namespace std {

template< class T >
constexpr bool is_function_v = is_function<T>::value;

template< class Base, class Derived >
constexpr bool is_base_of_v = is_base_of<Base, Derived>::value;

template< class T >
constexpr bool is_member_pointer_v = is_member_pointer<T>::value;

template< class T, class U >
constexpr bool is_same_v = is_same<T, U>::value;

template< class T, class U >
constexpr bool is_convertible_v = is_convertible<T, U>::value;

template< class T >
constexpr bool is_void_v = is_void<T>::value;

template < class... Args >
constexpr bool conjunction_v = conjunction<Args...>::value;

}

namespace reflect {

namespace utils {

/**
 * @brief Concatenating multiple tuples into one
 *
 */
constexpr decltype(auto) multiple_concat () {
    return boost::hana::make_tuple();
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
    return ::boost::hana::concat(std::forward<T>(value),multiple_concat(::std::forward<Args>(args)...));
}

template <class T>
/**
 * @brief Check if type is std::reference_wrapper
 *
 */
struct is_reference_wrapper : ::std::false_type {};

template <class U>
/**
 * @brief Check if type is std::reference_wrapper
 *
 */
struct is_reference_wrapper<::std::reference_wrapper<U>> : ::std::true_type {};

template <class T>
constexpr bool is_reference_wrapper_v = is_reference_wrapper<T>::value; /**< Helper variable template for is_reference_wrapper */

}

namespace detail {

template <class Base, class T, class Derived, class... Args>
/**
 * @brief Constexpr invoke implementation for derived class with args
 *
 */
constexpr auto INVOKE(T Base::*pmf, Derived&& ref, Args&&... args)
    noexcept(noexcept((::std::forward<Derived>(ref).*pmf)(::std::forward<Args>(args)...)))
 -> ::std::enable_if_t<::std::is_function_v<T> &&
                     ::std::is_base_of_v<Base, ::std::decay_t<Derived>>,
    decltype((::std::forward<Derived>(ref).*pmf)(::std::forward<Args>(args)...))>
{
      return (::std::forward<Derived>(ref).*pmf)(::std::forward<Args>(args)...);
}

template <class Base, class T, class RefWrap, class... Args>
/**
 * @brief Constexpr invoke implementation for reference with args
 *
 */
constexpr auto INVOKE(T Base::*pmf, RefWrap&& ref, Args&&... args)
    noexcept(noexcept((ref.get().*pmf)(::std::forward<Args>(args)...)))
 -> std::enable_if_t<::std::is_function_v<T> &&
                     utils::is_reference_wrapper_v<::std::decay_t<RefWrap>>,
    decltype((ref.get().*pmf)(::std::forward<Args>(args)...))>
{
      return (ref.get().*pmf)(::std::forward<Args>(args)...);
}

template <class Base, class T, class Pointer, class... Args>
/**
 * @brief Constexpr invoke implementation for pointer with args
 *
 */
constexpr auto INVOKE(T Base::*pmf, Pointer&& ptr, Args&&... args)
    noexcept(noexcept(((*::std::forward<Pointer>(ptr)).*pmf)(::std::forward<Args>(args)...)))
 -> std::enable_if_t<::std::is_function_v<T> &&
                     !utils::is_reference_wrapper_v<::std::decay_t<Pointer>> &&
                     !::std::is_base_of_v<Base, ::std::decay_t<Pointer>>,
    decltype(((*::std::forward<Pointer>(ptr)).*pmf)(::std::forward<Args>(args)...))>
{
      return ((*::std::forward<Pointer>(ptr)).*pmf)(::std::forward<Args>(args)...);
}

template <class Base, class T, class Derived>
/**
 * @brief Constexpr invoke implementation for derived class
 *
 */
constexpr auto INVOKE(T Base::*pmd, Derived&& ref)
    noexcept(noexcept(::std::forward<Derived>(ref).*pmd))
 -> ::std::enable_if_t<!::std::is_function_v<T> &&
                     ::std::is_base_of_v<Base, ::std::decay_t<Derived>>,
    decltype(::std::forward<Derived>(ref).*pmd)>
{
      return ::std::forward<Derived>(ref).*pmd;
}

template <class Base, class T, class RefWrap>
/**
 * @brief Constexpr invoke implementation for reference
 *
 */
constexpr auto INVOKE(T Base::*pmd, RefWrap&& ref)
    noexcept(noexcept(ref.get().*pmd))
 -> std::enable_if_t<!::std::is_function_v<T> &&
                     utils::is_reference_wrapper_v<::std::decay_t<RefWrap>>,
    decltype(ref.get().*pmd)>
{
      return ref.get().*pmd;
}

template <class Base, class T, class Pointer>
/**
 * @brief Constexpr invoke implementation for pointer
 *
 */
constexpr auto INVOKE(T Base::*pmd, Pointer&& ptr)
    noexcept(noexcept((*::std::forward<Pointer>(ptr)).*pmd))
 -> std::enable_if_t<!::std::is_function_v<T> &&
                     !utils::is_reference_wrapper_v<::std::decay_t<Pointer>> &&
                     !::std::is_base_of_v<Base, ::std::decay_t<Pointer>>,
    decltype((*::std::forward<Pointer>(ptr)).*pmd)>
{
      return (*::std::forward<Pointer>(ptr)).*pmd;
}

template <class F, class... Args>
/**
 * @brief Constexpr invoke implementation
 *
 * @param f function pointer
 * @param args function pointer args(with object pointer/reference)
 * @return std::enable_if_t<_Tp1, _Tp2> enabled if F is member pointer
 */
constexpr auto INVOKE(F&& f, Args&&... args)
    noexcept(noexcept(::std::forward<F>(f)(::std::forward<Args>(args)...)))
 -> std::enable_if_t<!::std::is_member_pointer_v<::std::decay_t<F>>,
    decltype(::std::forward<F>(f)(::std::forward<Args>(args)...))>
{
      return ::std::forward<F>(f)(::std::forward<Args>(args)...);
}

template <class, class = void>
/**
 * @brief Constexpr result_of implementation for no types
 *
 */
struct result_of {};
template <typename F, typename...Args>
/**
 * @brief Constexpr result_of implementation
 *
 */
struct result_of<F(Args...),
                 decltype(void(INVOKE(::std::declval<F>(), ::std::declval<Args>()...)))> {
    /**
     * @brief
     *
     */
    using type = decltype(INVOKE(::std::declval<F>(), ::std::declval<Args>()...));
};

template<::std::size_t Index, class A, class B>
/**
 * @brief Compare types index implementation
 *
 * @param A type A
 * @param B type B
 * @return tuple of index if A and B same type or empty tuple
 */
constexpr decltype(auto) compare_types_index_impl(A&&, B&&) {
    if constexpr (::std::is_same_v<A,B>) return ::boost::hana::tuple_c<std::size_t,Index>;
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
    return utils::multiple_concat(detail::compare_types_index_impl<Indices>(::std::forward<T>(value),::boost::hana::at_c<Indices>(::std::forward<Tp>(tup)))...);
}

template<::std::size_t I, class F, class Tuple, class... Args>
/**
 * @brief Foreach implementation
 *
 * @param func function reference/lambda
 * @param tup object to iterate
 * @param args aditional argument passing to function
 */
constexpr decltype(auto) constexpr_foreach_index_impl (F&& func, Tuple&& tup, Args&&... args) {
    if constexpr (::std::is_void_v<decltype(func(::boost::hana::at_c<I>(tup),::std::forward<Args>(args)...))>) {
        func(::boost::hana::at_c<I>(tup),::std::forward<Args>(args)...);
        return ::boost::hana::make_tuple();
    } else return ::boost::hana::make_tuple(func(::boost::hana::at_c<I>(tup),::std::forward<Args>(args)...));

}

template<::std::size_t... Indices, class F, class Tuple, class... Args>
/**
 * @brief Foreach implementation
 *
 * @param std::index_sequence<Indices...> index sequence
 * @param func function reference/lambda
 * @param tup object to iterate
 * @param args aditional argument passing to function
 */
constexpr decltype(auto) constexpr_foreach_seq_impl(::std::index_sequence<Indices...>&&, F&& func, Tuple&& tup, Args&&... args) {
    return utils::multiple_concat(constexpr_foreach_index_impl<Indices>(::std::forward<F>(func),::std::forward<Tuple>(tup),::std::forward<Args>(args)...)...);
}

template<std::size_t Offset, ::std::size_t... Indices>
/**
 * @brief Tuple range of integral constants implementation
 *
 * @param std::index_sequence<Indices...> index sequence
 */
constexpr decltype (auto) generate_tuple_indices_seq_impl(::std::index_sequence<Indices...>&&) {
    return ::boost::hana::tuple_c<std::size_t,Indices + Offset...>;
}

template <std::size_t... Indices, class Tuple, class IndexTuple>
constexpr decltype (auto) copy_tuple_sequence_impl (::std::index_sequence<Indices...>&&, Tuple&& tup, IndexTuple&& seq_tup) {
    return ::boost::hana::make_tuple(::boost::hana::at(tup,::boost::hana::at_c<Indices>(seq_tup))...);
}

}

namespace utils {

template<class A, class B>
/**
 * @brief compare types
 * @return true if types same, othervise false
 */
constexpr decltype(auto) compare_types(A&&, B&&) noexcept {
    if constexpr (::std::is_same_v<std::decay_t<A>,std::decay_t<B>>) return true;
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
constexpr decltype(auto) find_values (T&& value, ::boost::hana::tuple<Args...>&&  tup) {
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
constexpr decltype(auto) find_values (T const& value, ::boost::hana::tuple<Args...> const&  tup) {
    return  detail::find_values_args_impl(::std::make_index_sequence<sizeof...(Args)>(),value,tup);
}

template< class F, class... ArgTypes >
/**
 * @brief Constexpr variant of std::invoke
 *
 * @param f Function pointer
 * @param args Function arguments
 * @return function result
 */
constexpr auto constexpr_invoke(F&& f, ArgTypes&&... args)
    // exception specification for QoI
    noexcept(noexcept(detail::INVOKE(std::forward<F>(f), ::std::forward<ArgTypes>(args)...)))
 -> decltype(detail::INVOKE(::std::forward<F>(f), ::std::forward<ArgTypes>(args)...))
{
    return detail::INVOKE(::std::forward<F>(f), ::std::forward<ArgTypes>(args)...);
}

template <class>
/**
 * @brief Constexpr variant of std::result_of
 *
 */
struct constexpr_result_of;
template <class F, class... ArgTypes>
/**
 * @brief Constexpr variant of std::result_of
 *
 */
struct constexpr_result_of<F(ArgTypes...)> : detail::result_of<F(ArgTypes...)> {};

template <class T>
/**
 * @brief Helper type template for constexpr_result_of
 *
 */
using constexpr_result_of_t = typename constexpr_result_of<T>::type;

/**
 * @brief Compile-time counter
 *
 */
template<::std::size_t N = 255> struct counter : public counter<N - 1> {
    static constexpr ::std::size_t value = N; /**< value of counter */
};
/**
 * @brief Zero counter
 *
 */
template<> struct counter<0> { static constexpr ::std::size_t value = 0; };

template<class F, class Tuple, class... Args>
/**
 * @brief Constexpr foreach iterating over tuple
 *
 * @param func Function pointer/lambda
 * @param tup Object to iterate
 * @param args Aditional argument passing to function
 * @return Tuple of return values or empty tuple if return value is void
 */
constexpr decltype(auto) for_each(F&& func, Tuple&& tup, Args&&... args) {
    return detail::constexpr_foreach_seq_impl(::std::make_index_sequence<decltype(::boost::hana::size(tup))::value>(),::std::forward<F>(func),::std::forward<Tuple>(tup),::std::forward<Args>(args)...);
}

template <::std::size_t N, ::std::size_t Offset = 0>
/**
 * @brief Generate tuple integral constants from [Offset; N+Offset)
 * @return Ruple of intergral constants
 *
 */
constexpr decltype (auto) generate_tuple_indices() {
    return detail::generate_tuple_indices_seq_impl<Offset>(::std::make_index_sequence<N>());
}

template <class Tuple, class IndexTuple>
constexpr decltype (auto) copy_tuple_sequence (Tuple&& tup, IndexTuple&& seq_tup) {
    return detail::copy_tuple_sequence_impl(::std::make_index_sequence<decltype(::boost::hana::size(seq_tup))::value>(),std::forward<Tuple>(tup),std::forward<IndexTuple>(seq_tup));
}

}

}

#define STRING_MAXLEN 64 /**< maximum length of compile-time string */
#define  CHECK_STR_CHAR(_, i, str) (sizeof(str) > (i) ? str[(i)] : 0),
#define CT_STR(str) BOOST_PP_REPEAT(STRING_MAXLEN, CHECK_STR_CHAR,str) 0

#define HANA_STR(str) ::boost::hana::string_c<CT_STR(str) > /**<constexpr compile-time string */

#endif // UTILS_HPP
