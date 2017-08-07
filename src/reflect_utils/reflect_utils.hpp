#ifndef REFLECT_UTILS_HPP
#define REFLECT_UTILS_HPP

#ifndef COUSOUS_DISABLE_MOST_CT_CHECKS
#include "opt/reflect_invokes.hpp"
#include "opt/reflect_metadata.hpp"
#else
#include "noopt/reflect_invokes.hpp"
#include "noopt/reflect_metadata.hpp"
#endif

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
  using result_type =
      metautils::unpack_type_typename_helper_t<std::decay_t<decltype(
          metautils::get_opt_val(utils::result_type<T, Generator>(
              ::boost::hana::llong_c<I>)))>>; /**< result type of method */
  template <long long N>
  using arg_type = metautils::unpack_type_typename_helper_t<::std::decay_t<
      decltype(metautils::get_opt_val(utils::method_arg<T, Generator>(
          ::boost::hana::llong_c<I>, ::boost::hana::llong_c<N>)))>>; /**< N-th
                                                                      type */
  using arg0_type = arg_type<0>;
  using arg1_type = arg_type<1>;
  using arg2_type = arg_type<2>;
  using arg3_type = arg_type<3>;
  using arg4_type = arg_type<4>;
  using arg5_type = arg_type<5>;
  using arg6_type = arg_type<6>;

  constexpr static auto args_count{
      metautils::get_opt_val(utils::methods_args_count<T, Generator>(
          ::boost::hana::llong_c<I>))}; /**< args count */
};
}
}

#endif // REFLECT_UTILS_HPP
