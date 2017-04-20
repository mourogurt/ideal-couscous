#include <iostream>

#include <reflect.hpp>
#include <boost/hana/for_each.hpp>

namespace couscous {
inline namespace utils {using namespace ::reflect::utils; }
inline namespace metautils { using namespace ::reflect::metautils; }
inline namespace reflect {using namespace ::reflect; }
}

namespace hana = boost::hana;

struct SomeStruct{
    template<class T>
    void foo(T&&);
    void bar();
    void bar(float);
    IN_METAINFO(SomeStruct)
    REFLECT_OBJ_MTD(foo<int>,int&&)         //We can't reflect template method(because we can't get pointer to template method), but we can reflect specialized method
    REFLECT_OBJ_MTD(foo<double>,double&&)
    REFLECT_OBJ_MTD(bar)
    REFLECT_OBJ_MTD(bar,float)
};

int main() {
    //String: there are two ways to create meta string. First: BOOST_HANA_STRING("...") - pros: input string can be any size; cons: only run time creation(at least for now)
    //Second: HANA_STR("...") - pros: compile time where it possible; cons: max size of input string limited to STRING_MAXLEN
    std::cout << std::boolalpha << (BOOST_HANA_STRING("Hello couscous") == HANA_STR("Hello couscous")) << std::endl;
    constexpr auto str = HANA_STR("This is ct-string\n");
    std::cout << hana::to<const char*>(str);
    //member_name - get N-th name (ct-string) of member
    std::cout << couscous::get_opt_val(hana::transform(couscous::member_name<SomeStruct,couscous::AllMethods>(hana::size_c<0>),hana::to<const char*>)) << std::endl;
    std::cout << std::endl;
    hana::for_each(couscous::gen_inds_tup<decltype (couscous::get_opt_val(couscous::count<SomeStruct,couscous::AllMethods>()))>(),[](auto&& item){
        std::cout << couscous::get_opt_val(hana::transform(couscous::member_name<SomeStruct,couscous::AllMethods>(item),hana::to<const char*>)) << std::endl;
    }); //Now we generate tuple of indices by calling gen_inds_tup<N,Offset = hana::size_t<0>>() and call lambda for each item in tuple
    //find_by_name - finds indices by ct-string and return them in tuple
    constexpr auto inds_tup = couscous::get_opt_val(couscous::find_by_name<SomeStruct,couscous::AllMethods>(HANA_STR("bar")));
    hana::for_each(inds_tup,[](auto&& item) {
        std::cout << item << '\t' << couscous::get_opt_val(hana::transform(couscous::member_name<SomeStruct,couscous::AllMethods>(item),hana::to<const char*>)) << std::endl;
    });
}
