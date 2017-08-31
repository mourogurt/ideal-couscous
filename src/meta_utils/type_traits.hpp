#ifndef REFL_TYPE_TRAITS_HPP
#define REFL_TYPE_TRAITS_HPP

#include <boost/hana/type.hpp>

namespace reflect {

namespace metautils {

namespace detail {

namespace optional_type_helper_impl {

/**
 * @brief Default condition (typename type doesn't contains in class)
 */
template <class T, bool Cond> struct checker { using type = T; };

/**
 * @brief Spetialization if typename type contains in class
 */
template <class T> struct checker<T, true> { using type = typename T::type; };
}
}

template <class T> struct Type { using type = T; };

/**
 * @brief Check if type is std::reference_wrapper
 *
 */
template <class T> struct is_reference_wrapper : ::std::false_type {};

/**
 * @brief Check if type is std::reference_wrapper
 *
 */
template <class U>
struct is_reference_wrapper<::std::reference_wrapper<U>> : ::std::true_type {};

template <class T>
constexpr bool is_reference_wrapper_v =
    is_reference_wrapper<T>::value; /**< Helper variable template for
                                       is_reference_wrapper */

/**
 * @brief Helper struct to unpack type typename from class if it exists
 */
template <class T> class unpack_type_typename_helper {
  constexpr static auto hasType =
      ::boost::hana::is_valid([](auto x) -> ::boost::hana::type<
                                  typename decltype(x)::type>{});

public:
  using type = typename detail::optional_type_helper_impl::checker<
      T, decltype(hasType(std::declval<T>()))::value>::type;
};

template <class T>
using unpack_type_typename_helper_t =
    typename unpack_type_typename_helper<T>::type; /**<
                                   Helper type template for
                                   unpack_type_typename_helper */
}
}

#endif // TYPE_TRAITS_HPP
