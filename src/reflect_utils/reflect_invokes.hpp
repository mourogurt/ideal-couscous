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
 * @brief  Implementation of get function
 * @param p - metadata of element
 * @param args - Arguments of invocation
 * @return T::return_type
 */
template <class T, class... Args>
constexpr decltype(auto) get_impl(T &&p, Args &&... args) {
  if
    constexpr(::std::decay_t<decltype(::boost::hana::size(
                  ::std::declval<typename T::arg_types>()))>::value !=
              0) return p(::std::forward<Args>(args)...);
  else
    return p();
}

/**
 * @brief Implementation of set function
 * @param p - metadata of element
 * @param s_arg - rvalue argument
 * @param args - Arguments of invocation
 * @return boost::hana::bool_c<true>
 */
template <class T, class SetArg, class... Args>
constexpr decltype(auto) set_impl(T &&p, SetArg &&s_arg, Args &&... args) {
  if
    constexpr(::std::decay_t<decltype(::boost::hana::size(
                  ::std::declval<typename T::arg_types>()))>::value != 0) {
      p(::std::forward<Args>(args)...) = s_arg;
      return ::boost::hana::bool_c<true>;
    }
  else {
    p() = s_arg;
    return ::boost::hana::bool_c<true>;
  }
}

/**
 * @brief Implementation of gets function
 * @param inds_tup - tuple of indices
 * @param ::std::index_sequence<Indices...> - index sequence
 * @param args - Arguments of invocation
 * @return boost::hana::tuple<...>
 */
template <class T, class Generator, class IndexTuple, ::std::size_t... Indices,
          class... Args>
constexpr decltype(auto) gets_impl(IndexTuple &&inds_tup,
                                   ::std::index_sequence<Indices...> &&,
                                   Args &&... args) {
#ifndef COUSOUS_DISABLE_MOST_CT_CHECKS
  static_assert(::std::decay_t<decltype(
                        ::boost::hana::size(metautils::copy_tuple_sequence(
                            MetaClass<T>::metadata,
                            Generator::template generate<decltype(
                                MetaClass<T>::metadata)>())))>::value >=
                    decltype(::boost::hana::size(inds_tup))::value,
                "Number of indices must be not greater than number of elements "
                "in generator");
#endif
  return metautils::multiple_concat(::boost::hana::make_tuple(
      get<T, Generator>(::boost::hana::at_c<Indices>(inds_tup),
                        ::std::forward<Args>(args)...))...);
}

/**
 * @brief Implementation of sets function
 * @param inds_tup - tuple of indices
 * @param s_arg - rvalue argument
 * @param ::std::index_sequence<Indices...> - index sequence
 * @param args - Arguments of invocation
 * @return boost::hana::tuple<...>
 */
template <class T, class Generator, class IndexTuple, class SetArg,
          ::std::size_t... Indices, class... Args>
constexpr decltype(auto) sets_impl(IndexTuple &&inds_tup, SetArg &&s_arg,
                                   ::std::index_sequence<Indices...> &&,
                                   Args &&... args) {
#ifndef COUSOUS_DISABLE_MOST_CT_CHECKS
  static_assert(::std::decay_t<decltype(
                        ::boost::hana::size(metautils::copy_tuple_sequence(
                            MetaClass<T>::metadata,
                            Generator::template generate<decltype(
                                MetaClass<T>::metadata)>())))>::value >=
                    decltype(::boost::hana::size(inds_tup))::value,
                "Number of indices must be not greater than number of elements "
                "in generator");
#endif
  return metautils::multiple_concat(::boost::hana::make_tuple(set<T, Generator>(
      ::boost::hana::at_c<Indices>(inds_tup), ::std::forward<SetArg>(s_arg),
      ::std::forward<Args>(args)...))...);
}

/**
 * @brief Extracting each argument from tuple
 * @param index - index of element to invoke (boost::hana::llong)
 * @param tup - tuple of args
 * @param std::index_sequence<Indices...> - index sequence
 * @return boost::hana::tuple<type of get(...)>
 */
template <class T, class Generator, class I, class Tuple,
          ::std::size_t... Indices>
constexpr decltype(auto)
gets_unpack_tuple_element_impl(I &&index, Tuple &&tup,
                               ::std::index_sequence<Indices...> &&) {
  return ::boost::hana::make_tuple(get<T, Generator>(
      ::std::forward<I>(index), ::boost::hana::at_c<Indices>(tup)...));
}

/**
 * @brief Extracting each index and args tuple
 * @param index - index of element to invoke (boost::hana::llong)
 * @param tup - tuple of args
 * @return boost::hana::tuple<type of get(...)>
 */
template <class T, class Generator, class I, class Tuple>
constexpr decltype(auto) gets_tuple_element_impl(I &&index, Tuple &&tup) {
  return gets_unpack_tuple_element_impl<T, Generator>(
      ::std::forward<I>(index), ::std::forward<Tuple>(tup),
      ::std::make_index_sequence<decltype(::boost::hana::size(tup))::value>());
}

/**
 * @brief Implementation of gets_tuple_args
 * @param inds_tup - tuple of indices
 * @param args_tup - tuple of tuples of arguments
 * @param std::index_sequence<Indices...> - index sequence
 * @return boost::hana::tuple<type of each get(...)>
 */
template <class T, class Generator, class IndexTuple, class ArgsTuple,
          ::std::size_t... Indices>
constexpr decltype(auto)
gets_tuple_args_impl(IndexTuple &&inds_tup, ArgsTuple &&args_tup,
                     ::std::index_sequence<Indices...> &&) {
  return metautils::multiple_concat(gets_tuple_element_impl<T, Generator>(
      ::boost::hana::at_c<Indices>(inds_tup),
      ::boost::hana::at_c<Indices>(args_tup))...);
}

/**
 * @brief Extracting each argument from tuple
 * @param index - index of element to invoke (boost::hana::llong)
 * @param s_arg - rvalue argument
 * @param tup - tuple of args
 * @param std::index_sequence<Indices...> - index sequence
 * @return boost::hana::tuple<type of each set(...)>
 */
template <class T, class Generator, class I, class SetArg, class Tuple,
          ::std::size_t... Indices>
constexpr decltype(auto)
sets_unpack_tuple_element_impl(I &&index, SetArg &&s_arg, Tuple &&tup,
                               ::std::index_sequence<Indices...> &&) {
  return ::boost::hana::make_tuple(
      set<T, Generator>(::std::forward<I>(index), ::std::forward<SetArg>(s_arg),
                        ::boost::hana::at_c<Indices>(tup)...));
}

/**
 * @brief Extracting each index, args tuple and rvalue argument
 * @param index - index of element to invoke (boost::hana::llong)
 * @param s_arg - rvalue argument
 * @param tup - tuple of args
 * @return boost::hana::tuple<type of each set(...)>
 */
template <class T, class Generator, class I, class SetArg, class Tuple>
constexpr decltype(auto) sets_tuple_element_impl(I &&index, SetArg &&s_arg,
                                                 Tuple &&tup) {
  return sets_unpack_tuple_element_impl<T, Generator>(
      ::std::forward<I>(index), ::std::forward<SetArg>(s_arg),
      ::std::forward<Tuple>(tup),
      ::std::make_index_sequence<decltype(::boost::hana::size(tup))::value>());
}

/**
 * @brief Implementation of sets_tuple_args
 * @param inds_tup - tuple of indices
 * @param s_args_tup - tuple of rvalue arguments
 * @param args_tup - tuple of tuples of arguments
 * @param std::index_sequence<Indices...> - index sequence
 * @return boost::hana::tuple<type of each set(...)>
 */
template <class T, class Generator, class IndexTuple, class SetArgTuple,
          class ArgsTuple, ::std::size_t... Indices>
constexpr decltype(auto)
sets_tuple_args_impl(IndexTuple &&inds_tup, SetArgTuple &&s_args_tup,
                     ArgsTuple &&args_tup,
                     ::std::index_sequence<Indices...> &&) {
  return metautils::multiple_concat(sets_tuple_element_impl<T, Generator>(
      ::boost::hana::at_c<Indices>(inds_tup),
      ::boost::hana::at_c<Indices>(s_args_tup),
      ::boost::hana::at_c<Indices>(args_tup))...);
}
}

/**
 * @brief Get value of a method/variable
 * @param index - index of element to invoke (boost::hana::llong)
 * @param args - Invocation arguments
 * @return object which type is type of variable or result type of
 * method(bost::hana::type_c<void> if result type is void) or
 * boost::hana::nothing if error occurs
 */
template <class T, class Generator, class I, class... Args>
constexpr decltype(auto) get(I &&index, Args &&... args) {
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
  if
    constexpr(decltype(::boost::hana::equal(
        check_invoke<T, Generator, decltype(::std::forward<Args>(args))...>(
            index),
        ::boost::hana::bool_c<true>))::value) {
      if
        constexpr(
            !::std::is_void<decltype(detail::get_impl(
                ::boost::hana::at(metautils::copy_tuple_sequence(
                                      MetaClass<T>::metadata,
                                      Generator::template generate<decltype(
                                          MetaClass<T>::metadata)>()),
                                  index),
                ::std::forward<Args>(args)...))>::value) {
          return detail::get_impl(
              ::boost::hana::at(metautils::copy_tuple_sequence(
                                    MetaClass<T>::metadata,
                                    Generator::template generate<decltype(
                                        MetaClass<T>::metadata)>()),
                                index),
              ::std::forward<Args>(args)...);
        }
      else {
        detail::get_impl(
            ::boost::hana::at(metautils::copy_tuple_sequence(
                                  MetaClass<T>::metadata,
                                  Generator::template generate<decltype(
                                      MetaClass<T>::metadata)>()),
                              index),
            ::std::forward<Args>(args)...);
        return ::boost::hana::type_c<void>;
      }
    }
  else
    return ::boost::hana::nothing;
}

/**
 * @brief Set value to method/variable
 * @param index - index of element to invoke (boost::hana::llong)
 * @param s_arg - rvalue argument
 * @param args - Invocation arguments
 * @return boost::hana::bool_c<true> or boost::hana::nothing
 */
template <class T, class Generator = info::DefaultIndexGenerator, class I,
          class SetArg, class... Args>
constexpr decltype(auto) set(I &&index, SetArg &&s_arg, Args &&... args) {
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
  if
    constexpr(decltype(::boost::hana::equal(
        check_set<T, Generator, decltype(s_arg), decltype(args)...>(index),
        ::boost::hana::bool_c<true>))::value) {
      return detail::set_impl(
          ::boost::hana::at(
              metautils::copy_tuple_sequence(
                  MetaClass<T>::metadata, Generator::template generate<decltype(
                                              MetaClass<T>::metadata)>()),
              index),
          ::std::forward<SetArg>(s_arg), ::std::forward<Args>(args)...);
    }
  else
    return ::boost::hana::nothing;
}

/**
 * @brief Get multiple values of variables/methods
 * @param inds_tup - tuple of indices
 * @param args - Invocation arguments
 * @return boost::hana::tuple<type of each get(...)>
 */
template <class T, class Generator = info::DefaultIndexGenerator,
          class IndexTuple, class... Args>
constexpr decltype(auto) gets(IndexTuple &&inds_tup, Args &&... args) {
#ifndef COUSOUS_DISABLE_MOST_CT_CHECKS
  static_assert(decltype(check_reflected<T>())::value,
                "Class must be reflected");
  static_assert(info::is_generator_v<::std::decay_t<Generator>>,
                "Generator is not a generator class");
  static_assert(
      ::std::is_same_v<::boost::hana::tuple_tag,
                       ::boost::hana::tag_of_t<::std::decay_t<IndexTuple>>>,
      "inds_tup must be a tuple");
#endif
  return detail::gets_impl<T, Generator>(
      ::std::forward<IndexTuple>(inds_tup),
      ::std::make_index_sequence<decltype(
          ::boost::hana::size(inds_tup))::value>(),
      ::std::forward<Args>(args)...);
}

/**
 * @brief Set multiple values of variables/methods
 * @param inds_tup - tuple of indices
 * @param s_arg - rvalue argument
 * @param args - Invocation arguments
 * @return boost::hana::tuple<type of each set(...)>
 */
template <class T, class Generator = info::DefaultIndexGenerator,
          class IndexTuple, class SetArg, class... Args>
constexpr decltype(auto) sets(IndexTuple &&inds_tup, SetArg &&s_arg,
                              Args &&... args) {
#ifndef COUSOUS_DISABLE_MOST_CT_CHECKS
  static_assert(decltype(check_reflected<T>())::value,
                "Class must be reflected");
  static_assert(info::is_generator_v<::std::decay_t<Generator>>,
                "Generator is not a generator class");
  static_assert(
      ::std::is_same_v<::boost::hana::tuple_tag,
                       ::boost::hana::tag_of_t<::std::decay_t<IndexTuple>>>,
      "inds_tup must be a tuple");
#endif
  return detail::sets_impl<T, Generator>(
      ::std::forward<IndexTuple>(inds_tup), ::std::forward<SetArg>(s_arg),
      ::std::make_index_sequence<decltype(
          ::boost::hana::size(inds_tup))::value>(),
      ::std::forward<Args>(args)...);
}

/**
 * @brief Get multiple values of variables/methods (like gets, but tuple as
 * input parameter)
 * @param inds_tup - tuple of indices
 * @param args_tup - tuple of tuples of arguments
 * @return boost::hana::tuple<type of each get(...)>
 */
template <class T, class Generator = info::DefaultIndexGenerator,
          class IndexTuple, class ArgsTuple>
constexpr decltype(auto) gets_tuple_args(IndexTuple &&inds_tup,
                                         ArgsTuple &&args_tup) {
#ifndef COUSOUS_DISABLE_MOST_CT_CHECKS
  static_assert(decltype(check_reflected<T>())::value,
                "Class must be reflected");
  static_assert(info::is_generator_v<::std::decay_t<Generator>>,
                "Generator is not a generator class");
  static_assert(
      ::std::is_same_v<::boost::hana::tuple_tag,
                       ::boost::hana::tag_of_t<::std::decay_t<IndexTuple>>>,
      "inds_tup must be a tuple");
  static_assert(
      ::std::is_same_v<::boost::hana::tuple_tag,
                       ::boost::hana::tag_of_t<::std::decay_t<ArgsTuple>>>,
      "args_tup must be a tuple");
  static_assert(
      decltype(::boost::hana::equal(::boost::hana::size(inds_tup),
                                    ::boost::hana::size(args_tup)))::value,
      "Number of elements in each tuple must be equal");
#endif
  return detail::gets_tuple_args_impl<T, Generator>(
      ::std::forward<IndexTuple>(inds_tup), ::std::forward<ArgsTuple>(args_tup),
      ::std::make_index_sequence<decltype(
          ::boost::hana::size(inds_tup))::value>());
}

/**
 * @brief Set multiple values of variables/methods (like sets, but tuple  as
 * input parameter)
 * @param inds_tup - tuple of indices
 * @param s_args_tup - tuple of rvalue arguments
 * @param args_tup - tuple of tuples of arguments
 * @return boost::hana::optional<...> of boost::hana::tuple<type of each
 * set(...)> or boost::hana::nothing
 */
template <class T, class Generator = info::DefaultIndexGenerator,
          class IndexTuple, class SetArgTuple, class ArgsTuple>
constexpr decltype(auto) sets_tuple_args(IndexTuple &&inds_tup,
                                         SetArgTuple &&s_args_tup,
                                         ArgsTuple &&args_tup) {
#ifndef COUSOUS_DISABLE_MOST_CT_CHECKS
  static_assert(decltype(check_reflected<T>())::value,
                "Class must be reflected");
  static_assert(info::is_generator_v<::std::decay_t<Generator>>,
                "Generator is not a generator class");
  static_assert(
      ::std::is_same_v<::boost::hana::tuple_tag,
                       ::boost::hana::tag_of_t<::std::decay_t<IndexTuple>>>,
      "inds_tup must be a tuple");
  static_assert(
      ::std::is_same_v<::boost::hana::tuple_tag,
                       ::boost::hana::tag_of_t<::std::decay_t<SetArgTuple>>>,
      "s_args_tup must be a tuple");
  static_assert(
      ::std::is_same_v<::boost::hana::tuple_tag,
                       ::boost::hana::tag_of_t<::std::decay_t<ArgsTuple>>>,
      "args_tup must be a tuple");
  static_assert(
      decltype(::boost::hana::equal(::boost::hana::size(inds_tup),
                                    ::boost::hana::size(s_args_tup)))::value,
      "Number of elements in each tuple must be equal");
  static_assert(
      decltype(::boost::hana::equal(::boost::hana::size(inds_tup),
                                    ::boost::hana::size(args_tup)))::value,
      "Number of elements in each tuple must be equal");
#endif
  return detail::sets_tuple_args_impl<T, Generator>(
      ::std::forward<IndexTuple>(inds_tup),
      ::std::forward<SetArgTuple>(s_args_tup),
      ::std::forward<ArgsTuple>(args_tup),
      ::std::make_index_sequence<decltype(
          ::boost::hana::size(inds_tup))::value>());
}
}
}

#endif // REFLECT_INVOKES_HPP
