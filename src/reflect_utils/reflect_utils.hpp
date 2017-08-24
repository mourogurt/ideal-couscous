#ifndef REFLECT_UTILS_HPP
#define REFLECT_UTILS_HPP

#include "reflect_invokes.hpp"
#include "reflect_metadata.hpp"

namespace reflect {
/**
 * @brief Namespace related to all reflection utils
 */
namespace utils {

/**
 * @brief Helper struct for getting method types
 */
template <class T, long long I, class Generator = info::DefaultIndexGenerator>
struct MethodInfo {
private:
  using MetaClass = info::MetaClass<typename ::std::decay_t<
      T>::MetaInfo_type>; /**< Helper type template to specify Metadata class */
public:
  using types = typename ::std::decay_t<decltype(
      utils::detail::method_args_helper_method_impl<T, Generator>(
          ::boost::hana::llong_c<I>))>::type; /**< boost::hana::tuple<...> */
  using result_type = metautils::unpack_type_typename_helper_t<
      std::decay_t<decltype(utils::result_type<T, Generator>(
          ::boost::hana::llong_c<I>))>>; /**< result type of method */
  template <long long N>
  using arg_type = metautils::unpack_type_typename_helper_t<
      ::std::decay_t<decltype(utils::method_arg<T, Generator>(
          ::boost::hana::llong_c<I>, ::boost::hana::llong_c<N>))>>; /**< N-th
                                                                      type */

  constexpr static auto args_count{utils::methods_args_count<T, Generator>(
      ::boost::hana::llong_c<I>)}; /**< args count */
};
}
}

#endif // REFLECT_UTILS_HPP
