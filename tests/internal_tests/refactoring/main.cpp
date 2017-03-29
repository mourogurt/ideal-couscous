#include <reflect.hpp>
#include <iostream>
#include <experimental/type_traits>
#include <cmath>

struct Obj {
    int a{3};
    OUT_METAINFO(Obj)
};

METAINFO(Obj)
REFLECT_OBJ_VAR(a)
END_METAINFO


struct Obj2 {
    int b{2};
    int foo (int){ return 0;}
    void foo (char){}
    void foo () const {}
    static void foo (double) {}
    IN_METAINFO(Obj2)
    REFLECT_OBJ_VAR(b)
    REFLECT_OBJ_MET(foo,int)
    REFLECT_OBJ_MET(foo,char)
    REFLECT_CONST_OBJ_MET(foo)
    REFLECT_STATIC_MET(foo,double)
};

int main() {
    /*make_mem_fn<decltype(reflect::info::detail::MethodInfo<Obj2,int>::return_type(&Obj2::foo)),
                decltype(reflect::info::detail::MethodInfo<Obj2,int>::pointer_type(&Obj2::foo)),int>(&Obj2::foo) a;*/
    //reflect::info::make_method<decltype(reflect::info::detail::MethodInfo<Obj2,int>::return_type(&Obj2::foo)) (int),Obj2, int>(&Obj2::foo);
    //reflect::info::make_method<decltype(reflect::info::detail::MethodInfo<Obj2,char>::return_type(&Obj2::foo)) (char),Obj2, char>(&Obj2::foo);
    //reflect::info::make_const_method<decltype(reflect::info::detail::MethodInfo<Obj2>::return_type(&Obj2::foo)) () const,Obj2>(&Obj2::foo);
    //reflect::info::make_method<decltype(reflect::info::detail::MethodInfo<void,double>::return_type(&Obj2::foo)) (double), double>(&Obj2::foo);
    //std::result_of
    //decltype(reflect::metautils::constexpr_invoke(&Obj2::foo,std::declval<Obj2>(),std::declval<int>())) a;
    //decltype(std::declval<Obj2&>().foo(std::declval<int>())) a;
    return 0;
}
