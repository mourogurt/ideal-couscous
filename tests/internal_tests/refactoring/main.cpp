#include <reflect.hpp>
#include <iostream>


struct Obj {
    int a{3};
    OUT_OF_CLASS_META_INFO(Obj)
};

META_INFO(Obj)
REFLECT_OBJ_VARIABLE(a)
END_META_INFO

struct Obj2 {
    int b{2};
    IN_CLASS_META_INFO(Obj2)
    REFLECT_OBJ_VARIABLE(b)
};

int main() {
    Obj o1;
    Obj2 o2;
    std::cout << boost::hana::to<const char*>(reflect::utils::get_class_name<Obj>()) << std::endl;
    std::cout << boost::hana::to<const char*>(reflect::utils::get_class_name<Obj2>()) << std::endl;
    std::cout << reflect::utils::get_count<Obj>() << std::endl;
    std::cout << reflect::utils::get_count<Obj2>() << std::endl;
    /*
    std::cout << boost::hana::at_c<0>(reflect::info::MetaClass<Obj::MetaInfo_type>::metadata)(o1) << std::endl;
    std::cout << boost::hana::at_c<0>(reflect::info::MetaClass<Obj2::MetaInfo_type>::metadata)(o2) << std::endl;
    std::cout << std::boolalpha << reflect::info::is_variable_v<std::decay_t<decltype (boost::hana::at_c<0>(reflect::info::MetaClass<Obj::MetaInfo_type>::metadata))>> << std::endl;
    std::cout << std::boolalpha << reflect::info::is_variable_v<std::decay_t<decltype (boost::hana::at_c<0>(reflect::info::MetaClass<Obj2::MetaInfo_type>::metadata))>> << std::endl;
    auto tup = reflect::info::ObjVariableIndexGenerator::generate<decltype(reflect::info::MetaClass<Obj::MetaInfo_type>::metadata)>();
    boost::hana::for_each(tup,[](auto&& x) {
       std::cout << x << " ";
    });
    std::cout << std::endl;*/
    return 0;
}
