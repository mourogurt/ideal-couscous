#ifndef REFLECT_INFORMATION_OBJECT_HPP
#define REFLECT_INFORMATION_OBJECT_HPP

#include "../../meta_utils/utils.hpp"

namespace reflect {

namespace info {

template <class T>
/**
 * @brief SFINAE check if type is pointer to object field
 *
 */
struct is_object
{
    template <class C> static constexpr ::std::true_type check(decltype(&C::is_object));
    template <class> static constexpr ::std::false_type check(...);
    static constexpr bool value = ::std::is_same<::std::true_type, decltype(check<T>(nullptr))>::value;
};

template <class T> constexpr bool is_object_v = is_object<T>::value; /**< Helper variable template for is_object */

template <class ParentGenerator, bool condition = true>
/**
 * @brief The ObjectIndexGenerator class - generate indices where object fields are located
 */
class ObjectIndexGenerator final {
    template <class Item, std::size_t Index>
    constexpr static decltype (auto) check_metadata_variable () {
        if constexpr (is_object_v<::std::decay_t<Item>> == condition) return ::boost::hana::make_tuple(::boost::hana::size_c<Index>);
        else return ::boost::hana::make_tuple();
    }

    template <class Tuple, ::std::size_t... Indices>
    constexpr static decltype (auto) generate_impl (const ::boost::hana::tuple<::boost::hana::size_t<Indices>...>&) {
        return metautils::multiple_concat(check_metadata_variable<decltype (::boost::hana::at_c<Indices>(::std::declval<Tuple>())),Indices>()...);
    }
public:
    using reverse = ObjectIndexGenerator<ParentGenerator,!condition>; /**< Reverse generator */

    template <class Tuple>
    /**
     * @brief generate function
     * @return ::boost::hana::tuple of indices
     */
    constexpr static decltype (auto) generate () {
        return generate_impl<Tuple>(ParentGenerator::template generate<Tuple>());
    }
};

}

}

#endif // REFLECT_INFORMATION_OBJECT_HPP
