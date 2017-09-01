#ifndef REFLECT_INVOKES_HPP
#define REFLECT_INVOKES_HPP

#include "../meta_utils/meta_utils.hpp"
#include "../reflect_information/reflect_information.hpp"
#include "reflect_metadata.hpp"
#include <boost/hana/equal.hpp>

namespace reflect {

namespace utils {

template <class T>
using MetaClass = info::MetaClass<typename ::std::decay_t<
    T>::MetaInfo_type>; /**< Helper type template to specify Metadata class */

template <class T, class Generator = info::DefaultIndexGenerator, class I,
          class... Args>
constexpr decltype(auto) get(I &&, Args &&...);

template <class T, class Generator, class I, class SetArg, class... Args>
constexpr decltype(auto) set(I &&, SetArg &&, Args &&...);

namespace detail {

/**
 * @brief  Implementation of invoke function
 * @param p - metadata of element
 * @param args - Arguments of invocation
 * @return T::return_type
 */
template <class T, class... Args>
constexpr decltype(auto) invoke_impl(T &&p, Args &&... args) {
  if
    constexpr(::std::decay_t<decltype(::boost::hana::size(
                  ::std::declval<typename T::arg_types>()))>::value !=
              0) return p(::std::forward<Args>(args)...);
  else
    return p();
}
}

/**
 * @brief invoke a method/variable
 * @param index - index of element to invoke (boost::hana::llong)
 * @param args - Invocation arguments
 * @return object which type is type of variable or result type of
 * method(bost::hana::type_c<void> if result type is void) or
 * boost::hana::nothing if error occurs
 */
template <class T, class Generator, class I, class... Args>
constexpr decltype(auto) invoke(I &&index, Args &&... args) {
#ifndef COUSOUS_DISABLE_MOST_CT_CHECKS
  static_assert(decltype(check_reflected<T>())::value,
                "Class must be reflected");
  static_assert(info::is_generator_v<::std::decay_t<Generator>>,
                "Generator is not a generator class");
  static_assert(
      ::std::is_same_v<::boost::hana::integral_constant_tag<long long>,
                       ::boost::hana::tag_of_t<I>>,
      "Index must be an integral constant of long long");
#endif
  return detail::invoke_impl(
      ::boost::hana::at(
          metautils::copy_tuple_sequence(
              MetaClass<T>::metadata,
              Generator::template generate<decltype(MetaClass<T>::metadata)>()),
          index),
      ::std::forward<Args>(args)...);
}
}
}

#endif // REFLECT_INVOKES_HPP
