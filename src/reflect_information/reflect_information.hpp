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
constexpr auto is_generator = ::boost::hana::is_valid(
    [](auto &&p, auto &&tup) -> decltype(
        &::std::decay_t<decltype(p)>::template generate<decltype(tup)>) {});

template <class T>
constexpr bool is_generator_v = decltype(is_generator(
    ::std::declval<T>(),
    ::std::declval<::boost::hana::tuple<>>()))::value; /**< Helper variable
                                                          template for
                                                          is_generator */

/**
 * @brief SFINAE check if class is reflected
 */
constexpr auto is_reflected = ::boost::hana::is_valid(
    [](auto &&p) -> decltype(&::std::decay_t<decltype(p)>::is_reflected) {});

template <class T>
constexpr bool is_reflected_v =
    decltype(is_reflected(::std::declval<T>()))::value; /**< Helper variable
                                                         template for
                                                         is_reflected */

namespace detail {

template <class MetaInfo_type> constexpr decltype(auto) names_tuple();

template <class MetaInfo_type> constexpr decltype(auto) metadata_tuple();

/**
 * @brief Get all metadata in inheritied classes recursivly if class have
 * parents (with index_sequence)
 */
template <class Tuple, ::std::size_t... indices>
constexpr decltype(auto)
get_class_subnames_impl(::std::index_sequence<indices...> &&) {
  return metautils::multiple_concat(
      names_tuple<typename ::std::decay_t<decltype(::boost::hana::at_c<indices>(
          std::declval<Tuple>()))>::MetaInfo_type>()...);
}

/**
 * @brief Get all names in inheritied classes recursivly if class have parents
 * (with index_sequence)
 */
template <class Tuple, ::std::size_t... indices>
constexpr decltype(auto)
get_class_submetadata_impl(::std::index_sequence<indices...> &&) {
  return metautils::multiple_concat(
      metadata_tuple<
          typename ::std::decay_t<decltype(::boost::hana::at_c<indices>(
              std::declval<Tuple>()))>::MetaInfo_type>()...);
}

/**
 * @brief Get all metadata in inheritied classes recursivly
 */
template <class Tuple> constexpr decltype(auto) get_class_subnames() {
  constexpr auto size =
      decltype(::boost::hana::size(::std::declval<Tuple>()))::value;
  if
    constexpr(size > 0) return get_class_subnames_impl<Tuple>(
        ::std::make_index_sequence<size>());
  else
    return ::boost::hana::make_tuple();
}

/**
 * @brief Get all names in inheritied classes recursivly
 */
template <class Tuple> constexpr decltype(auto) get_class_submetadata() {
  constexpr auto size =
      decltype(::boost::hana::size(::std::declval<Tuple>()))::value;
  if
    constexpr(size > 0) return get_class_submetadata_impl<Tuple>(
        ::std::make_index_sequence<size>());
  else
    return ::boost::hana::make_tuple();
}

/**
  * @brief Concating all names in one tuple
  */
template <class MetaInfo_type, long long... Indices>
constexpr decltype(auto)
names_tuple_impl(::std::integer_sequence<long long, Indices...> &&) {
  return metautils::multiple_concat(
      names_state(metautils::counter<Indices>{},
                  static_cast<const typename MetaInfo_type::Type *>(nullptr),
                  static_cast<const MetaInfo_type *>(nullptr))...,
      get_class_subnames<typename MetaInfo_type::Parent_types>());
}

/**
  * @brief Concating all metadata in one tuple
  */
template <class MetaInfo_type, long long... Indices>
constexpr decltype(auto)
metadata_tuple_impl(::std::integer_sequence<long long, Indices...> &&) {
  return metautils::multiple_concat(
      metadata_state(metautils::counter<Indices>{},
                     static_cast<const typename MetaInfo_type::Type *>(nullptr),
                     static_cast<const MetaInfo_type *>(nullptr))...,
      get_class_submetadata<typename MetaInfo_type::Parent_types>());
}

/**
  * @brief Concating all names in one tuple
  */
template <class MetaInfo_type> constexpr decltype(auto) names_tuple() {
  return names_tuple_impl<MetaInfo_type>(
      ::std::make_integer_sequence<
          long long, decltype(counter(metautils::counter<>{},
                                      static_cast<const MetaInfo_type *>(
                                          nullptr)))::value>{});
}

/**
  * @brief Concating all metadata in one tuple
  */
template <class MetaInfo_type> constexpr decltype(auto) metadata_tuple() {
  return metadata_tuple_impl<MetaInfo_type>(
      ::std::make_integer_sequence<
          long long, decltype(counter(metautils::counter<>{},
                                      static_cast<const MetaInfo_type *>(
                                          nullptr)))::value>{});
}
}

/**
 * @brief Class that stores meta-information about T
 */
template <class T> struct MetaClass {
  using Parent_types = typename T::Parent_types;
  static constexpr auto class_name{BOOST_HANA_STRING(class_name_detail(
      static_cast<const typename T::Type *>(nullptr),
      static_cast<const T *>(
          nullptr)))}; /**< compile-time string of class name */
  static constexpr auto names{
      detail::names_tuple<T>()}; /**< tuple of all variable names */
  static constexpr auto metadata{
      detail::metadata_tuple<T>()}; /**< tuple of all method names*/
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

/**
 *  @brief Declarating metainformation inside class. Basicly it creates
 * is_reflected function(to check if class is reflected), class_name_detail to
 * retrive class name c-t string, counter - c-t counter
 */
#define IN_METAINFO(TYPE, ...)                                                 \
  using Type = TYPE;                                                           \
  using MetaInfo_type = TYPE;                                                  \
  using Parent_types = ::boost::hana::tuple<__VA_ARGS__>;                      \
  static constexpr auto is_reflected() { return std::true_type(); }            \
  friend constexpr auto class_name_detail(const Type *, const MetaInfo_type *) \
      ->decltype(#TYPE) {                                                      \
    return #TYPE;                                                              \
  }                                                                            \
  friend constexpr ::reflect::metautils::counter<0> counter(                   \
      ::reflect::metautils::counter<0>, const MetaInfo_type *);

/**
 * @brief Declarating metainformation outside of class. All staff related to
 * reflection will be in reflect::info::MetaInfo<TYPE> except is_reflected
 * function
 */
#define OUT_METAINFO(TYPE)                                                     \
  friend struct reflect::info::MetaInfo<TYPE>;                                 \
  using MetaInfo_type = ::reflect::info::MetaInfo<TYPE>;                       \
  static constexpr auto is_reflected() { return std::true_type(); }

/**
 * @brief Declarating MetaInfo spetialization for TYPE. All related to
 * reflection staff goes here
 */
#define METAINFO(TYPE, ...)                                                    \
  namespace reflect {                                                          \
  namespace info {                                                             \
  template <> struct MetaInfo<TYPE> {                                          \
    using Type = TYPE;                                                         \
    using Parent_types = ::boost::hana::tuple<__VA_ARGS__>;                    \
    using MetaInfo_type = ::reflect::info::MetaInfo<Type>;                     \
    friend constexpr auto class_name_detail(const Type *,                      \
                                            const MetaInfo_type *)             \
        -> decltype(#TYPE) {                                                   \
      return #TYPE;                                                            \
    }                                                                          \
    friend constexpr ::reflect::metautils::counter<0>                          \
        counter(::reflect::metautils::counter<0>, const MetaInfo_type *);

/**
 * @brief Declarating MetaInfo spetialization for template class TYPE<TEMPLATE>.
 * First param is template class without template, second - all types that will
 * be needed to spetialize templat, third - template args for TYPE.
 */
#define TEMPLATE_METAINFO(TYPE, TEMPLATE_TYPE, TEMPLATE, ...)                  \
  namespace reflect {                                                          \
  namespace info {                                                             \
  template <TEMPLATE_TYPE> struct MetaInfo<TYPE<TEMPLATE>> {                   \
    using Type = TYPE<TEMPLATE>;                                               \
    using Parent_types = ::boost::hana::tuple<__VA_ARGS__>;                    \
    using MetaInfo_type = ::reflect::info::MetaInfo<Type>;                     \
    friend constexpr auto class_name_detail(const Type *,                      \
                                            const MetaInfo_type *)             \
        -> decltype(#TYPE) {                                                   \
      return #TYPE;                                                            \
    }                                                                          \
    friend constexpr ::reflect::metautils::counter<0>                          \
        counter(::reflect::metautils::counter<0>, const MetaInfo_type *);

/**
 * @brief Needed only to end metainformation outside class (OUT_METAINFO)
 */
#define END_METAINFO                                                           \
  }                                                                            \
  ;                                                                            \
  }                                                                            \
  }

/**
 * @brief Appending element to STATE. Element index will be same as current
 * COUNTER. Creates function that will return boost::hana::tuple of element for
 * current counter value
 */
#define TUPLE_APPEND(STATE, COUNTER, ...)                                      \
  friend constexpr auto STATE(                                                 \
      ::reflect::metautils::counter<decltype(                                  \
          COUNTER(::reflect::metautils::counter<>{},                           \
                  static_cast<const MetaInfo_type *>(nullptr)))::value>,       \
      const Type *, const MetaInfo_type *)                                     \
      ->decltype(::boost::hana::make_tuple(__VA_ARGS__)) {                     \
    return ::boost::hana::make_tuple(__VA_ARGS__);                             \
  }

/**
 * @brief Increasing COUNTER by one. Create COUNTER function that will hold
 * current counter value + 1
 */
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
