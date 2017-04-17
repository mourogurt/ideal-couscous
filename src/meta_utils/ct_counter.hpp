#ifndef REFL_CT_COUNTER_HPP
#define REFL_CT_COUNTER_HPP

#include <cstddef>

namespace reflect {

namespace metautils {

/**
 * @brief Compile-time counter
 *
 */
template<::std::size_t N = 255> struct counter : public counter<N - 1> {
    static constexpr ::std::size_t value = N; /**< value of counter */
};
/**
 * @brief Zero counter
 *
 */
template<> struct counter<0> { static constexpr ::std::size_t value = 0; };

}

}

#endif // CT_COUNTER_HPP
