#ifndef REFLECT_METADDATA_HPP
#define REFLECT_METADDATA_HPP

#include <boost/hana/greater.hpp>
#include <boost/hana/greater_equal.hpp>

#include "../meta_utils/meta_utils.hpp"
#include "../reflect_information/reflect_information.hpp"

namespace reflect {

namespace utils {

template <class T>
using MetaClass = info::MetaClass<typename ::std::decay_t<
    T>::MetaInfo_type>; /**< Helper type template to specify Metadata class */

template <class T> using Parents = typename MetaClass<T>::Parent_types;

template <class T, class Generator = info::DefaultIndexGenerator, class... Args,
          class I>
constexpr decltype(auto) check_invoke(I &&);

template <class T> constexpr decltype(auto) parents_count();

template <class T> constexpr decltype(auto) parents_types();

namespace detail {

/**
 * @brief Helper struct to get tuple of types(boost::hana::tuple_t)
 */
template <class... Args> struct method_args_helper_impl {
  constexpr static auto value{::boost::hana::tuple_t<Args...>};
};

/**
 * @brief Helper struct to get tuple of types(boost::hana::tuple_t) (template
 * spetialization for boost::hana::tuple)
 */
template <class... Args>
struct method_args_helper_impl<::boost::hana::tuple<Args...>> {
  constexpr static auto value{::boost::hana::tuple_t<Args...>};
};

/**
 * @brief Helper struct to get tuple of types of all
 * parents(boost::hana::tuple_t)
 */
template <class... Args> struct parents_types_helper_impl {
  constexpr static auto value{metautils::multiple_concat(
      ::boost::hana::tuple_t<Args...>, parents_types<Args>()...)};
};

/**
 * @brief Helper struct to get tuple of types of all
 * parents(boost::hana::tuple_t) (template
 * spetialization for boost::hana::tuple)
 */
template <class... Args>
struct parents_types_helper_impl<::boost::hana::tuple<Args...>> {
  constexpr static auto value{metautils::multiple_concat(
      ::boost::hana::tuple_t<Args...>, parents_types<Args>()...)};
};

/**
 * @brief Counting all parents
 */
template <class... Args>
constexpr decltype(auto) parents_count_unpack(::boost::hana::tuple<Args...>) {
  return (parents_count<Args>() + ...);
}

/**
 * @brief Return tuple of args type of method
 * @param index - method index
 * @return boost::hana::type_t<Tuple...>
 */
template <class T, class Generator, class I>
constexpr decltype(auto) method_args_helper_method_impl(I &&index) {
#ifndef COUSOUS_DISABLE_MOST_CT_CHECKS
  static_assert(info::is_reflected_v<::std::decay_t<T>>,
                "Class must be reflected");
  static_assert(info::is_generator_v<::std::decay_t<Generator>>,
                "Generator is not a generator class");
  static_assert(::std::decay_t<decltype(
                        ::boost::hana::size(metautils::copy_tuple_sequence(
                            MetaClass<T>::metadata,
                            Generator::template generate<decltype(
                                MetaClass<T>::metadata)>())))>::value >
                    ::std::decay_t<I>::value,
                "Index must be less than number of elements in the generator");
#endif
  return ::boost::hana::type_c<
      typename ::std::decay_t<decltype(::boost::hana::at(
          metautils::copy_tuple_sequence(
              MetaClass<T>::metadata,
              Generator::template generate<decltype(MetaClass<T>::metadata)>()),
          index))>::arg_types>;
}

/**
 * @brief SFINAE check whether we can call operator () with given types
 */
template <class T, class... Args> struct is_invocable {
  template <class C>
  static constexpr ::std::true_type
  check(::std::decay_t<decltype(::std::declval<C>()(::std::declval<Args>()...))>
            *);
  template <class> static constexpr ::std::false_type check(...);
  static constexpr bool value =
      ::std::is_same<::std::true_type, decltype(check<T>(nullptr))>::value;
};

template <class... Args>
constexpr bool is_invocable_v = is_invocable<Args...>::value; /**< Helper
                                                                 variable
                                                                 template for
                                                                 is_invocable */

/**
 * @brief Struct checker if return value of invocation is assignable
 */
template <class T, class SetArg, class... Args> struct is_invoke_assignable {
  static constexpr bool value = ::std::is_assignable<
      decltype(::std::declval<T>()(::std::declval<Args>()...)), SetArg>::value;
};

template <class... Args>
constexpr bool is_invoke_assignable_v =
    is_invoke_assignable<Args...>::value; /**< Helper variable template for
                                             is_invoke_assignable */

/**
 * @brief Implementation of check_invoke
 * @return boost::hana::bool_c<true/false>
 */
template <class... Args> constexpr decltype(auto) check_invoke_impl() {
  if
    constexpr(is_invocable_v<Args...>) return ::boost::hana::bool_c<true>;
  else
    return ::boost::hana::bool_c<false>;
}

/**
 * @brief Implementation of check_set
 * @return boost::hana::bool_c<true/false>
 */
template <class T, class SetArg, class... Args>
constexpr decltype(auto) check_set_impl() {
  if
    constexpr(is_invocable_v<T, Args...>) {
      if
        constexpr(
            is_invoke_assignable_v<T, SetArg,
                                   Args...>) return ::boost::hana::bool_c<true>;
      else
        return ::boost::hana::bool_c<false>;
    }
  else
    return ::boost::hana::bool_c<false>;
}
}

/**
 * @brief Check if class is reflected
 * @return boost::hana::bool_c<true/false>
 */
template <class T> constexpr decltype(auto) check_reflected() {
  return ::boost::hana::bool_c<info::is_reflected_v<::std::decay_t<T>>>;
}

/**
 * @brief Returns class name
 * @return ct-string
 */
template <class T> constexpr decltype(auto) class_name() {
#ifndef COUSOUS_DISABLE_MOST_CT_CHECKS
  static_assert(decltype(check_reflected<T>())::value,
                "Class must be reflected");
#endif
  return MetaClass<T>::class_name;
}

/**
 * @brief Returns amount of parent classes
 * @return boost::hana::llong<...>
 */
template <class T> constexpr decltype(auto) parents_count() {
#ifndef COUSOUS_DISABLE_MOST_CT_CHECKS
  static_assert(decltype(check_reflected<T>())::value,
                "Class must be reflected");
#endif
  constexpr auto top_parents_count =
      decltype(::boost::hana::size(::std::declval<Parents<T>>()))::value;
  if
    constexpr(top_parents_count >
              0) return ::boost::hana::llong_c<top_parents_count> +
        ::boost::hana::llong_c<decltype(
            detail::parents_count_unpack(::std::declval<Parents<T>>()))::value>;
  else
    return ::boost::hana::llong_c<0>;
}

/**
 * @brief Return all parent types
 * @return boost::hana::tuple_t<...>
 */
template <class T> constexpr decltype(auto) parents_types() {
#ifndef COUSOUS_DISABLE_MOST_CT_CHECKS
  static_assert(decltype(check_reflected<T>())::value,
                "Class must be reflected");
#endif
  if
    constexpr(parents_count<T>() > 0) return decltype(
        detail::parents_types_helper_impl<Parents<T>>())::value;
  else
    return ::boost::hana::tuple_t<>;
}

/**
 * @brief Return parent type specified by index
 * @param index - index(boost::hana::llong)
 * @return boost::hana::type_c<...>
 */
template <class T, class I> constexpr decltype(auto) parent_type(I &&index) {
#ifndef COUSOUS_DISABLE_MOST_CT_CHECKS
  static_assert(decltype(check_reflected<T>())::value,
                "Class must be reflected");
  static_assert(
      ::std::is_same_v<::boost::hana::integral_constant_tag<long long>,
                       ::boost::hana::tag_of_t<I>>,
      "Index must be an integral constant of long long");
  static_assert(decltype(parents_count<T>())::value > ::std::decay_t<I>::value,
                "Index must be less than number of parents");
#endif
  return ::boost::hana::at(parents_types<T>(), index);
}

/**
 * @brief Returns amount of elements generated by the generator
 * @return boost::hana::llong<...>
 */
template <class T, class Generator = info::DefaultIndexGenerator>
constexpr decltype(auto) count() {
#ifndef COUSOUS_DISABLE_MOST_CT_CHECKS
  static_assert(decltype(check_reflected<T>())::value,
                "Class must be reflected");
  static_assert(info::is_generator_v<::std::decay_t<Generator>>,
                "Generator is not a generator class");

#endif
  // Forcing unevaluated context to not interact with data(only with
  // types)
  return ::boost::hana::llong_c<decltype(
      ::boost::hana::size(metautils::copy_tuple_sequence(
          MetaClass<T>::metadata, Generator::template generate<decltype(
                                      MetaClass<T>::metadata)>())))::value>;
}

/**
 * @brief Get name of element
 * @param index - index(boost::hana::llong)
 * @return boost::hana::string<...>
 */
template <class T, class Generator = info::DefaultIndexGenerator, class I>
constexpr decltype(auto) member_name(I &&index) {
#ifndef COUSOUS_DISABLE_MOST_CT_CHECKS
  static_assert(decltype(check_reflected<T>())::value,
                "Class must be reflected");
  static_assert(info::is_generator_v<::std::decay_t<Generator>>,
                "Generator is not a generator class");
  static_assert(
      ::std::is_same_v<::boost::hana::integral_constant_tag<long long>,
                       ::boost::hana::tag_of_t<I>>,
      "Index must be an integral constant of long long");
  static_assert(
      decltype(::boost::hana::size(metautils::copy_tuple_sequence(
          MetaClass<T>::names, Generator::template generate<decltype(
                                   MetaClass<T>::metadata)>())))::value >
          ::std::decay_t<I>::value,
      "Index must be less than number of elements in the generator");
#endif
  return ::boost::hana::at(
      metautils::copy_tuple_sequence(
          MetaClass<T>::names,
          Generator::template generate<decltype(MetaClass<T>::metadata)>()),
      index);
}

/**
 * @brief Find element index by name
 * @param str - ct-string (boost::hana::string)
 * @return boost::hana::tuple_c<long long,...>
 */
template <class T, class Generator = info::DefaultIndexGenerator, class String>
constexpr decltype(auto) find_by_name(String &&str) {
#ifndef COUSOUS_DISABLE_MOST_CT_CHECKS
  static_assert(decltype(check_reflected<T>())::value,
                "Class must be reflected");
  static_assert(info::is_generator_v<::std::decay_t<Generator>>,
                "Generator is not a generator class");
  static_assert(::std::is_same_v<::boost::hana::string_tag,
                                 ::boost::hana::tag_of_t<String>>,
                "str must be a ct-string");
#endif
  return metautils::find_value_types(
      str,
      metautils::copy_tuple_sequence(
          MetaClass<T>::names,
          Generator::template generate<decltype(MetaClass<T>::metadata)>()));
}

/**
 * @brief Find first element index by name
 * @param str - ct-string (boost::hana::string)
 * @return boost::hana::llong<...>
 */
template <class T, class Generator = info::DefaultIndexGenerator, class String>
constexpr decltype(auto) find_by_name_first(String &&str) {
#ifndef COUSOUS_DISABLE_MOST_CT_CHECKS
  static_assert(
      decltype(::boost::hana::size(find_by_name<T, Generator>(str)))::value > 0,
      "Tuple is empty");
#endif
  return ::boost::hana::at_c<0>(find_by_name<T, Generator>(str));
}

/**
 * @brief Return args types of method
 * @param index - method index(boost::llong)
 * @return boost::hana::tuple_t<Types...>
 */
template <class T, class Generator = info::DefaultIndexGenerator, class I>
constexpr decltype(auto) method_args(I &&index) {
#ifndef COUSOUS_DISABLE_MOST_CT_CHECKS
  static_assert(decltype(check_reflected<T>())::value,
                "Class must be reflected");
  static_assert(info::is_generator_v<::std::decay_t<Generator>>,
                "Generator is not a generator class");
  static_assert(
      ::std::is_same_v<::boost::hana::integral_constant_tag<long long>,
                       ::boost::hana::tag_of_t<I>>,
      "Index must be an integral constant of long long");
  static_assert(
      decltype(::boost::hana::size(metautils::copy_tuple_sequence(
          MetaClass<T>::names, Generator::template generate<decltype(
                                   MetaClass<T>::metadata)>())))::value >
          ::std::decay_t<I>::value,
      "Index must be less than number of elements in the generator");
#endif
  return detail::method_args_helper_impl<
      typename ::std::decay_t<decltype(::boost::hana::at(
          metautils::copy_tuple_sequence(
              MetaClass<T>::metadata,
              Generator::template generate<decltype(MetaClass<T>::metadata)>()),
          index))>::arg_types>::value;
}

/**
 * @brief Returns amount of method args
 * @param index - method index(boost::hana::llong)
 * @return boost::hana::llong<...>
 */
template <class T, class Generator = info::DefaultIndexGenerator, class I>
constexpr decltype(auto) methods_args_count(I &&index) {
  return ::boost::hana::size(method_args<T, Generator>(index));
}

/**
 * @brief Returns result type of method/variable
 * @param index - method index(boost::hana::llong)
 * @return boost::hana::type_t<...>
 */
template <class T, class Generator = info::DefaultIndexGenerator, class I>
constexpr decltype(auto) result_type(I &&index) {
#ifndef COUSOUS_DISABLE_MOST_CT_CHECKS
  static_assert(decltype(check_reflected<T>())::value,
                "Class must be reflected");
  static_assert(info::is_generator_v<::std::decay_t<Generator>>,
                "Generator is not a generator class");
  static_assert(
      ::std::is_same_v<::boost::hana::integral_constant_tag<long long>,
                       ::boost::hana::tag_of_t<I>>,
      "Index must be an integral constant of long long");
  static_assert(
      decltype(::boost::hana::size(metautils::copy_tuple_sequence(
          MetaClass<T>::names, Generator::template generate<decltype(
                                   MetaClass<T>::metadata)>())))::value >
          ::std::decay_t<I>::value,
      "Index must be less than number of elements in the generator");
#endif
  return ::boost::hana::type_c<
      typename ::std::decay_t<decltype(::boost::hana::at(
          metautils::copy_tuple_sequence(
              MetaClass<T>::metadata,
              Generator::template generate<decltype(MetaClass<T>::metadata)>()),
          index))>::return_type>;
}

/**
 * @brief Returns N-th argument type of method
 * @param index1 - method index(boost::hana::llong)
 * @param index2 - argument index(boost::hana::llong)
 * @return boost::hana::type_t<...>
 *
 */
template <class T, class Generator = info::DefaultIndexGenerator, class I,
          class J>
constexpr decltype(auto) method_arg(I &&index1, J &&index2) {
#ifndef COUSOUS_DISABLE_MOST_CT_CHECKS
  static_assert(
      ::std::is_same_v<::boost::hana::integral_constant_tag<long long>,
                       ::boost::hana::tag_of_t<J>>,
      "Index must be an integral constant of long long");
  constexpr auto args_count = decltype(::boost::hana::size(
      method_args<T, Generator>(::std::forward<I>(index1))))::value;
  static_assert(args_count > ::std::decay_t<J>::value,
                "Index must be less than number of types in the method");
#endif
  return ::boost::hana::at(method_args<T, Generator>(::std::forward<I>(index1)),
                           index2);
}

/**
 * @brief Checks whether method can be invoked
 * @param index - index(boost::hana::llong)
 * @return boost::hana::type_c<bool,...> (true or
 * false if method can be invoked)
 */
template <class T, class Generator, class... Args, class I>
constexpr decltype(auto) check_invoke(I &&index) {
#ifndef COUSOUS_DISABLE_MOST_CT_CHECKS
  static_assert(decltype(check_reflected<T>())::value,
                "Class must be reflected");
  static_assert(info::is_generator_v<::std::decay_t<Generator>>,
                "Generator is not a generator class");
  static_assert(
      ::std::is_same_v<::boost::hana::integral_constant_tag<long long>,
                       ::boost::hana::tag_of_t<I>>,
      "Index must be an integral constant of long long");
  static_assert(
      decltype(::boost::hana::size(metautils::copy_tuple_sequence(
          MetaClass<T>::names, Generator::template generate<decltype(
                                   MetaClass<T>::metadata)>())))::value >
          ::std::decay_t<I>::value,
      "Index must be less than number of elements in the generator");
#endif
  return detail::check_invoke_impl<
      ::std::decay_t<decltype(::boost::hana::at(
          metautils::copy_tuple_sequence(
              MetaClass<T>::metadata,
              Generator::template generate<decltype(MetaClass<T>::metadata)>()),
          index))>,
      Args...>();
}

/**
 * @brief Check if we can assign to return value(or value) of method/variable
 * @param index - index(boost::hana::llong)
 * @return boost::hana::type_c<bool,...> (true or
 * false if method can be invoked)
 */
template <class T, class Generator, class SetArg, class... Args, class I>
constexpr decltype(auto) check_set(I &&index) {
#ifndef COUSOUS_DISABLE_MOST_CT_CHECKS
  static_assert(decltype(check_reflected<T>())::value,
                "Class must be reflected");
  static_assert(info::is_generator_v<::std::decay_t<Generator>>,
                "Generator is not a generator class");
  static_assert(
      ::std::is_same_v<::boost::hana::integral_constant_tag<long long>,
                       ::boost::hana::tag_of_t<I>>,
      "Index must be an integral constant of long long");
  static_assert(
      decltype(::boost::hana::size(metautils::copy_tuple_sequence(
          MetaClass<T>::names, Generator::template generate<decltype(
                                   MetaClass<T>::metadata)>())))::value >
          ::std::decay_t<I>::value,
      "Index must be less than number of elements in the generator");
#endif
  return detail::check_set_impl<
      ::std::decay_t<decltype(::boost::hana::at(
          metautils::copy_tuple_sequence(
              MetaClass<T>::metadata,
              Generator::template generate<decltype(MetaClass<T>::metadata)>()),
          index))>,
      SetArg, Args...>();
}
}
}

#endif // REFLECT_METADDATA_HPP
