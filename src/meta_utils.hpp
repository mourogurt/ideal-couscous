#ifndef META_UTILS_HPP
#define META_UTILS_HPP
#include "meta_information.hpp"

namespace detail {

template <class Callable, class Tuple, std::size_t ...Indices, class... Args>
constexpr void for_each_impl(Callable&& f,Tuple&& tuple, std::index_sequence<Indices...>, Args&... args) {
    using swallow = int[];
        (void)swallow{1,(f(boost::hana::at_c<Indices>(std::forward<Tuple>(tuple)),args...), void(), int{})...};
}

template <class Callable, class Tuple, std::size_t ...Indices, class... Args>
constexpr decltype(auto) for_each_impl_ret(Callable&& f,Tuple&& tuple, std::index_sequence<Indices...>, Args&... args) {
    return boost::hana::make_tuple (f(boost::hana::at_c<Indices>(std::forward<Tuple>(tuple)),args...)...);
}

template <class Callable, class Tuple, std::size_t ...Indices, class... Args>
constexpr void for_each_impl_index(Callable&& f,Tuple&& tuple, std::index_sequence<Indices...>&&, Args&... args) {
    using swallow = int[];
        (void)swallow{1,(f(std::forward<std::size_t>(Indices),boost::hana::at_c<Indices>(std::forward<Tuple>(tuple)),args...), void(), int{})...};
}

template <class Callable, class Tuple, std::size_t ...Indices, class... Args>
constexpr decltype(auto) for_each_impl_ret_index(Callable&& f,Tuple&& tuple, std::index_sequence<Indices...>&&, Args&... args) {
    return boost::hana::make_tuple (f(std::forward<std::size_t>(Indices),boost::hana::at_c<Indices>(std::forward<Tuple>(tuple)),args...)...);
}

}

namespace utils {
//Normal foreach loop with no results

template <class Callable, class Tuple, class... Args>
constexpr void for_each_tuple_args(Callable&& f, Tuple&& tuple, Args&... args) {
    constexpr std::size_t N = decltype(boost::hana::size(std::forward<Tuple>(tuple)))::value;
    detail::for_each_impl(std::forward<Callable>(f), std::forward<Tuple>(tuple),
                  std::make_index_sequence<N>{}, args...);
}

template <class Callable, class Tuple, class... Args>
constexpr void for_each_tuple_args(Callable&& f, Tuple&& tuple, Args&&... args) {
    constexpr std::size_t N = decltype(boost::hana::size(std::forward<Tuple>(tuple)))::value;
    detail::for_each_impl(std::forward<Callable>(f), std::forward<Tuple>(tuple),
                  std::make_index_sequence<N>{}, std::forward<Args>(args)...);
}


//Foreach with results of lamda function

template <class Callable, class Tuple, class... Args>
constexpr decltype(auto) for_each_tuple_args_ret(Callable&& f, Tuple&& tuple, Args&... args) {
    constexpr std::size_t N = decltype(boost::hana::size(std::forward<Tuple>(tuple)))::value;
    return detail::for_each_impl_ret(std::forward<Callable>(f), std::forward<Tuple>(tuple),
                  std::make_index_sequence<N>{}, args...);
}

template <class Callable, class Tuple, class... Args>
constexpr decltype(auto) for_each_tuple_args_ret(Callable&& f, Tuple&& tuple, Args&&... args) {
    constexpr std::size_t N = decltype(boost::hana::size(std::forward<Tuple>(tuple)))::value;
    return detail::for_each_impl_ret(std::forward<Callable>(f), std::forward<Tuple>(tuple),
                  std::make_index_sequence<N>{}, std::forward<Args>(args)...);
}

//Foreach with first argument as Index

template <class Callable, class Tuple, class... Args>
constexpr void for_each_tuple_args_index(Callable&& f, Tuple&& tuple, Args&... args) {
    constexpr std::size_t N = decltype(boost::hana::size(std::forward<Tuple>(tuple)))::value;
    detail::for_each_impl_index(std::forward<Callable>(f), std::forward<Tuple>(tuple),
                  std::make_index_sequence<N>{}, args...);
}

template <class Callable, class Tuple, class... Args>
constexpr void for_each_tuple_args_index(Callable&& f, Tuple&& tuple, Args&&... args) {
    constexpr std::size_t N = decltype(boost::hana::size(std::forward<Tuple>(tuple)))::value;
    detail::for_each_impl_index(std::forward<Callable>(f), std::forward<Tuple>(tuple),
                  std::make_index_sequence<N>{}, std::forward<Args>(args)...);
}

//Foreach with index and result

template <class Callable, class Tuple, class... Args>
constexpr decltype(auto) for_each_tuple_args_ret_index(Callable&& f, Tuple&& tuple, Args&... args) {
    constexpr std::size_t N = decltype(boost::hana::size(std::forward<Tuple>(tuple)))::value;
    return detail::for_each_impl_ret_index(std::forward<Callable>(f), std::forward<Tuple>(tuple),
                  std::make_index_sequence<N>{}, args...);
}

template <class Callable, class Tuple, class... Args>
constexpr decltype(auto) for_each_tuple_args_ret_index(Callable&& f, Tuple&& tuple, Args&&... args) {
    constexpr std::size_t N = decltype(boost::hana::size(std::forward<Tuple>(tuple)))::value;
    return detail::for_each_impl_ret_index(std::forward<Callable>(f), std::forward<Tuple>(tuple),
                  std::make_index_sequence<N>{}, std::forward<Args>(args)...);
}

template<class Tp, class Char, class Traits>
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
}

}

namespace reflect {

template <class Obj, class T, typename std::enable_if_t<is_reflected_v<std::decay_t<Obj>>,bool> = 1>
constexpr decltype(auto) find_variable_name (T&& name) {
    auto tmp = MetaClass<typename std::decay_t<Obj>>::vars_names;
    return detail::find_values(name,tmp);
}

template <class Obj, class T, typename std::enable_if_t<is_reflected_v<std::decay_t<Obj>>,bool> = 1>
constexpr decltype(auto) find_method_name (T&& name) {
    auto tmp = MetaClass<typename std::decay_t<Obj> >::methods_names;
    return detail::find_values(name,tmp);
}

template <std::size_t I, class T, typename std::enable_if_t<is_reflected_v<std::decay_t<T>>,bool> = 1>
constexpr decltype(auto) get_variable (const T& obj) {
    auto tmp = MetaClass<typename std::decay_t<T> >::vars_metadata;
    return boost::hana::at_c<I>(tmp)(obj);
}

template <class Index, class T, typename std::enable_if_t<is_reflected_v<std::decay_t<T>>,bool> = 1>
constexpr decltype(auto) get_variable (T&& obj, Index&& i) {
    auto tmp = MetaClass<typename std::decay_t<T> >::vars_metadata;
    return boost::hana::at(tmp,i)(std::forward<T>(obj));
}

template <class T, class Index, class... Args>
constexpr decltype(auto) emit_method (T&& obj, Index&& i, Args&&... args) {
    auto tmp = MetaClass<typename std::decay_t<T> >::methods_metadata;
    return boost::hana::at(tmp,i)(std::forward<T>(obj),std::forward<Args>(args)...);
}

template <class T, class Index, class... Args, typename std::enable_if_t<is_reflected_v<std::decay_t<T>>,bool> = 1>
constexpr decltype(auto) emit_method (const T& obj, Index&& i, Args&&... args) {
    auto tmp = MetaClass<typename std::decay_t<T> >::methods_metadata;
    return boost::hana::at(tmp,i)(obj,std::forward<Args>(args)...);
}

}

#endif // UTILS_HPP
