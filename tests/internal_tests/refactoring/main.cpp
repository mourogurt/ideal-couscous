#include <reflect.hpp>
#include <iostream>
#include <experimental/type_traits>
#include <cmath>
#include <boost/hana/for_each.hpp>
#include <QObject>
#include <limits>

struct Obj {
    int a{3};
    static int b;
    OUT_METAINFO(Obj)
};

METAINFO(Obj)
REFLECT_OBJ_VAR(a)
REFLECT_STATIC_VAR(b)
END_METAINFO


class Obj2 {
public:
    int b{2};
    void foo (int){ return;}
    void foo (std::string){}
    void foo () const {}
    static void foo (double) {}
    IN_METAINFO(Obj2)
    REFLECT_OBJ_VAR(b)
    REFLECT_OBJ_MET(foo,int)
    REFLECT_OBJ_MET(foo,std::string)
    REFLECT_CONST_OBJ_MET(foo)
    REFLECT_STATIC_MET(foo,double)
};

int main() {

    return 0;
}
