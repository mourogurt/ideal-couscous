#ifndef META_UTILS_HPP
#define META_UTILS_HPP

#include "meta_utils/meta_utils.hpp"
#include "reflect_information/reflect_information.hpp"
#include "reflect_utils/reflect_utils.hpp"

/**
 * @brief Reflection namespace
 * @todo serialization examples (console, json)
 * @todo performance benchmarks
 * @todo compile time benchmarks
 * @todo c++14 port
 * @todo document detail namespace
 * @todo Variable/method flags (public, private, protected)
 * @todo Runtime reflection over static
 * @todo Reflect constructors
 * @todo Signals
 * @todo Slots
 * @todo Move macroses out of class definition
 * @todo Parameters
 * @todo Enums
 * @todo Inheritance
 * @todo Polymorphism
 */
namespace reflect {
using All = info::DefaultIndexGenerator;
using AllVars = info::VariableIndexGenerator<All>;
using ObjVars = info::ObjectIndexGenerator<AllVars>;
using StaticVars = info::StaticIndexGenerator<AllVars>;
using AllMethods = info::MethodIndexGenerator<All>;
using ObjMethods = info::ObjectIndexGenerator<AllMethods>;
using ConstMethods = info::ConstMethodIndexGenerator<ObjMethods>;
using StaticMethods = info::StaticIndexGenerator<AllMethods>;
}

#endif // UTILS_HPP
