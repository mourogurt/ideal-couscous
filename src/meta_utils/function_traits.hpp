#ifndef REFL_FUNCTION_TRAITS_HPP
#define REFL_FUNCTION_TRAITS_HPP

#include <utility>
#include <type_traits>
#include <experimental/type_traits>

#include "tuple_traits.hpp"
#include "type_traits.hpp"

namespace reflect {

namespace metautils {

namespace detail {

/**
 * @brief Constexpr invoke implementation for derived class with args
 *
 */
template <class Base, class T, class Derived, class... Args>
constexpr auto INVOKE(T Base::*pmf, Derived&& ref, Args&&... args)
    noexcept(noexcept((::std::forward<Derived>(ref).*pmf)(::std::forward<Args>(args)...)))
 -> ::std::enable_if_t<::std::is_function_v<T> &&
                     ::std::is_base_of_v<Base, ::std::decay_t<Derived>>,
    decltype((::std::forward<Derived>(ref).*pmf)(::std::forward<Args>(args)...))>
{
      return (::std::forward<Derived>(ref).*pmf)(::std::forward<Args>(args)...);
}

/**
 * @brief Constexpr invoke implementation for reference with args
 *
 */
template <class Base, class T, class RefWrap, class... Args>
constexpr auto INVOKE(T Base::*pmf, RefWrap&& ref, Args&&... args)
    noexcept(noexcept((ref.get().*pmf)(::std::forward<Args>(args)...)))
 -> std::enable_if_t<::std::is_function_v<T> &&
                     metautils::is_reference_wrapper_v<::std::decay_t<RefWrap>>,
    decltype((ref.get().*pmf)(::std::forward<Args>(args)...))>
{
      return (ref.get().*pmf)(::std::forward<Args>(args)...);
}

/**
 * @brief Constexpr invoke implementation for pointer with args
 *
 */
template <class Base, class T, class Pointer, class... Args>
constexpr auto INVOKE(T Base::*pmf, Pointer&& ptr, Args&&... args)
    noexcept(noexcept(((*::std::forward<Pointer>(ptr)).*pmf)(::std::forward<Args>(args)...)))
 -> std::enable_if_t<::std::is_function_v<T> &&
                     !metautils::is_reference_wrapper_v<::std::decay_t<Pointer>> &&
                     !::std::is_base_of_v<Base, ::std::decay_t<Pointer>>,
    decltype(((*::std::forward<Pointer>(ptr)).*pmf)(::std::forward<Args>(args)...))>
{
      return ((*::std::forward<Pointer>(ptr)).*pmf)(::std::forward<Args>(args)...);
}

/**
 * @brief Constexpr invoke implementation for derived class
 *
 */
template <class Base, class T, class Derived>
constexpr auto INVOKE(T Base::*pmd, Derived&& ref)
    noexcept(noexcept(::std::forward<Derived>(ref).*pmd))
 -> ::std::enable_if_t<!::std::is_function_v<T> &&
                     ::std::is_base_of_v<Base, ::std::decay_t<Derived>>,
    decltype(::std::forward<Derived>(ref).*pmd)>
{
      return ::std::forward<Derived>(ref).*pmd;
}

/**
 * @brief Constexpr invoke implementation for reference
 *
 */
template <class Base, class T, class RefWrap>
constexpr auto INVOKE(T Base::*pmd, RefWrap&& ref)
    noexcept(noexcept(ref.get().*pmd))
 -> std::enable_if_t<!::std::is_function_v<T> &&
                     metautils::is_reference_wrapper_v<::std::decay_t<RefWrap>>,
    decltype(ref.get().*pmd)>
{
      return ref.get().*pmd;
}

/**
 * @brief Constexpr invoke implementation for pointer
 *
 */
template <class Base, class T, class Pointer>
constexpr auto INVOKE(T Base::*pmd, Pointer&& ptr)
    noexcept(noexcept((*::std::forward<Pointer>(ptr)).*pmd))
 -> std::enable_if_t<!::std::is_function_v<T> &&
                     !metautils::is_reference_wrapper_v<::std::decay_t<Pointer>> &&
                     !::std::is_base_of_v<Base, ::std::decay_t<Pointer>>,
    decltype((*::std::forward<Pointer>(ptr)).*pmd)>
{
      return (*::std::forward<Pointer>(ptr)).*pmd;
}

/**
 * @brief Constexpr invoke implementation
 *
 * @param f function pointer
 * @param args function pointer args(with object pointer/reference)
 * @return std::enable_if_t<_Tp1, _Tp2> enabled if F is member pointer
 */
template <class F, class... Args>
constexpr auto INVOKE(F&& f, Args&&... args)
    noexcept(noexcept(::std::forward<F>(f)(::std::forward<Args>(args)...)))
 -> std::enable_if_t<!::std::is_member_pointer_v<::std::decay_t<F>>,
    decltype(::std::forward<F>(f)(::std::forward<Args>(args)...))>
{
      return ::std::forward<F>(f)(::std::forward<Args>(args)...);
}

/**
 * @brief Foreach implementation
 *
 * @param func function reference/lambda
 * @param tup object to iterate
 * @param args aditional argument passing to function
 */
template<::std::size_t I, class F, class Tuple, class... Args>
constexpr decltype(auto) constexpr_foreach_index_impl (F&& func, Tuple&& tup, Args&&... args) {
    if constexpr (::std::is_void_v<decltype(func(::boost::hana::at_c<I>(tup),::std::forward<Args>(args)...))>) {
        func(::boost::hana::at_c<I>(tup),::std::forward<Args>(args)...);
        return ::boost::hana::make_tuple();
    } else return ::boost::hana::make_tuple(func(::boost::hana::at_c<I>(tup),::std::forward<Args>(args)...));

}

/**
 * @brief Foreach implementation
 *
 * @param std::index_sequence<Indices...> index sequence
 * @param func function reference/lambda
 * @param tup object to iterate
 * @param args aditional argument passing to function
 */
template<::std::size_t... Indices, class F, class Tuple, class... Args>
constexpr decltype(auto) constexpr_foreach_seq_impl(::std::index_sequence<Indices...>&&, F&& func, Tuple&& tup, Args&&... args) {
    return metautils::multiple_concat(constexpr_foreach_index_impl<Indices>(::std::forward<F>(func),::std::forward<Tuple>(tup),::std::forward<Args>(args)...)...);
}

}

/**
 * @brief Constexpr implementation of std::invoke
 *
 * @param f Function pointer
 * @param args Function arguments
 * @return function result
 */
template< class F, class... ArgTypes >
constexpr auto constexpr_invoke(F&& f, ArgTypes&&... args)
    // exception specification for QoI
    noexcept(noexcept(detail::INVOKE(std::forward<F>(f), ::std::forward<ArgTypes>(args)...)))
 -> decltype(detail::INVOKE(::std::forward<F>(f), ::std::forward<ArgTypes>(args)...))
{
    return detail::INVOKE(::std::forward<F>(f), ::std::forward<ArgTypes>(args)...);
}

/**
 * @brief Constexpr foreach iterating over tuple
 *
 * @param func Function pointer/lambda
 * @param tup Object to iterate
 * @param args Aditional argument passing to function
 * @return Tuple of return values or empty tuple if return value is void
 */
template<class F, class Tuple, class... Args>
constexpr decltype(auto) for_each(F&& func, Tuple&& tup, Args&&... args) {
    return detail::constexpr_foreach_seq_impl(::std::make_index_sequence<decltype(::boost::hana::size(tup))::value>(),::std::forward<F>(func),::std::forward<Tuple>(tup),::std::forward<Args>(args)...);
}

}

}

#endif // FUNCTION_TRAITS_HPP
