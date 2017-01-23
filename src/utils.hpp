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

}

namespace reflect {

//INVOKE details
namespace detail {

template <class T>
struct is_reference_wrapper : ::std::false_type {};

template <class U>
struct is_reference_wrapper<::std::reference_wrapper<U>> : ::std::true_type {};

template <class T>
constexpr bool is_reference_wrapper_v = is_reference_wrapper<T>::value;

template <class Base, class T, class Derived, class... Args>
constexpr auto INVOKE(T Base::*pmf, Derived&& ref, Args&&... args)
    noexcept(noexcept((::std::forward<Derived>(ref).*pmf)(::std::forward<Args>(args)...)))
 -> ::std::enable_if_t<::std::is_function_v<T> &&
                     ::std::is_base_of_v<Base, ::std::decay_t<Derived>>,
    decltype((::std::forward<Derived>(ref).*pmf)(::std::forward<Args>(args)...))> {
      return (::std::forward<Derived>(ref).*pmf)(::std::forward<Args>(args)...);
}

template <class Base, class T, class RefWrap, class... Args>
constexpr auto INVOKE(T Base::*pmf, RefWrap&& ref, Args&&... args)
    noexcept(noexcept((ref.get().*pmf)(::std::forward<Args>(args)...)))
 -> std::enable_if_t<::std::is_function_v<T> &&
                     is_reference_wrapper_v<::std::decay_t<RefWrap>>,
    decltype((ref.get().*pmf)(::std::forward<Args>(args)...))> {
      return (ref.get().*pmf)(::std::forward<Args>(args)...);
}

template <class Base, class T, class Pointer, class... Args>
constexpr auto INVOKE(T Base::*pmf, Pointer&& ptr, Args&&... args)
    noexcept(noexcept(((*::std::forward<Pointer>(ptr)).*pmf)(::std::forward<Args>(args)...)))
 -> std::enable_if_t<::std::is_function_v<T> &&
                     !is_reference_wrapper_v<::std::decay_t<Pointer>> &&
                     !::std::is_base_of_v<Base, ::std::decay_t<Pointer>>,
    decltype(((*::std::forward<Pointer>(ptr)).*pmf)(::std::forward<Args>(args)...))> {
      return ((*::std::forward<Pointer>(ptr)).*pmf)(::std::forward<Args>(args)...);
}

template <class Base, class T, class Derived>
constexpr auto INVOKE(T Base::*pmd, Derived&& ref)
    noexcept(noexcept(::std::forward<Derived>(ref).*pmd))
 -> ::std::enable_if_t<!::std::is_function_v<T> &&
                     ::std::is_base_of_v<Base, ::std::decay_t<Derived>>,
    decltype(::std::forward<Derived>(ref).*pmd)> {
      return ::std::forward<Derived>(ref).*pmd;
}

template <class Base, class T, class RefWrap>
constexpr auto INVOKE(T Base::*pmd, RefWrap&& ref)
    noexcept(noexcept(ref.get().*pmd))
 -> std::enable_if_t<!::std::is_function_v<T> &&
                     is_reference_wrapper_v<::std::decay_t<RefWrap>>,
    decltype(ref.get().*pmd)> {
      return ref.get().*pmd;
}

template <class Base, class T, class Pointer>
constexpr auto INVOKE(T Base::*pmd, Pointer&& ptr)
    noexcept(noexcept((*::std::forward<Pointer>(ptr)).*pmd))
 -> std::enable_if_t<!::std::is_function_v<T> &&
                     !is_reference_wrapper_v<::std::decay_t<Pointer>> &&
                     !::std::is_base_of_v<Base, ::std::decay_t<Pointer>>,
    decltype((*::std::forward<Pointer>(ptr)).*pmd)> {
      return (*::std::forward<Pointer>(ptr)).*pmd;
}

template <class F, class... Args>
constexpr auto INVOKE(F&& f, Args&&... args)
    noexcept(noexcept(::std::forward<F>(f)(::std::forward<Args>(args)...)))
 -> std::enable_if_t<!::std::is_member_pointer_v<::std::decay_t<F>>,
    decltype(::std::forward<F>(f)(::std::forward<Args>(args)...))> {
      return ::std::forward<F>(f)(::std::forward<Args>(args)...);
}

template <typename, typename = void>
struct result_of {};
template <typename F, typename...Args>
struct result_of<F(Args...),
                 decltype(void(INVOKE(::std::declval<F>(), ::std::declval<Args>()...)))> {
    using type = decltype(INVOKE(::std::declval<F>(), ::std::declval<Args>()...));
};

template <class Callable, class Tuple, ::std::size_t ...Indices, class... Args>
constexpr void for_each_impl(Callable&& f,Tuple&& tuple, ::std::index_sequence<Indices...>, Args&... args) {
    using swallow = int[];
        (void)swallow{1,(f(::boost::hana::at_c<Indices>(::std::forward<Tuple>(tuple)),args...), void(), int{})...};
}

template <class Callable, class Tuple, ::std::size_t ...Indices, class... Args>
constexpr decltype(auto) for_each_impl_ret(Callable&& f,Tuple&& tuple, ::std::index_sequence<Indices...>, Args&... args) {
    return ::boost::hana::make_tuple (f(::boost::hana::at_c<Indices>(::std::forward<Tuple>(tuple)),args...)...);
}

template <class Callable, class Tuple, ::std::size_t ...Indices, class... Args>
constexpr void for_each_impl_index(Callable&& f,Tuple&& tuple, ::std::index_sequence<Indices...>&&, Args&... args) {
    using swallow = int[];
        (void)swallow{1,(f(::std::forward<::std::size_t>(Indices),::boost::hana::at_c<Indices>(::std::forward<Tuple>(tuple)),args...), void(), int{})...};
}

template <class Callable, class Tuple, ::std::size_t ...Indices, class... Args>
constexpr decltype(auto) for_each_impl_ret_index(Callable&& f,Tuple&& tuple, ::std::index_sequence<Indices...>&&, Args&... args) {
    return ::boost::hana::make_tuple (f(::std::forward<::std::size_t>(Indices),::boost::hana::at_c<Indices>(::std::forward<Tuple>(tuple)),args...)...);
}

template<::std::size_t Index, class A, class B, ::std::enable_if_t<::std::is_same_v<A,B>,bool> = 0>
constexpr decltype(auto) compare_types_impl(A&&, B&&) {
    return ::boost::hana::make_tuple(::boost::hana::size_c<Index>);
}

template<::std::size_t Index, class A, class B, ::std::enable_if_t<!::std::is_same_v<A,B>,bool> = 0>
constexpr decltype(auto) compare_types_impl(A&&, B&&) {
    return ::boost::hana::make_tuple();
}

}

namespace utils {

constexpr decltype(auto) multiple_concat () {
    return boost::hana::make_tuple();
}

template <class T>
constexpr decltype(auto) multiple_concat (T&& value) {
    return ::std::forward<T>(value);
}

template<class A, class B, ::std::enable_if_t<::std::is_same_v<std::decay_t<A>,std::decay_t<B>>,bool> = 0>
constexpr decltype(auto) compare_types(A&&, B&&) {
    return true;
}

template<class A, class B, ::std::enable_if_t<!::std::is_same_v<std::decay_t<A>,std::decay_t<B>>,bool> = 0>
constexpr decltype(auto) compare_types(A&&, B&&) {
    return false;
}

template <class T, class... Args>
constexpr decltype(auto) multiple_concat (T&& value , Args&&... args) {
    return ::boost::hana::concat(value,multiple_concat(::std::forward<Args>(args)...));
}

template <class T, class Tp, ::std::size_t... Indices>
constexpr decltype(auto) find_values_args (::std::index_sequence< Indices... >&&, T&& value, Tp&& tup) {
    return utils::multiple_concat(detail::compare_types_impl<Indices>(::std::forward<T>(value),::boost::hana::at_c<Indices>(::std::forward<Tp>(tup)))...);
}

template<class T, class ... Args>
constexpr decltype(auto) find_values (T&& value, ::boost::hana::tuple<Args...>&&  tup) {
    return  find_values_args(::std::make_index_sequence<sizeof...(Args)>(),::std::forward<T>(value),::std::forward<::boost::hana::tuple<Args...>>(tup));
}

template<class T, class ... Args>
constexpr decltype(auto) find_values (T const& value, ::boost::hana::tuple<Args...> const&  tup) {
    return  find_values_args(::std::make_index_sequence<sizeof...(Args)>(),value,tup);
}

template< class F, class... ArgTypes >
constexpr auto constexpr_invoke(F&& f, ArgTypes&&... args)
    // exception specification for QoI
    noexcept(noexcept(detail::INVOKE(std::forward<F>(f), ::std::forward<ArgTypes>(args)...)))
 -> decltype(detail::INVOKE(::std::forward<F>(f), ::std::forward<ArgTypes>(args)...))
{
    return detail::INVOKE(::std::forward<F>(f), ::std::forward<ArgTypes>(args)...);
}

template <class> struct incomplete_result_of;
template <class F, class... ArgTypes>
struct incomplete_result_of<F(ArgTypes...)> : detail::result_of<F(ArgTypes...)> {};

template <class T>
using incomplete_result_of_t = typename incomplete_result_of<T>::type;

template<int N = 255> struct counter : public counter<N - 1> {
    static constexpr int value = N;
};
template<> struct counter<0> { static constexpr int value = 0; };

//Normal foreach loop with no results

template <class Callable, class Tuple, class... Args>
constexpr void for_each_tuple_args(Callable&& f, Tuple&& tuple, Args&... args) {
    constexpr ::std::size_t N = decltype(::boost::hana::size(::std::forward<Tuple>(tuple)))::value;
    detail::for_each_impl(::std::forward<Callable>(f), ::std::forward<Tuple>(tuple),
                  ::std::make_index_sequence<N>{}, args...);
}

template <class Callable, class Tuple, class... Args>
constexpr void for_each_tuple_args(Callable&& f, Tuple&& tuple, Args&&... args) {
    constexpr ::std::size_t N = decltype(::boost::hana::size(::std::forward<Tuple>(tuple)))::value;
    detail::for_each_impl(::std::forward<Callable>(f), ::std::forward<Tuple>(tuple),
                  ::std::make_index_sequence<N>{}, ::std::forward<Args>(args)...);
}


//Foreach with results of lamda function

template <class Callable, class Tuple, class... Args>
constexpr decltype(auto) for_each_tuple_args_ret(Callable&& f, Tuple&& tuple, Args&... args) {
    constexpr ::std::size_t N = decltype(::boost::hana::size(::std::forward<Tuple>(tuple)))::value;
    return detail::for_each_impl_ret(::std::forward<Callable>(f), ::std::forward<Tuple>(tuple),
                  ::std::make_index_sequence<N>{}, args...);
}

template <class Callable, class Tuple, class... Args>
constexpr decltype(auto) for_each_tuple_args_ret(Callable&& f, Tuple&& tuple, Args&&... args) {
    constexpr ::std::size_t N = decltype(::boost::hana::size(::std::forward<Tuple>(tuple)))::value;
    return detail::for_each_impl_ret(::std::forward<Callable>(f), ::std::forward<Tuple>(tuple),
                  ::std::make_index_sequence<N>{}, ::std::forward<Args>(args)...);
}

//Foreach with first argument as Index

template <class Callable, class Tuple, class... Args>
constexpr void for_each_tuple_args_index(Callable&& f, Tuple&& tuple, Args&... args) {
    constexpr ::std::size_t N = decltype(::boost::hana::size(::std::forward<Tuple>(tuple)))::value;
    detail::for_each_impl_index(::std::forward<Callable>(f), ::std::forward<Tuple>(tuple),
                  ::std::make_index_sequence<N>{}, args...);
}

template <class Callable, class Tuple, class... Args>
constexpr void for_each_tuple_args_index(Callable&& f, Tuple&& tuple, Args&&... args) {
    constexpr ::std::size_t N = decltype(::boost::hana::size(::std::forward<Tuple>(tuple)))::value;
    detail::for_each_impl_index(::std::forward<Callable>(f), ::std::forward<Tuple>(tuple),
                  ::std::make_index_sequence<N>{}, ::std::forward<Args>(args)...);
}

//Foreach with index and result

template <class Callable, class Tuple, class... Args>
constexpr decltype(auto) for_each_tuple_args_ret_index(Callable&& f, Tuple&& tuple, Args&... args) {
    constexpr ::std::size_t N = decltype(::boost::hana::size(::std::forward<Tuple>(tuple)))::value;
    return detail::for_each_impl_ret_index(::std::forward<Callable>(f), std::forward<Tuple>(tuple),
                  ::std::make_index_sequence<N>{}, args...);
}

template <class Callable, class Tuple, class... Args>
constexpr decltype(auto) for_each_tuple_args_ret_index(Callable&& f, Tuple&& tuple, Args&&... args) {
    constexpr ::std::size_t N = decltype(::boost::hana::size(::std::forward<Tuple>(tuple)))::value;
    return detail::for_each_impl_ret_index(std::forward<Callable>(f), ::std::forward<Tuple>(tuple),
                  ::std::make_index_sequence<N>{}, ::std::forward<Args>(args)...);
}

}

}

#define STRING_MAXLEN 64
#define  CHECK_STR_CHAR(_, i, str) (sizeof(str) > (i) ? str[(i)] : 0),
#define CT_STR(str) BOOST_PP_REPEAT(STRING_MAXLEN, CHECK_STR_CHAR,str) 0

#define HANA_STR(str) ::boost::hana::string_c<CT_STR(str) >

#endif // UTILS_HPP
