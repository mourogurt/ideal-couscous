#ifndef REFL_INFORMATION_HPP
#define REFL_INFORMATION_HPP

#include "../meta_utils/meta_utils.hpp"
#include "functions/reflect_information_method.hpp"
#include "variables/reflect_information_variable.hpp"

namespace reflect {

/**
 * @brief Namespace related to all reflection information(metadata, names, etc)
 */
namespace info {

/**
 * @brief SFINAE check if class is generator
 */
template <class T> class is_generator {
  template <class C, long long... Indices>

  static constexpr ::std::true_type
  check(decltype(&C::template generate<
                 ::boost::hana::tuple<::boost::hana::llong<Indices>...>>));
  template <class> static constexpr ::std::false_type check(...);

public:
  static constexpr bool value =
      ::std::is_same<::std::true_type, decltype(check<T>(nullptr))>::value;
};

template <class T> constexpr bool is_generator_v = is_generator<T>::value;

/**
 * @brief SFINAE check if class is reflected
 *
 */
template <class T> class is_reflected {

  template <class C>
  static constexpr ::std::true_type check(decltype(&C::is_reflected));
  template <class> static constexpr ::std::false_type check(...);

public:
  static constexpr bool value =
      ::std::is_same<::std::true_type, decltype(check<T>(nullptr))>::
          value; /**< true true if reflected, othervise false */
};

template <class T>
constexpr bool is_reflected_v =
    is_reflected<T>::value; /**< Helper variable template for is_reflected */

namespace detail {

/**
  * @brief Concating all names in one tuple
  */
template <class Type, class MetaInfo_type, long long... Indices>
constexpr decltype(auto)
names_tuple(::std::integer_sequence<long long, Indices...> &&) {
  return metautils::multiple_concat(names_state(
      metautils::counter<Indices>{}, static_cast<const Type *>(nullptr),
      static_cast<const MetaInfo_type *>(nullptr))...);
}

/**
  * @brief Concating all metadata in one tuple
  */
template <class Type, class MetaInfo_type, long long... Indices>
constexpr decltype(auto)
metadata_tuple(::std::integer_sequence<long long, Indices...> &&) {
  return metautils::multiple_concat(metadata_state(
      metautils::counter<Indices>{}, static_cast<const Type *>(nullptr),
      static_cast<const MetaInfo_type *>(nullptr))...);
}
}

/**
 * @brief Class that stores meta-information about T
 *
 */
template <class T> struct MetaClass {
  using Type = typename T::Type;
  using MetaInfo_type = T;
  static constexpr auto class_name{BOOST_HANA_STRING(class_name_detail(
      static_cast<const typename T::Type *>(nullptr),
      static_cast<const T *>(
          nullptr)))}; /**< compile-time string of class name */
  static constexpr auto names{detail::names_tuple<Type, MetaInfo_type>(
      ::std::make_integer_sequence<
          long long,
          decltype(counter(metautils::counter<>{},
                           static_cast<const MetaInfo_type *>(
                               nullptr)))::value>{})}; /**< tuple of all
                                                          variable names */
  static constexpr auto metadata{detail::metadata_tuple<Type, MetaInfo_type>(
      ::std::make_integer_sequence<
          long long, decltype(
                         counter(metautils::counter<>{},
                                 static_cast<const MetaInfo_type *>(
                                     nullptr)))::value>{})}; /**< tuple of all
                                                                method names  */
};

class EmptyGenerator;

/**
 * @brief The DefaultIndexGenerator class - generate tuple of indices [0..N-1]
 * where N - tuple size
 */
class DefaultIndexGenerator final {
public:
  using reverse = EmptyGenerator;

  /**
    * @brief Generate tuple of indices
    * @return  ::boost::hana::tuple<0...N-1>
    */
  template <class Tuple> constexpr static decltype(auto) generate() {
    return metautils::gen_inds_tup<decltype(
        ::boost::hana::size(::std::declval<Tuple>()))>();
  }
};

/**
 * @brief Empty generator
 */
class EmptyGenerator final {
public:
  using reverse = DefaultIndexGenerator;

  template <class Tuple>
  /**
   * @brief Generate empty tuple
   * @return ::boost::hana::tuple<>
   */
  constexpr static ::boost::hana::tuple<> generate() {
    return {};
  }
};

/**
 * @brief Class that stores meta-functions
 */
template <class... Args> struct MetaInfo;
}
}

#define IN_METAINFO(TYPE)                                                      \
  using Type = TYPE;                                                           \
  using MetaInfo_type = TYPE;                                                  \
  static constexpr auto is_reflected() { return std::true_type(); }            \
  friend constexpr auto class_name_detail(const Type *, const MetaInfo_type *) \
      ->decltype(#TYPE) {                                                      \
    return #TYPE;                                                              \
  }                                                                            \
  friend constexpr ::reflect::metautils::counter<0> counter(                   \
      ::reflect::metautils::counter<0>, const MetaInfo_type *);

#define OUT_METAINFO(TYPE)                                                     \
  friend struct reflect::info::MetaInfo<TYPE>;                                 \
  using MetaInfo_type = ::reflect::info::MetaInfo<TYPE>;                       \
  static constexpr auto is_reflected() { return std::true_type(); }

#define METAINFO(TYPE)                                                         \
  namespace reflect {                                                          \
  namespace info {                                                             \
  template <> struct MetaInfo<TYPE> {                                          \
    using Type = TYPE;                                                         \
    using MetaInfo_type = ::reflect::info::MetaInfo<Type>;                     \
    friend constexpr auto class_name_detail(const Type *,                      \
                                            const MetaInfo_type *)             \
        -> decltype(#TYPE) {                                                   \
      return #TYPE;                                                            \
    }                                                                          \
    friend constexpr ::reflect::metautils::counter<0>                          \
        counter(::reflect::metautils::counter<0>, const MetaInfo_type *);

#define TEMPLATE_METAINFO(TYPE, TEMPLATE_TYPE, TEMPLATE)                       \
  namespace reflect {                                                          \
  namespace info {                                                             \
  template <TEMPLATE_TYPE> struct MetaInfo<TYPE<TEMPLATE>> {                   \
    using Type = TYPE<TEMPLATE>;                                               \
    using MetaInfo_type = ::reflect::info::MetaInfo<Type>;                     \
    friend constexpr auto class_name_detail(const Type *,                      \
                                            const MetaInfo_type *)             \
        -> decltype(#TYPE) {                                                   \
      return #TYPE;                                                            \
    }                                                                          \
    friend constexpr ::reflect::metautils::counter<0>                          \
        counter(::reflect::metautils::counter<0>, const MetaInfo_type *);

#define END_METAINFO                                                           \
  }                                                                            \
  ;                                                                            \
  }                                                                            \
  }

#define TUPLE_APPEND(STATE, COUNTER, ...)                                      \
  friend constexpr auto STATE(                                                 \
      ::reflect::metautils::counter<decltype(                                  \
          COUNTER(::reflect::metautils::counter<>{},                           \
                  static_cast<const MetaInfo_type *>(nullptr)))::value>,       \
      const Type *, const MetaInfo_type *)                                     \
      ->decltype(::boost::hana::make_tuple(__VA_ARGS__)) {                     \
    return ::boost::hana::make_tuple(__VA_ARGS__);                             \
  }

#define INCREASE_COUNTER(COUNTER)                                              \
  friend constexpr ::reflect::metautils::counter<                              \
      decltype(COUNTER(::reflect::metautils::counter<>{},                      \
                       static_cast<const MetaInfo_type *>(nullptr)))::value +  \
      1u>                                                                      \
  COUNTER(::reflect::metautils::counter<                                       \
              decltype(COUNTER(                                                \
                  ::reflect::metautils::counter<>{},                           \
                  static_cast<const MetaInfo_type *>(nullptr)))::value +       \
              1u>,                                                             \
          const MetaInfo_type *);

#endif // META_INFORMATION_HPP
