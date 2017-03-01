#ifndef REFLECT_INFORMATION_VARIABLE_HPP
#define REFLECT_INFORMATION_VARIABLE_HPP

#include "../../meta_utils/utils.hpp"

namespace reflect {

namespace info {

template <class T>
/**
 * @brief SFINAE check if type is pointer to variable
 *
 */
struct is_variable
{
    template <class C> static constexpr ::std::true_type check(decltype(&C::is_variable));
    template <class> static constexpr ::std::false_type check(...);
    static constexpr bool value = ::std::is_same<::std::true_type, decltype(check<T>(nullptr))>::value;
};

template <class T> constexpr bool is_variable_v = is_variable<T>::value; /**< Helper variable template for is_variable */

namespace detail {

}

class ObjVariableIndexGenerator final {

    template <class Item, std::size_t Index>
    constexpr static decltype (auto) check_metadata_variable () {
        if constexpr (is_variable_v<::std::decay_t<Item>>) return ::boost::hana::make_tuple(::boost::hana::size_c<Index>);
        else return ::boost::hana::make_tuple();
    }

    template <class Tuple, ::std::size_t... Indices>
    constexpr static decltype (auto) generate_impl (::std::index_sequence<Indices...>&&) {
        return metautils::multiple_concat(check_metadata_variable<decltype (::boost::hana::at_c<Indices>(::std::declval<Tuple>())),Indices>()...);
    }

public:

    template <class Tuple>
    constexpr static decltype (auto) generate () {
        constexpr std::size_t N = ::std::decay_t<decltype(::boost::hana::size(::std::declval<Tuple>()))>::value;
        return generate_impl<Tuple>(::std::make_index_sequence<N>());
    }

};

template< class Result, class Obj>
/**
 * @brief Pointer to object variable container
 *
 */
class obj_var_t final {
    Result Obj::* p; /**< Pointer to object variable */
public:
    /**
     * @brief Object type of pointer
     *
     */
    using obj_type = Obj;
    /**
     * @brief Pointer type
     *
     */
    using type = Result Obj::*;
    /**
     * @brief Return type
     *
     */
    using return_type = Result;
    /**
     * @brief method that indicates that the pointer - variable
     *
     * @return std::true_type
     */
    static constexpr auto is_variable () {return std::true_type();}
    /**
     * @brief Constexpr constructor
     *
     * @param p object pointer
     */
    constexpr obj_var_t(type p) noexcept: p(p) {}
    /**
     * @brief Invoke operator
     *
     * @param obj member object
     * @return utils::constexpr_invoke object of obj_var_t::return_type
     */
    constexpr decltype(auto) operator()(Obj&& obj) const noexcept (noexcept (metautils::constexpr_invoke(p, obj))) {
        return metautils::constexpr_invoke(p, obj);
    }
    /**
     * @brief Invoke operator
     *
     * @param obj member object
     * @return utils::constexpr_invoke object of obj_var_t::return_type
     */
    constexpr decltype(auto) operator()(const Obj& obj) const noexcept (noexcept (metautils::constexpr_invoke(p, obj))) {
        return metautils::constexpr_invoke(p, obj);
    }
    /**
     * @brief Invoke operator
     *
     * @param obj member object
     * @return utils::constexpr_invoke object of obj_var_t::return_type
     */
    constexpr decltype(auto) operator()(Obj& obj) const noexcept (noexcept (metautils::constexpr_invoke(p, obj))) {
        return metautils::constexpr_invoke(p, obj);
    }
};

template< class R, class T  >
/**
 * @brief Create obj_var_t
 *
 * @param pm pointer to object variable
 * @return mem_fn_t<R, T>
 */
constexpr auto make_obj_var(R T::* pm)
 -> obj_var_t<R, T> {
    return {pm};
}

}

}

#define REFLECT_OBJ_VARIABLE(NAME) \
    TUPLE_APPEND(names_state,counter,HANA_STR(#NAME)) \
    TUPLE_APPEND(metadata_state,counter,::reflect::info::make_obj_var(&Type::NAME))\
    INCREASE_COUNTER(counter)

#endif // REFLECT_INFORMATION_VARIABLE_HPP
