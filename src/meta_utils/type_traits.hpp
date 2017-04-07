#ifndef REFL_TYPE_TRAITS_HPP
#define REFL_TYPE_TRAITS_HPP

#include <boost/hana/optional.hpp>
#include <boost/hana/type.hpp>

namespace reflect {

namespace metautils {

/**
 * @brief Check if type is std::reference_wrapper
 *
 */
template <class T>
struct is_reference_wrapper : ::std::false_type {};

/**
 * @brief Check if type is std::reference_wrapper
 *
 */
template <class U>
struct is_reference_wrapper<::std::reference_wrapper<U>> : ::std::true_type {};

template <class T>
constexpr bool is_reference_wrapper_v = is_reference_wrapper<T>::value; /**< Helper variable template for is_reference_wrapper */

/**
 * @brief Get value of optional
 * @param optional_value - boost::hana::optional<...>
 * @return value of optional or nothing
 */
template<class T>
constexpr decltype (auto) get_optional_value (T&& optional_value) {
    if constexpr (!decltype(::boost::hana::is_nothing(optional_value))::value) return optional_value.value();
    else return ::boost::hana::nothing;
}

/**
 * @brief Helper struct to get type of class
 */
template <class T>
struct optional_type_helper {
    using type = typename T::type;
};

/**
 * @brief Template spetialization for boost::hana::nothing
 */
template<>
struct optional_type_helper<::boost::hana::optional<>> {
    using type = ::boost::hana::optional<>;
};

template<class T> using optional_type_helper_t = typename optional_type_helper<T>::type; /**< Helper type template for optional_type_helper */

}

}

#endif // TYPE_TRAITS_HPP
