#ifndef REFLECT_METADDATA_HPP
#define REFLECT_METADDATA_HPP

#include "../meta_utils/meta_utils.hpp"
#include "../reflect_information/reflect_information.hpp"

namespace reflect {

namespace utils {

template <class T>
using MetaClass = info::MetaClass<typename ::std::decay_t<typename T::MetaInfo_type>>; /**< Helper type template to specify Metadata class */

template <class T>
constexpr decltype (auto) get_class_name() {
    if constexpr (!info::is_reflected_v<::std::decay_t<T>>) return ::boost::hana::nothing;
    return info::MetaClass<typename ::std::decay_t<typename T::MetaInfo_type> >::class_name;
}

template <class T, class Generator = info::DefaultIndexGenerator>
constexpr decltype (auto) get_count() {
    if constexpr (!info::is_reflected_v<::std::decay_t<T>>) return ::boost::hana::nothing;
    if constexpr (!info::is_generator_v<::std::decay_t<Generator>>) return ::boost::hana::nothing;
    //Forcing unevaluated context to not interact with data(only with types)
    return boost::hana::size_c<decltype(::boost::hana::size(metautils::copy_tuple_sequence(MetaClass<T>::metadata,Generator::template generate<decltype(MetaClass<T>::metadata)>())))::value>;
}

template <class T, class Generator = info::DefaultIndexGenerator, class I>
constexpr decltype (auto) get_name (I&& index) {
    if constexpr (!info::is_reflected_v<::std::decay_t<T>>) return ::boost::hana::nothing;
    if constexpr (!info::is_generator_v<::std::decay_t<Generator>>) return ::boost::hana::nothing;
    if constexpr (decltype(::boost::hana::size(metautils::copy_tuple_sequence(MetaClass<T>::names,Generator::template generate<decltype(MetaClass<T>::metadata)>())))::value
                  <= ::std::decay_t<I>::value) return ::boost::hana::nothing;
    return ::boost::hana::at(metautils::copy_tuple_sequence(MetaClass<T>::names,Generator::template generate<decltype(MetaClass<T>::metadata)>()),index);
}

template <class T, class Generator = info::DefaultIndexGenerator, class String>
constexpr decltype (auto) find_name(String&& str) {
    if constexpr (!info::is_reflected_v<::std::decay_t<T>>) return ::boost::hana::nothing;
    if constexpr (!info::is_generator_v<::std::decay_t<Generator>>) return ::boost::hana::nothing;
    return metautils::find_value_types(str,metautils::copy_tuple_sequence(MetaClass<T>::names,Generator::template generate<decltype(MetaClass<T>::metadata)>()));
}

}

}

#endif // REFLECT_METADDATA_HPP
