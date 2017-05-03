#ifndef REFLECT_HPP
#define REFLECT_HPP

#include "meta_utils/meta_utils.hpp"
#include "reflect_information/reflect_information.hpp"
#include "reflect_utils/reflect_utils.hpp"

/**
 * @brief Reflection namespace
 * @todo move from tuple of indices to ranges
 * @todo serialization examples (console, json)
 * @todo performance benchmarks
 * @todo compile time benchmarks
 * @todo c++14 port
 * @todo Variable/method flags (public, private, protected)
 * @todo Runtime reflection over static
 * @todo Reflect constructors
 * @todo Signals
 * @todo Slots
 * @todo Parameters
 * @todo Enums
 * @todo Inheritance
 * @todo Polymorphism
 */
namespace reflect {

using All = info::DefaultIndexGenerator; /**< All metainformation */
using AllVars = info::VariableIndexGenerator<All>; /**< All vars */
using ObjVars = info::ObjectIndexGenerator<AllVars>; /**< Object vars */
using StaticVars = info::StaticIndexGenerator<AllVars>; /**< Static vars */
using AllMethods = info::MethodIndexGenerator<All>; /**< All methods */
using ObjMethods = info::ObjectIndexGenerator<AllMethods>; /**< Object methods */
using ConstMethods = info::ConstMethodIndexGenerator<ObjMethods>; /**< Const methods */
using StaticMethods = info::StaticIndexGenerator<AllMethods>; /**< Static methods */

/**
 * @brief Helper struct for getting method types
 */
template<class T, ::std::size_t I, class Generator = info::DefaultIndexGenerator>
struct MethodInfo {
private:
    using MetaClass = info::MetaClass<typename ::std::decay_t<typename T::MetaInfo_type>>; /**< Helper type template to specify Metadata class */
public:
    using types = typename ::std::decay_t<decltype(utils::detail::method_args_helper_method_impl<T,Generator>(::boost::hana::size_c<I>))>::type; /**< boost::hana::tuple<...> */
    using result_type = metautils::optional_type_helper_t<std::decay_t<decltype(metautils::get_opt_val(utils::result_type<T,Generator>(::boost::hana::size_c<I>)))>>; /**< result type of method */
    template<::std::size_t N> using arg_type = metautils::optional_type_helper_t<::std::decay_t<decltype(metautils::get_opt_val(utils::get_method_arg<T,Generator>(::boost::hana::size_c<I>,::boost::hana::size_c<N>)))>>; /**< N-th type */
    using arg0_type = arg_type<0>;
    using arg1_type = arg_type<1>;
    using arg2_type = arg_type<2>;
    using arg3_type = arg_type<3>;
    using arg4_type = arg_type<4>;
    using arg5_type = arg_type<5>;
    using arg6_type = arg_type<6>;

    constexpr static auto args_count {metautils::get_opt_val(utils::methods_args_count<T,Generator>(::boost::hana::size_c<I>))}; /**< args count */
};

}

#endif // UTILS_HPP
