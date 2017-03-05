#ifndef REFLECT_INFORMATION_STATIC_COMMON_HPP
#define REFLECT_INFORMATION_STATIC_COMMON_HPP

#include "../../meta_utils/utils.hpp"

namespace reflect {

namespace info {

template <class T>
/**
 * @brief SFINAE check if type is pointer to static
 *
 */
struct is_static
{
    template <class C> static constexpr ::std::true_type check(decltype(&C::is_static));
    template <class> static constexpr ::std::false_type check(...);
    static constexpr bool value = ::std::is_same<::std::true_type, decltype(check<T>(nullptr))>::value;
};

template <class T> constexpr bool is_static_v = is_static<T>::value; /**< Helper variable template for is_static */

template <class ParentGenerator, bool condition = true>

class StaticIndexGenerator final {
    template <class Item, std::size_t Index>
    constexpr static decltype (auto) check_metadata_variable () {
        if constexpr (is_static_v<::std::decay_t<Item>> == condition) return ::boost::hana::make_tuple(::boost::hana::size_c<Index>);
        else return ::boost::hana::make_tuple();
    }

    template <class Tuple, ::std::size_t... Indices>
    constexpr static decltype (auto) generate_impl (const ::boost::hana::tuple<::boost::hana::size_t<Indices>...>&) {
        return metautils::multiple_concat(check_metadata_variable<decltype (::boost::hana::at_c<Indices>(::std::declval<Tuple>())),Indices>()...);
    }
public:
    using reverse = StaticIndexGenerator<ParentGenerator,!condition>;

    template <class Tuple>
    constexpr static decltype (auto) generate () {
        return generate_impl<Tuple>(ParentGenerator::template generate<Tuple>());
    }
};

}

}

#endif // REFLECT_INFORMATION_STATIC_COMMON_HPP
