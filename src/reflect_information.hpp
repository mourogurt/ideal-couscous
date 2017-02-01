#ifndef META_INFORMATION_HPP
#define META_INFORMATION_HPP

#include "utils.hpp"

namespace reflect {

namespace detail {

template <class T, ::std::size_t... Indices>
constexpr decltype (auto) vars_names_tuple (::std::index_sequence< Indices... >&&) noexcept {
    constexpr auto res = utils::multiple_concat(VariableNames(utils::counter<Indices>{},static_cast<const T*>(nullptr))...);
    return res;
}

template <class T, ::std::size_t... Indices>
constexpr decltype (auto) methods_names_tuple (::std::index_sequence< Indices... >&&) noexcept {
    constexpr auto res = utils::multiple_concat(MethodNames(utils::counter<Indices>{},static_cast<const T*>(nullptr))...);
    return res;
}

template <class T, ::std::size_t... Indices>
constexpr decltype (auto) vars_ptr_tuple (::std::index_sequence< Indices... >&&) noexcept {
    constexpr auto res = utils::multiple_concat(VariablePtrs(utils::counter<Indices>{},static_cast<const T*>(nullptr))...);
    return res;
}

template <class T, ::std::size_t... Indices>
constexpr decltype (auto) methods_ptr_tuple (::std::index_sequence< Indices... >&&) noexcept {
    constexpr auto res = utils::multiple_concat(MethodPtrs(utils::counter<Indices>{},static_cast<const T*>(nullptr))...);
    return res;
}

template<class T, class... Args>
struct MethodInfo {
    template<class Ret>
    static constexpr auto get(Ret(T::*)(Args...))
     -> Ret(T::*)(Args...);
    template<class Ret>
    static constexpr auto get_ret(Ret(T::*)(Args...))
     -> Ret;
};

template<class T, class... Args>
struct ConstMethodInfo {
    template<class Ret>
    static constexpr auto get(Ret(T::*)(Args...) const)
     -> Ret(T::*)(Args...) const;
};

template <class... Args>
struct FunctionInfo {
    template<class Ret>
    static constexpr auto get()
     -> Ret(*)(Args...);
    template<class Ret>
    static constexpr auto get(Ret(*)(Args...))
     -> Ret(*)(Args...);
};

template <class... Args>
struct is_convertible_multi {
    constexpr static bool value = false;
};

template <class... Args1, class... Args2>
struct is_convertible_multi<::boost::hana::tuple<Args1...>, ::boost::hana::tuple<Args2...>> {
    constexpr static bool value =  ::std::conjunction_v<::std::is_convertible<Args1,Args2>...>;
};

template <class... Args>
constexpr bool is_convertible_multi_v = is_convertible_multi<Args...>::value;

template<class Args_tuple, class ... Args>
constexpr bool is_suitable_types_impl () noexcept {
    if constexpr (sizeof... (Args) != decltype (::boost::hana::size(::std::declval<Args_tuple>()))::value) return false;
    else {
        if constexpr (is_convertible_multi_v<::boost::hana::tuple<Args...>,Args_tuple>) return true;
        else return false;
    }
}

}

namespace info {

template< class Result, class Obj, class... Args >
/**
 * @brief Pointer to member function container
 *
 */
class mem_fn_t final {
    Result Obj::* p; /**< Pointer to member function */
public:
    /**
     * @brief Object type of pointer to member function
     *
     */
    using obj_type = Obj;
    /**
     * @brief Function type
     *
     */
    using type = Result Obj::*;
    /**
     * @brief Function argument types
     *
     */
    using arg_types = ::boost::hana::tuple<Args...>;
    /**
     * @brief Function return type
     *
     */
    using return_type = decltype(utils::constexpr_invoke(std::declval<type>(),std::declval<obj_type>(),std::declval<Args>()...));
    /**
     * @brief Constexpr constructor
     *
     * @param p method pointer
     */
    constexpr mem_fn_t(type p) noexcept: p(p) {}
    template <class... Args2>
    /**
     * @brief Invoke function
     *
     * @param obj member object
     * @param args function arguments
     * @return utils::constexpr_invoke object of mem_fn_t::return_type
     */
    constexpr decltype(auto) operator()(Obj&& obj,Args2&&... args) const noexcept (noexcept (utils::constexpr_invoke(p, obj, ::std::forward<Args2>(args)...))) {
        return utils::constexpr_invoke(p, obj, ::std::forward<Args2>(args)...);
    }
    template <class... Args2>
    /**
     * @brief Invoke function
     *
     * @param obj member object
     * @param args function arguments
     * @return utils::constexpr_invoke object of mem_fn_t::return_type
     */
    constexpr decltype(auto) operator()(const Obj& obj, Args2&&... args) const noexcept (noexcept (utils::constexpr_invoke(p, obj, ::std::forward<Args2>(args)...))) {
        return utils::constexpr_invoke(p, obj, ::std::forward<Args2>(args)...);
    }
    template <class... Args2>
    /**
     * @brief Invoke function
     *
     * @param obj member object
     * @param args function arguments
     * @return utils::constexpr_invoke object of mem_fn_t::return_type
     */
    constexpr decltype(auto) operator()(Obj& obj, Args2&&... args) const noexcept (noexcept (utils::constexpr_invoke(p, obj, ::std::forward<Args2>(args)...))) {
        return utils::constexpr_invoke(p, obj, ::std::forward<Args2>(args)...);
    }
};

template <class Result, class Obj, class... Args>
/**
 * @brief Pointer to static function container
 *
 */
class static_fn_t final {
    Obj* p; /**< Function pointer */
public:
    /**
     * @brief Function type
     *
     */
    using type = Obj*;
    /**
     * @brief Function argument types
     *
     */
    using arg_types = ::boost::hana::tuple<Args...>;
    /**
     * @brief Function return type
     *
     */
    using return_type = decltype(utils::constexpr_invoke(std::declval<type>(),std::declval<Args>()...));
    /**
     * @brief method that indicates that the function - static
     *
     * @return std::true_type
     */
    static constexpr auto is_static () {return std::true_type();}
    /**
     * @brief Constexpr constructor
     *
     * @param p function pointer
     */
    constexpr static_fn_t(type p) noexcept : p(p) {}
    template <class... Args2>
    /**
     * @brief Invoke function
     *
     * @param args function arguments
     * @return utils::constexpr_invoke object of static_fn_t::return_type
     */
    constexpr decltype(auto) operator()(Args2&&... args) const noexcept (noexcept (utils::constexpr_invoke(p, ::std::forward<Args2>(args)...))) {
        return utils::constexpr_invoke(p, ::std::forward<Args2>(args)...);
    }
};

template <class Obj>
/**
 * @brief Pointer container
 *
 */
class static_mem_t final {
    Obj* p; /**< Pointer */
public:
    /**
     * @brief Pointer type
     *
     */
    using type = Obj*;
    /**
     * @brief Tuple pointer type (Needed to unify all pointer structs)
     *
     */
    using arg_types = ::boost::hana::tuple<Obj>;
    /**
     * @brief Object of pointer type
     *
     */
    using return_type = Obj;
    /**
     * @brief method that indicates that the function - static
     *
     * @return std::true_type
     */
    static constexpr auto is_static () {return std::true_type();}
    /**
     * @brief Constexpr constructor
     *
     * @param p pointer
     */
    constexpr static_mem_t(type p) noexcept : p(p) {}
    /**
     * @brief Dereference of pointer
     *
     * @return object of static_mem_t::return_type
     */
    constexpr decltype(auto) operator()() const noexcept {
        return *p;
    }
};

template< class R, class T, class... Args  >
/**
 * @brief Create mem_fn_t
 *
 * @param pm pointer to member field/method
 * @return mem_fn_t<R, T, Args>
 */
constexpr auto make_mem_fn(R T::* pm)
 -> mem_fn_t<R, T, Args...> {
    return {pm};
}

template<class R, class... Args  >
/**
 * @brief Create static_fn_t
 *
 * @param pm function pointer
 * @return static_fn_t<R, ::std::remove_pointer_t<_Tp1>, Args>
 */
constexpr auto make_static_fn(decltype(detail::FunctionInfo<Args...>::template get<R>()) pm)
 -> static_fn_t<R, ::std::remove_pointer_t<decltype(detail::FunctionInfo<Args...>::template get<R>())>, Args...> {
    return {pm};
}

template <class T>
/**
 * @brief Create static_mem_t
 *
 * @param pm pointer
 * @return static_mem_t<T>
 */
constexpr auto make_static_member (T* pm)
 -> static_mem_t<T> {
    return {pm};
}

template <class T>
/**
 * @brief SFINAE check if class is reflected
 *
 */
struct is_reflected
{

    /**
     * @brief If is_reflected method exist
     *
     * @return ::std::true_type
     */
    template <class C> static constexpr ::std::true_type check(decltype(&C::is_reflected));
    /**
     * @brief If is_reflected method doesn't exist
     *
     * @return ::std::false_type
     */
    template <class> static constexpr ::std::false_type check(...);
    static constexpr bool value = ::std::is_same<::std::true_type, decltype(check<T>(nullptr))>::value; /**< true true if reflected, othervise false */
};

template <class T> constexpr bool is_reflected_v = is_reflected<T>::value; /**< Helper variable template for is_reflected */

template <class T>
/**
 * @brief SFINAE check if pointer container (mem_fn_t, static_fn_t, static_mem_t) is static
 *
 */
struct is_static
{

    /**
     * @brief returns true_type if class if static_fn_t, static_mem_t (is_static method exist)
     *
     * @return ::std::true_type
     */
    template <class C> static constexpr ::std::true_type check(decltype(&C::is_static));
    /**
     * @brief return false_type for all other classes
     *
     * @return ::std::false_type
     */
    template <class> static constexpr ::std::false_type check(...);
    static constexpr bool value = ::std::is_same<::std::true_type, decltype(check<T>(nullptr))>::value; /**< true if true_type, othervise false */
};

template <class T> constexpr bool is_static_v = is_static<T>::value; /**< Helper variable template for is_static */

template <class Obj, class... Args>
/**
 * @brief Check if Args convertible to function arguments
 *
 * @return bool
 */
constexpr bool is_suitable_types () noexcept {
    return detail::is_suitable_types_impl<typename Obj::arg_types,Args...>();
}

template <class T, typename ::std::enable_if_t<is_reflected_v<T>,bool> = 1>
/**
 * @brief Class that stores meta-information about T
 *
 */
struct MetaClass {
    static constexpr auto name              {HANA_STR(ClassName(static_cast<const T*>(nullptr)))}; /**< compile-time string of class name */
    static constexpr auto vars_names        {detail::vars_names_tuple<T>(::std::make_index_sequence<decltype(T::Variable_counter(utils::counter<>{}))::value>{})}; /**< tuple of class variable names */
    static constexpr auto methods_names     {detail::methods_names_tuple<T>(::std::make_index_sequence<decltype(T::Method_counter(utils::counter<>{}))::value>{})}; /**< tuple of class method names  */
    static constexpr auto vars_metadata     {detail::vars_ptr_tuple<T>(::std::make_index_sequence<decltype(T::Variable_counter(utils::counter<>{}))::value>{})}; /**< tuple of class variable pointers */
    static constexpr auto methods_metadata  {detail::methods_ptr_tuple<T>(::std::make_index_sequence<decltype(T::Method_counter(utils::counter<>{}))::value>{})}; /**< tuple of class method pointers */
};

}

}

#define METACLASS_DEFINITION(TYPE) \
        using Type = TYPE; \
        static constexpr auto is_reflected () {return std::true_type();} \
        friend constexpr auto ClassName(const Type*) RETURN(#TYPE)\
        constexpr static ::reflect::utils::counter<0> Variable_counter (::reflect::utils::counter<0>);\
        constexpr static ::reflect::utils::counter<0> Method_counter (::reflect::utils::counter<0>);

#define RETURN(R) -> decltype(R) { return R; }

#define TUPLE_APPEND(STATE, COUNTER, ...) \
    friend constexpr auto STATE(::reflect::utils::counter<decltype(COUNTER(::reflect::utils::counter<>{}))::value>, \
            const Type*) \
        RETURN( ::boost::hana::make_tuple(__VA_ARGS__))

#define TUPLE_APPEND2(STATE, COUNTER, ...) \
    TUPLE_APPEND(STATE, COUNTER, __VA_ARGS__) \
    constexpr static ::reflect::utils::counter<decltype(COUNTER(::reflect::utils::counter<>{}))::value+1u> COUNTER (::reflect::utils::counter<decltype(COUNTER(::reflect::utils::counter<>{}))::value+1u>);

#define REFLECT_VARIABLE(NAME) \
    TUPLE_APPEND (VariableNames,Variable_counter,HANA_STR(#NAME)) \
    TUPLE_APPEND2(VariablePtrs,Variable_counter,::reflect::info::make_mem_fn<decltype(NAME), Type>(&Type::NAME))

#define REFLECT_METHOD(NAME,...) \
    TUPLE_APPEND (MethodNames,Method_counter,HANA_STR(#NAME)) \
    TUPLE_APPEND2(MethodPtrs,Method_counter,::reflect::info::make_mem_fn<::reflect::utils::constexpr_result_of_t<decltype(::reflect::detail::MethodInfo<Type, ##__VA_ARGS__>::get(&Type::NAME))\
    (Type, ##__VA_ARGS__)> (__VA_ARGS__),Type, ##__VA_ARGS__>(&Type::NAME))

#define REFLECT_STATIC_VARIABLE(NAME) \
    TUPLE_APPEND (VariableNames,Variable_counter,HANA_STR(#NAME)) \
    TUPLE_APPEND2(VariablePtrs,Variable_counter,::reflect::info::make_static_member(&Type::NAME))

#define REFLECT_STATIC_METHOD(NAME,...) \
    TUPLE_APPEND (MethodNames,Method_counter,HANA_STR(#NAME)) \
    TUPLE_APPEND2(MethodPtrs,Method_counter,::reflect::info::make_static_fn<::reflect::utils::constexpr_result_of_t<decltype(::reflect::detail::FunctionInfo<__VA_ARGS__>::get(&Type::NAME)) \
    (__VA_ARGS__)>, ##__VA_ARGS__>(Type::NAME))

#define REFLECT_CONST_METHOD(NAME,...) \
    TUPLE_APPEND (MethodNames,Method_counter,HANA_STR(#NAME)) \
    TUPLE_APPEND2(MethodPtrs,Method_counter,::reflect::info::make_mem_fn<::reflect::utils::constexpr_result_of_t<decltype(::reflect::detail::ConstMethodInfo<Type, ##__VA_ARGS__>::get(&Type::NAME)) \
    (Type, ##__VA_ARGS__)>(__VA_ARGS__)const, Type, ##__VA_ARGS__>(&Type::NAME))

#endif // META_INFORMATION_HPP
