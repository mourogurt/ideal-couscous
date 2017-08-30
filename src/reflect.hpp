#ifndef REFLECT_HPP
#define REFLECT_HPP

#include "meta_utils/meta_utils.hpp"
#include "reflect_information/reflect_information.hpp"
#include "reflect_utils/reflect_utils.hpp"

/**
 * @brief Reflection namespace
 * @todo add old examples
 * @todo serialization examples (console, json)
 * @todo performance benchmarks
 * @todo compile time benchmarks
 * @todo memorization
 * @todo Runtime reflection
 * @todo Reflect constructors
 * @todo Signals
 * @todo Slots
 * @todo Parameters
 * @todo Enums
 */
namespace reflect {

using All = info::DefaultIndexGenerator;             /**< All metainformation */
using AllVars = info::VariableIndexGenerator<All>;   /**< All vars */
using ObjVars = info::ObjectIndexGenerator<AllVars>; /**< Object vars */
using StaticVars = info::StaticIndexGenerator<AllVars>; /**< Static vars */
using AllMethods = info::MethodIndexGenerator<All>;     /**< All methods */
using ObjMethods =
    info::ObjectIndexGenerator<AllMethods>; /**< Object methods */
using ConstMethods =
    info::ConstMethodIndexGenerator<ObjMethods>; /**< Const methods */
using StaticMethods =
    info::StaticIndexGenerator<AllMethods>; /**< Static methods */
}

#endif // UTILS_HPP
