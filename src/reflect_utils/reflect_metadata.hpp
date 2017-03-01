#ifndef REFLECT_METADDATA_HPP
#define REFLECT_METADDATA_HPP

#include "../meta_utils/utils.hpp"
#include "../reflect_information/reflect_information.hpp"

namespace reflect {

namespace utils {

template <class T>
using MetaClass = info::MetaClass<typename ::std::decay_t<typename T::MetaInfo_type>>;

template <class T>
constexpr decltype (auto) get_class_name() {
    return info::MetaClass<typename ::std::decay_t<typename T::MetaInfo_type> >::class_name;
}

template <class T, class Generator = info::DefaultIndexGenerator>
constexpr decltype (auto) get_count() {
    return ::boost::hana::size(metautils::copy_tuple_sequence(MetaClass<T>::metadata,Generator::template generate<decltype(MetaClass<T>::metadata)>()));
}

template <class T, class Generator = info::DefaultIndexGenerator, class I>
constexpr decltype (auto) get_name (T&& index) {
    constexpr auto tmp = metautils::copy_tuple_sequence(MetaClass<T>::names,Generator::template generate<decltype(MetaClass<T>::names)>());
    return ::boost::hana::at(tmp,index);
}

}

}

#endif // REFLECT_METADDATA_HPP
