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
    //Template methods can't be reflected(because there is no pointer to template method).
    REFLECT_OBJ_MTD(foo<int>,int&&)
    //What can't be said about the specialization of template methods.
    REFLECT_OBJ_MTD(foo<double>,double&&)
    REFLECT_OBJ_MTD(bar)
    REFLECT_OBJ_MTD(bar,float)
};

int main() {
    //member_name - get N-th name (ct-string) of member(var/method).
    std::cout << "Method name: "
              << couscous::get_opt_val(hana::transform(couscous::member_name<SomeStruct,couscous::AllMethods>(hana::size_c<0>),hana::to<const char*>)) << std::endl;
    constexpr auto inds = couscous::gen_inds_tup<decltype (couscous::get_opt_val(couscous::count<SomeStruct,couscous::AllMethods>()))>();
    hana::for_each(inds,[](auto&& item){
        std::cout << "Method name: "
                  << couscous::get_opt_val(hana::transform(couscous::member_name<SomeStruct,couscous::AllMethods>(item),hana::to<const char*>)) << std::endl;
    }); //Now we generate tuple of indices by calling gen_inds_tup<N,Offset = hana::size_t<0>>() and call lambda for each item in tuple.
    //find_by_name - finds indices by ct-string and return them in tuple.
    constexpr auto inds_tup = couscous::get_opt_val(couscous::find_by_name<SomeStruct,couscous::AllMethods>(BOOST_HANA_STRING("bar")));
    hana::for_each(inds_tup,[](auto&& item) {
        std::cout << "Index: " << item << '\t'
                  << couscous::get_opt_val(hana::transform(couscous::member_name<SomeStruct,couscous::AllMethods>(item),hana::to<const char*>)) << std::endl;
    });
}
