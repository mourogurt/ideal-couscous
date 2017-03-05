#ifndef META_INFORMATION_HPP
#define META_INFORMATION_HPP

#include "../meta_utils/utils.hpp"
#include "variables/reflect_information_variable.hpp"

namespace reflect {

namespace info {

//constexpr auto is_generator_v = boost::hana::is_valid([](auto&& x) -> decltype (x.template generate<boost::hana::tuple<>>()){});

/*template <class T>
struct is_gen_template {

};*/

template <class T>
struct is_generator {
    template <class C, ::std::size_t... Indices> static constexpr ::std::true_type check(decltype (&C::template generate<::boost::hana::tuple<::boost::hana::size_t<Indices>...>>));
    template <class> static constexpr ::std::false_type check(...);
    static constexpr bool value = ::std::is_same<::std::true_type, decltype(check<T>(nullptr))>::value;
};

template <class T> constexpr bool is_generator_v = is_generator<T>::value;

template <class T>
/**
 * @brief SFINAE check if class is reflected
 *
 */
struct is_reflected {

    /**
     * @brief If is_reflected method exist
     *
     * @return ::std::true_type
     */
    template <class C> static constexpr ::std::true_type check(decltype(&C::is_reflected));
    /**
     * @brief If is_reflected method doesn't exist
     *
     * @return ::std::false_type
     */
    template <class> static constexpr ::std::false_type check(...);
    static constexpr bool value = ::std::is_same<::std::true_type, decltype(check<T>(nullptr))>::value; /**< true true if reflected, othervise false */
};

template <class T> constexpr bool is_reflected_v = is_reflected<T>::value; /**< Helper variable template for is_reflected */

namespace detail {

template <class Type, class MetaInfo_type, ::std::size_t... Indices>
constexpr decltype (auto) names_tuple (::std::index_sequence<Indices... >&&) {
    return metautils::multiple_concat(names_state(metautils::counter<Indices>{},static_cast<const Type*>(nullptr),static_cast<const MetaInfo_type*>(nullptr))...);
}

template <class Type, class MetaInfo_type, ::std::size_t... Indices>
constexpr decltype (auto) metadata_tuple (::std::index_sequence<Indices... >&&) {
    return metautils::multiple_concat(metadata_state(metautils::counter<Indices>{},static_cast<const Type*>(nullptr),static_cast<const MetaInfo_type*>(nullptr))...);
}

}

template <class T>
/**
 * @brief Class that stores meta-information about T
 *
 */
struct MetaClass {
    static constexpr auto class_name   {HANA_STR(class_name_detail(static_cast<const typename T::Type*>(nullptr),static_cast<const T*>(nullptr)))}; /**< compile-time string of class name */
    static constexpr auto names        {detail::names_tuple<typename T::Type, T>(::std::make_index_sequence<decltype(T::counter(metautils::counter<>{}))::value>{})}; /**< tuple of all variable names */
    static constexpr auto metadata     {detail::metadata_tuple<typename T::Type, T>(::std::make_index_sequence<decltype(T::counter(metautils::counter<>{}))::value>{})}; /**< tuple of all method names  */
};

class EmptyGenerator;

class DefaultIndexGenerator final {
public:
    using reverse = EmptyGenerator;

    template<class Tuple>
    constexpr static decltype (auto) generate () {
        return metautils::generate_tuple_indices<decltype(::boost::hana::size(::std::declval<Tuple>()))>();
    }
};

class EmptyGenerator final {
public:
    using reverse = DefaultIndexGenerator;

    template<class Tuple>
    constexpr static ::boost::hana::tuple<> generate () {
        return {};
    }
};

template <class... Args>
struct MetaInfo;

}

}

#define IN_CLASS_META_INFO(TYPE) \
    using Type = TYPE; \
    using MetaInfo_type = TYPE; \
    static constexpr auto is_reflected () {return std::true_type();} \
    friend constexpr auto class_name_detail(const Type*, const MetaInfo_type*) -> decltype (#TYPE) { return #TYPE; } \
    constexpr static ::reflect::metautils::counter<0> counter (::reflect::metautils::counter<0>);

#define OUT_OF_CLASS_META_INFO(TYPE) \
    friend struct reflect::info::MetaInfo<TYPE>; \
    using MetaInfo_type = ::reflect::info::MetaInfo<TYPE>; \
    static constexpr auto is_reflected () {return std::true_type();}


#define META_INFO(TYPE) \
    namespace reflect { \
        namespace info { \
            template <> \
            struct MetaInfo<TYPE> { \
                using Type = TYPE; \
                using MetaInfo_type = ::reflect::info::MetaInfo<Type>; \
                friend constexpr auto class_name_detail(const Type*, const MetaInfo_type*) -> decltype (#TYPE) { return #TYPE; } \
                constexpr static ::reflect::metautils::counter<0> counter (::reflect::metautils::counter<0>);

#define TEMPLATE_META_INFO(TYPE,TEMPLATE_TYPE,TEMPLATE) \
    namespace reflect { \
        namespace info { \
            template <TEMPLATE_TYPE> \
            struct MetaInfo<TYPE<TEMPLATE>> { \
                using Type = TYPE<TEMPLATE>; \
                using MetaInfo_type = ::reflect::info::MetaInfo<Type>; \
                friend constexpr auto class_name_detail(const Type*, const MetaInfo_type*) -> decltype (#TYPE) { return #TYPE; } \
                constexpr static ::reflect::metautils::counter<0> counter (::reflect::metautils::counter<0>);

#define END_META_INFO \
            }; \
    } }

#define TUPLE_APPEND(STATE, COUNTER, ...) \
    friend constexpr auto STATE(::reflect::metautils::counter<decltype(COUNTER(::reflect::metautils::counter<>{}))::value>, \
            const Type*, const MetaInfo_type*) -> decltype (::boost::hana::make_tuple(__VA_ARGS__)) \
        { return ::boost::hana::make_tuple(__VA_ARGS__); }

#define INCREASE_COUNTER(COUNTER) \
    constexpr static ::reflect::metautils::counter<decltype(COUNTER(::reflect::metautils::counter<>{}))::value+1u> \
                     COUNTER (::reflect::metautils::counter<decltype(COUNTER(::reflect::metautils::counter<>{}))::value+1u>);

#endif // META_INFORMATION_HPP
