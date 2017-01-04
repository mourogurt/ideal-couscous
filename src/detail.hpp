#ifndef META_DETAILS_HPP
#define META_DETAILS_HPP

#include <functional>
#include <tuple>
#include <utility>
#include <boost/hana.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#define  CHECK_STR_CHAR(_, i, str) (sizeof(str) > (i) ? str[(i)] : 0),
#define CT_STR(str) BOOST_PP_REPEAT(256, CHECK_STR_CHAR,str) 0

#define HANA_STR(str) boost::hana::string_c<CT_STR(str) >

template <class Tup>
constexpr size_t tupleSize = decltype(
    boost::hana::size(std::declval<Tup>())
)::value;

template<int N = 255> struct counter : public counter<N - 1> {
    static constexpr int value = N;
    static constexpr counter<N-1> prev() { return {}; }
};
template<> struct counter<0> { static constexpr int value = 0; };

namespace std {
template< class T >
constexpr bool is_function_v = is_function<T>::value;

template< class Base, class Derived >
constexpr bool is_base_of_v = is_base_of<Base, Derived>::value;

template< class T >
constexpr bool is_member_pointer_v = is_member_pointer<T>::value;

template< class T, class U >
constexpr bool is_same_v = is_same<T, U>::value;

}

namespace detail {
template <class T>
struct is_reference_wrapper : std::false_type {};
template <class U>
struct is_reference_wrapper<std::reference_wrapper<U>> : std::true_type {};
template <class T>
constexpr bool is_reference_wrapper_v = is_reference_wrapper<T>::value;

template <class Base, class T, class Derived, class... Args>
constexpr auto INVOKE(T Base::*pmf, Derived&& ref, Args&&... args)
    noexcept(noexcept((std::forward<Derived>(ref).*pmf)(std::forward<Args>(args)...)))
 -> std::enable_if_t<std::is_function_v<T> &&
                     std::is_base_of_v<Base, std::decay_t<Derived>>,
    decltype((std::forward<Derived>(ref).*pmf)(std::forward<Args>(args)...))>
{
      return (std::forward<Derived>(ref).*pmf)(std::forward<Args>(args)...);
}

template <class Base, class T, class RefWrap, class... Args>
constexpr auto INVOKE(T Base::*pmf, RefWrap&& ref, Args&&... args)
    noexcept(noexcept((ref.get().*pmf)(std::forward<Args>(args)...)))
 -> std::enable_if_t<std::is_function_v<T> &&
                     is_reference_wrapper_v<std::decay_t<RefWrap>>,
    decltype((ref.get().*pmf)(std::forward<Args>(args)...))>

{
      return (ref.get().*pmf)(std::forward<Args>(args)...);
}

template <class Base, class T, class Pointer, class... Args>
constexpr auto INVOKE(T Base::*pmf, Pointer&& ptr, Args&&... args)
    noexcept(noexcept(((*std::forward<Pointer>(ptr)).*pmf)(std::forward<Args>(args)...)))
 -> std::enable_if_t<std::is_function_v<T> &&
                     !is_reference_wrapper_v<std::decay_t<Pointer>> &&
                     !std::is_base_of_v<Base, std::decay_t<Pointer>>,
    decltype(((*std::forward<Pointer>(ptr)).*pmf)(std::forward<Args>(args)...))>
{
      return ((*std::forward<Pointer>(ptr)).*pmf)(std::forward<Args>(args)...);
}

template <class Base, class T, class Derived>
constexpr auto INVOKE(T Base::*pmd, Derived&& ref)
    noexcept(noexcept(std::forward<Derived>(ref).*pmd))
 -> std::enable_if_t<!std::is_function_v<T> &&
                     std::is_base_of_v<Base, std::decay_t<Derived>>,
    decltype(std::forward<Derived>(ref).*pmd)>
{
      return std::forward<Derived>(ref).*pmd;
}

template <class Base, class T, class RefWrap>
constexpr auto INVOKE(T Base::*pmd, RefWrap&& ref)
    noexcept(noexcept(ref.get().*pmd))
 -> std::enable_if_t<!std::is_function_v<T> &&
                     is_reference_wrapper_v<std::decay_t<RefWrap>>,
    decltype(ref.get().*pmd)>
{
      return ref.get().*pmd;
}

template <class Base, class T, class Pointer>
constexpr auto INVOKE(T Base::*pmd, Pointer&& ptr)
    noexcept(noexcept((*std::forward<Pointer>(ptr)).*pmd))
 -> std::enable_if_t<!std::is_function_v<T> &&
                     !is_reference_wrapper_v<std::decay_t<Pointer>> &&
                     !std::is_base_of_v<Base, std::decay_t<Pointer>>,
    decltype((*std::forward<Pointer>(ptr)).*pmd)>
{
      return (*std::forward<Pointer>(ptr)).*pmd;
}

template <class F, class... Args>
constexpr auto INVOKE(F&& f, Args&&... args)
    noexcept(noexcept(std::forward<F>(f)(std::forward<Args>(args)...)))
 -> std::enable_if_t<!std::is_member_pointer_v<std::decay_t<F>>,
    decltype(std::forward<F>(f)(std::forward<Args>(args)...))>
{
      return std::forward<F>(f)(std::forward<Args>(args)...);
}

template <typename, typename = void>
struct result_of {};
template <typename F, typename...Args>
struct result_of<F(Args...),
                 decltype(void(INVOKE(std::declval<F>(), std::declval<Args>()...)))> {
    using type = decltype(INVOKE(std::declval<F>(), std::declval<Args>()...));
};

constexpr decltype(auto) multiple_concat () { return boost::hana::make_tuple();}

template <class T>
constexpr decltype(auto) multiple_concat (T&& value) {
    return std::forward<T>(value);
}

template <class T, class... Args>
constexpr decltype(auto) multiple_concat (T&& value , Args&&... args) {
    return boost::hana::concat(value,multiple_concat(std::forward<Args>(args)...));
}

template<std::size_t Index, class A, class B, std::enable_if_t<std::is_same_v<A,B>,bool> = 0>
constexpr decltype(auto) compare_types(A const&, B const&) {
    return boost::hana::make_tuple(boost::hana::size_c<Index>);
}

template<std::size_t Index, class A, class B, std::enable_if_t<!std::is_same_v<A,B>,bool> = 0>
constexpr decltype(auto) compare_types(A const&, B const&) {
    return boost::hana::make_tuple();
}

template <class T, class Tp, std::size_t... Indices>
constexpr decltype(auto) find_values_args (std::index_sequence< Indices... >&&, T&& value, Tp&& tup) {
    return multiple_concat(compare_types<Indices>(std::forward<T>(value),boost::hana::at_c<Indices>(std::forward<Tp>(tup)))...);
}

template<class T, class ... Args>
constexpr decltype(auto) find_values (T&& value, boost::hana::tuple<Args...>&&  tup) {
    return  find_values_args(std::make_index_sequence<sizeof...(Args)>(),std::forward<T>(value),std::forward<boost::hana::tuple<Args...>>(tup));
}

template<class T, class ... Args>
constexpr decltype(auto) find_values (T const& value, boost::hana::tuple<Args...> const&  tup) {
    return  find_values_args(std::make_index_sequence<sizeof...(Args)>(),value,tup);
}

template <class T, std::size_t... Indices>
constexpr decltype (auto) vars_names_tuple (std::index_sequence< Indices... >&&) {
    constexpr auto res = detail::multiple_concat(VariableNames(counter<Indices>{},static_cast<const T*>(nullptr))...);
    return res;
}

template <class T, std::size_t... Indices>
constexpr decltype (auto) methods_names_tuple (std::index_sequence< Indices... >&&) {
    constexpr auto res = detail::multiple_concat(MethodNames(counter<Indices>{},static_cast<const T*>(nullptr))...);
    return res;
}

template <class T, std::size_t... Indices>
constexpr decltype (auto) vars_ptr_tuple (std::index_sequence< Indices... >&&) {
    constexpr auto res = detail::multiple_concat(VariablePtrs(counter<Indices>{},static_cast<const T*>(nullptr))...);
    return res;
}

template <class T, std::size_t... Indices>
constexpr decltype (auto) methods_ptr_tuple (std::index_sequence< Indices... >&&) {
    constexpr auto res = detail::multiple_concat(MethodPtrs(counter<Indices>{},static_cast<const T*>(nullptr))...);
    return res;
}

}

template< class F, class... ArgTypes >
constexpr auto constexpr_invoke(F&& f, ArgTypes&&... args)
    // exception specification for QoI
    noexcept(noexcept(detail::INVOKE(std::forward<F>(f), std::forward<ArgTypes>(args)...)))
 -> decltype(detail::INVOKE(std::forward<F>(f), std::forward<ArgTypes>(args)...))
{
    return detail::INVOKE(std::forward<F>(f), std::forward<ArgTypes>(args)...);
}

template <class> struct incomplete_result_of;
template <class F, class... ArgTypes>
struct incomplete_result_of<F(ArgTypes...)> : detail::result_of<F(ArgTypes...)> {};


#endif // UTILS_HPP
