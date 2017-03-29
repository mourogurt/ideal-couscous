#ifndef REFLECT_INFORMATION_METHOD_HPP
#define REFLECT_INFORMATION_METHOD_HPP

#include "../../meta_utils/meta_utils.hpp"
#include "../types/reflect_information_object.hpp"
#include "../types/reflect_information_static.hpp"

namespace reflect {

namespace info {

namespace detail {

/**
 * @brief Helper struct to get object method types
 *
 */
template<class T, class... Args>
struct MethodInfo {
    template<class Ret>
    static constexpr auto pointer_type(Ret(T::*)(Args...)) -> Ret(T::*)(Args...);

    template<class Ret>
    static constexpr auto pointer_type(Ret(T::*)(Args...) const) -> Ret(T::*)(Args...) const;

    template<class Ret>
    static constexpr auto return_type(Ret(T::*)(Args...)) -> Ret;

    template<class Ret>
    static constexpr auto return_type(Ret(T::*)(Args...) const) -> Ret const;
};

/**
 * @brief Helper struct to get static method types
 *
 */
template<class... Args>
struct MethodInfo<void,Args...> {

    template<class Ret>
    static constexpr auto pointer_type(Ret(*)(Args...)) -> Ret(*)(Args...);

    template<class Ret>
    static constexpr auto return_type(Ret(*)(Args...)) -> Ret;
};

}

/**
 * @brief SFINAE check if type is pointer to method
 *
 */
template <class T>
struct is_method
{
    template <class C> static constexpr ::std::true_type check(decltype(&C::is_method));
    template <class> static constexpr ::std::false_type check(...);
    static constexpr bool value = ::std::is_same<::std::true_type, decltype(check<T>(nullptr))>::value;
};

template <class T> constexpr bool is_method_v = is_method<T>::value; /**< Helper variable template for is_variable */

namespace detail {

}

/**
 * @brief The MethodIndexGenerator class  - generate indices where methods are located
 */
template <class ParentGenerator, bool condition = true>
class MethodIndexGenerator final {
    template <class Item, std::size_t Index>
    constexpr static decltype (auto) check_metadata_variable () {
        if constexpr (is_method_v<::std::decay_t<Item>> == condition) return ::boost::hana::make_tuple(::boost::hana::size_c<Index>);
        else return ::boost::hana::make_tuple();
    }

    template <class Tuple, ::std::size_t... Indices>
    constexpr static decltype (auto) generate_impl (const ::boost::hana::tuple<::boost::hana::size_t<Indices>...>&) {
        return metautils::multiple_concat(check_metadata_variable<decltype (::boost::hana::at_c<Indices>(::std::declval<Tuple>())),Indices>()...);
    }
public:
    using reverse = MethodIndexGenerator<ParentGenerator,!condition>; /**< Reverse generator */

    /**
     * @brief generate function
     * @return ::boost::hana::tuple of indices
     */
    template <class Tuple>
    constexpr static decltype (auto) generate () {
        return generate_impl<Tuple>(ParentGenerator::template generate<Tuple>());
    }
};

/**
 * @brief SFINAE check if type is pointer to constant method
 *
 */
template <class T>
struct is_const_method
{
    template <class C> static constexpr ::std::true_type check(decltype(&C::is_const_method));
    template <class> static constexpr ::std::false_type check(...);
    static constexpr bool value = ::std::is_same<::std::true_type, decltype(check<T>(nullptr))>::value;
};

template <class T> constexpr bool is_const_method_v = is_const_method<T>::value; /**< Helper variable template for is_variable */

namespace detail {

}

/**
 * @brief The ConstMethodIndexGenerator class  - generate indices where const methods are located
 */
template <class ParentGenerator, bool condition = true>
class ConstMethodIndexGenerator final {
    template <class Item, std::size_t Index>
    constexpr static decltype (auto) check_metadata_variable () {
        if constexpr (is_const_method_v<::std::decay_t<Item>> == condition) return ::boost::hana::make_tuple(::boost::hana::size_c<Index>);
        else return ::boost::hana::make_tuple();
    }

    template <class Tuple, ::std::size_t... Indices>
    constexpr static decltype (auto) generate_impl (const ::boost::hana::tuple<::boost::hana::size_t<Indices>...>&) {
        return metautils::multiple_concat(check_metadata_variable<decltype (::boost::hana::at_c<Indices>(::std::declval<Tuple>())),Indices>()...);
    }
public:
    using reverse = ConstMethodIndexGenerator<ParentGenerator,!condition>; /**< Reverse generator */

    /**
     * @brief generate function
     * @return ::boost::hana::tuple of indices
     */
    template <class Tuple>
    constexpr static decltype (auto) generate () {
        return generate_impl<Tuple>(ParentGenerator::template generate<Tuple>());
    }
};

/**
 * @brief Pointer to object method container
 *
 */
template< class Result, class Obj, class... Args >
class obj_method_t final {
    Result Obj::* p; /**< Pointer to object method */
public:

    using obj_type = Obj; /**< Object type of pointer to object method */

    using type = Result Obj::*; /**< Method type */

    using arg_types = ::boost::hana::tuple<Obj,Args...>; /**< Method arguments type */

    using return_type = decltype(metautils::constexpr_invoke(std::declval<type>(),std::declval<obj_type>(),std::declval<Args>()...)); /**< Method return type */

    /**
     * @brief method that indicates that the pointer - method
     *
     * @return std::true_type
     */
    static constexpr auto is_method () {return std::true_type();}

    /**
     * @brief method that indicates that the pointer - pointer to object method
     *
     * @return std::true_type
     */
    static constexpr auto is_object () {return std::true_type();}

    /**
     * @brief Constexpr constructor
     *
     * @param p method pointer
     */
    constexpr obj_method_t(type p) noexcept: p(p) {}

    /**
     * @brief Invoke function
     *
     * @param obj member object
     * @param args function arguments
     * @return utils::constexpr_invoke object of mem_fn_t::return_type
     */
    template <class... Args2>
    constexpr decltype(auto) operator()(Obj&& obj,Args2&&... args) const noexcept (noexcept (metautils::constexpr_invoke(p, obj, ::std::forward<Args2>(args)...))) {
        return metautils::constexpr_invoke(p, obj, ::std::forward<Args2>(args)...);
    }

    /**
     * @brief Invoke method
     *
     * @param obj member object
     * @param args function arguments
     * @return utils::constexpr_invoke object of mem_fn_t::return_type
     */
    template <class... Args2>
    constexpr decltype(auto) operator()(const Obj& obj, Args2&&... args) const noexcept (noexcept (metautils::constexpr_invoke(p, obj, ::std::forward<Args2>(args)...))) {
        return metautils::constexpr_invoke(p, obj, ::std::forward<Args2>(args)...);
    }

    /**
     * @brief Invoke method
     *
     * @param obj member object
     * @param args function arguments
     * @return utils::constexpr_invoke object of mem_fn_t::return_type
     */
    template <class... Args2>
    constexpr decltype(auto) operator()(Obj& obj, Args2&&... args) const noexcept (noexcept (metautils::constexpr_invoke(p, obj, ::std::forward<Args2>(args)...))) {
        return metautils::constexpr_invoke(p, obj, ::std::forward<Args2>(args)...);
    }
};

/**
 * @brief Pointer to const object method container
 *
 */
template< class Result, class Obj, class... Args >
class obj_const_method_t final {
    Result const Obj::* p; /**< Pointer to const object method */
public:

    using obj_type = Obj; /**< Object type of pointer to const object method */

    using type = Result const Obj::*; /**< Method type */

    using arg_types = ::boost::hana::tuple<Obj,Args...>; /**< Method arguments type */

    using return_type = decltype(metautils::constexpr_invoke(std::declval<type>(),std::declval<obj_type>(),std::declval<Args>()...)); /**< Method return type */

    /**
     * @brief method that indicates that the pointer - const method
     *
     * @return std::true_type
     */
    static constexpr auto is_const_method () {return std::true_type();}

    /**
     * @brief method that indicates that the pointer - method
     *
     * @return std::true_type
     */
    static constexpr auto is_method () {return std::true_type();}

    /**
     * @brief method that indicates that the pointer - pointer to object method
     *
     * @return std::true_type
     */
    static constexpr auto is_object () {return std::true_type();}

    /**
     * @brief Constexpr constructor
     *
     * @param p method pointer
     */
    constexpr obj_const_method_t(type p) noexcept: p(p) {}

    /**
     * @brief Invoke method
     *
     * @param obj member object
     * @param args function arguments
     * @return utils::constexpr_invoke object of mem_fn_t::return_type
     */
    template <class... Args2>
    constexpr decltype(auto) operator()(Obj&& obj,Args2&&... args) const noexcept (noexcept (metautils::constexpr_invoke(p, obj, ::std::forward<Args2>(args)...))) {
        return metautils::constexpr_invoke(p, obj, ::std::forward<Args2>(args)...);
    }

    /**
     * @brief Invoke method
     *
     * @param obj member object
     * @param args function arguments
     * @return utils::constexpr_invoke object of mem_fn_t::return_type
     */
    template <class... Args2>
    constexpr decltype(auto) operator()(const Obj& obj, Args2&&... args) const noexcept (noexcept (metautils::constexpr_invoke(p, obj, ::std::forward<Args2>(args)...))) {
        return metautils::constexpr_invoke(p, obj, ::std::forward<Args2>(args)...);
    }

    /**
     * @brief Invoke method
     *
     * @param obj member object
     * @param args function arguments
     * @return utils::constexpr_invoke object of mem_fn_t::return_type
     */
    template <class... Args2>
    constexpr decltype(auto) operator()(Obj& obj, Args2&&... args) const noexcept (noexcept (metautils::constexpr_invoke(p, obj, ::std::forward<Args2>(args)...))) {
        return metautils::constexpr_invoke(p, obj, ::std::forward<Args2>(args)...);
    }
};

/**
 * @brief Pointer to static method container
 *
 */
template <class Obj, class... Args>
class static_method_t final {
    Obj* p; /**< Static method pointer */
public:

    using type = Obj*; /**< Method type */

    using arg_types = ::boost::hana::tuple<Args...>; /**< Method arguments type */

    using return_type = decltype(metautils::constexpr_invoke(std::declval<type>(),std::declval<Args>()...)); /**< Method return type */

    /**
     * @brief method that indicates that the pointer - method
     *
     * @return std::true_type
     */
    static constexpr auto is_method () {return std::true_type();}

    /**
     * @brief method that indicates that the method - static
     *
     * @return std::true_type
     */
    static constexpr auto is_static () {return std::true_type();}

    /**
     * @brief Constexpr constructor
     *
     * @param p function pointer
     */
    constexpr static_method_t(type p) noexcept : p(p) {}

    /**
     * @brief Invoke method
     *
     * @param args function arguments
     * @return utils::constexpr_invoke object of static_fn_t::return_type
     */
    template <class... Args2>
    constexpr decltype(auto) operator()(Args2&&... args) const noexcept (noexcept (metautils::constexpr_invoke(p, ::std::forward<Args2>(args)...))) {
        return metautils::constexpr_invoke(p, ::std::forward<Args2>(args)...);
    }
};

/**
 * @brief Create mem_fn_t
 *
 * @param pm pointer to member method
 * @return obj_method_t<R, T, Args...>
 */
template< class R, class T, class... Args  >
constexpr auto make_method(R T::* pm)
 -> obj_method_t<R, T, Args...> {
    return {pm};
}

/**
 * @brief Create mem_fn_t
 *
 * @param pm pointer to member method
 * @return obj_const_method_t<R, T, Args...>
 */
template< class R, class T, class... Args  >
constexpr auto make_const_method(R const T::* pm)
 -> obj_const_method_t<R, T, Args...> {
    return {pm};
}

/**
 * @brief Create mem_fn_t
 *
 * @param pm pointer to member method
 * @return static_method_t<R, T, Args...>
 */
template< class R, class... Args  >
constexpr auto make_method(R* pm)
 -> static_method_t<R, Args...> {
    return {pm};
}

}

}

#define REFLECT_OBJ_MET(NAME,...) \
    TUPLE_APPEND(names_state,counter,HANA_STR(#NAME)) \
    TUPLE_APPEND(metadata_state,counter,::reflect::info::make_method<decltype(::reflect::info::detail::MethodInfo<Type,##__VA_ARGS__>::return_type(&Type::NAME)) \
                 (__VA_ARGS__),Type,##__VA_ARGS__>(&Type::NAME)) \
    INCREASE_COUNTER(counter)

#define REFLECT_CONST_OBJ_MET(NAME,...) \
    TUPLE_APPEND(names_state,counter,HANA_STR(#NAME)) \
    TUPLE_APPEND(metadata_state,counter,::reflect::info::make_const_method<decltype(::reflect::info::detail::MethodInfo<Type,##__VA_ARGS__>::return_type(&Type::NAME)) \
                 (__VA_ARGS__) const,Type,##__VA_ARGS__>(&Type::NAME)) \
    INCREASE_COUNTER(counter)

#define REFLECT_STATIC_MET(NAME,...) \
    TUPLE_APPEND(names_state,counter,HANA_STR(#NAME)) \
    TUPLE_APPEND(metadata_state,counter,::reflect::info::make_method<decltype(::reflect::info::detail::MethodInfo<void,##__VA_ARGS__>::return_type(&Type::NAME)) \
                 (__VA_ARGS__),##__VA_ARGS__>(&Type::NAME)) \
    INCREASE_COUNTER(counter)

#endif // REFLECT_INFORMATION_METHOD_HPP
