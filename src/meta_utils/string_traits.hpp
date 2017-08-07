#ifndef REFL_STRING_TRAITS_HPP
#define REFL_STRING_TRAITS_HPP

#include <boost/hana/drop_back.hpp>
#include <boost/hana/minus.hpp>
#include <boost/hana/not_equal.hpp>
#include <boost/hana/size.hpp>
#include <boost/hana/string.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>

namespace reflect {

namespace metautils {

namespace detail {

/**
 * @brief counting non-zero chars in ct-string
 * @param std::index_sequence<Indices...> indices of ct-string
 * @param str - ct-string
 * @return std::size_t value of non-zero chars
 */
template <class T, ::std::size_t... Indices>
constexpr decltype(auto)
count_nonzero_chars_impl(std::index_sequence<Indices...> &&, const T &str) {
  return (... + ((str[boost::hana::size_c<Indices>] != boost::hana::char_c<0>)
                     ? 1
                     : 0));
}

/**
 * @brief return real size of ct-string (excluding ending zero chars)
 * @param str - ct-string
 * @return std::size_t size of ct-string
 */
template <class T>
constexpr decltype(auto) real_string_size_impl(const T &str) {
  constexpr ::std::size_t N = decltype(::boost::hana::size(str))::value;
  return count_nonzero_chars_impl(::std::make_index_sequence<N>(), str);
}

/**
 * @brief generate ct-string from tuple of chars
 * @param boost::hana::tuple<boost::hana::char_<s>...> - tuple of chars
 * @return boost::hana::string - ct-string
 */
template <char... s>
constexpr decltype(auto) copy_string_from_tupl_impl(
    const boost::hana::tuple<boost::hana::char_<s>...> &) {
  return boost::hana::string_c<s...>;
}
}
/**
 * @brief remove ending zero chars from ct-string
 * @param str - ct-string with ending zero chars
 * @return boost::hana::string - ct-string without ending zero chars
 */
template <class T>
constexpr decltype(auto) remove_zeros_from_ct_string(const T str) {
  return detail::copy_string_from_tupl_impl(boost::hana::drop_back(
      boost::hana::unpack(str, boost::hana::make<boost::hana::tuple_tag>),
      (boost::hana::size(str) -
       boost::hana::size_c<detail::real_string_size_impl(str)>)));
}
}
}

#ifndef STRING_MAXLEN
#define STRING_MAXLEN 64 /**< maximum length of compile-time string */
#endif
#define CHECK_STR_CHAR(_, i, str) (sizeof(str) > (i) ? str[(i)] : 0),
#define CT_STR(str) BOOST_PP_REPEAT(STRING_MAXLEN, CHECK_STR_CHAR, str) 0

#define HANA_STR(str)                                                          \
  reflect::metautils::remove_zeros_from_ct_string(                             \
      ::boost::hana::string_c<CT_STR(                                          \
          str)>) /**<constexpr compile-time string */

#endif // STRING_TRAITS_HPP
