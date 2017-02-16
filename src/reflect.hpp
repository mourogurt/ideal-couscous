#ifndef META_UTILS_HPP
#define META_UTILS_HPP
#include "reflect_metadata.hpp"
#include "reflect_invokes.hpp"

/**
 * @brief todo list for reflection
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
//Some helpers that depends on invokes and metadata go there
template< class T, ::std::size_t I>
using variable_type_t = decltype (get_variable<I>(::std::declval<T>()));
template< class T, class I>
using variable_type_index_t = decltype (get_variable(::std::declval<T>(),::std::declval<I>()));
}

#endif // UTILS_HPP
