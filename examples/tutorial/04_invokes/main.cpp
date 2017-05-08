#include <iostream>
#include <functional>
#include <boost/hana/for_each.hpp>

#include <reflect.hpp>

namespace couscous {
inline namespace utils {using namespace ::reflect::utils; }
inline namespace metautils { using namespace ::reflect::metautils; }
inline namespace reflect {using namespace ::reflect; }
}

namespace hana = boost::hana;
using namespace boost::hana::literals;

class A {
    int num {0};
    int num2 {1};
public:
    std::string str {"Hello couscous" };
    static int static_num;
    int& num_accessor() { return num; }
    int& num2_accessor() { return num2; }
    double const_method () const { return 5.; }
    static void static_method (std::size_t num, const std::string& str) {
        for (auto i = 0u; i < num; ++i)
            std::cout << str << std::endl;
    }
    void method1 (int n) { std::cout << "method1 invoked with argument: " << n << std::endl; }
    void method2 (int n) { std::cout << "method2 invoked with argument: " << n << std::endl; }
    void method3 (int n) { std::cout << "method3 invoked with argument: " << n << std::endl; }
    void method1 (const std::string& str) { std::cout << "method1 invoked with argument: " << str << std::endl; }
    void method2 (const std::string& str) { std::cout << "method2 invoked with argument: " << str << std::endl; }
    void method3 (const std::string& str) { std::cout << "method3 invoked with argument: " << str << std::endl; }
    OUT_METAINFO(A)
};

int A::static_num = 2;

METAINFO(A)
REFLECT_OBJ_VAR(str)
REFLECT_STATIC_VAR(static_num)
REFLECT_OBJ_MTD(num_accessor)
REFLECT_OBJ_MTD(num2_accessor)
REFLECT_CONST_OBJ_MTD(const_method)
REFLECT_STATIC_MTD(static_method,std::size_t,const std::string&)
REFLECT_OBJ_MTD(method1,int)
REFLECT_OBJ_MTD(method1,const std::string&) //Warning: Indexing of elements depends on the order of calling macroses
REFLECT_OBJ_MTD(method2,int)
REFLECT_OBJ_MTD(method2,const std::string&)
REFLECT_OBJ_MTD(method3,int)
REFLECT_OBJ_MTD(method3,const std::string&)
END_METAINFO

int main() {
    //To check whether you can call method with given args types you can use check_invoke
    std::cout << std::boolalpha << "Can be invoked: " << couscous::check_invoke<A,couscous::ObjVars,A&>(hana::size_c<0>).value()  << std::endl;
    std::cout << "Can be invoked: " << couscous::check_invoke<A,couscous::ObjVars,const A&>(hana::size_c<0>).value()  << std::endl;
    std::cout << "Can be invoked: " << couscous::check_invoke<A,couscous::ObjMethods,A&,int>(hana::size_c<3>).value() << std::endl; //third index: method1 with int
    //You can't call non const obj methods with const object
    std::cout << "Can be invoked: " << couscous::check_invoke<A,couscous::ObjMethods,const A&,int>(hana::size_c<3>).value() << std::endl;
    std::cout << "Can be invoked: " << couscous::check_invoke<A,couscous::ConstMethods,const A&>(hana::size_c<0>).value() << std::endl;
    //You can call static vars/methods with or without object parameter
    std::cout << "Can be invoked: " << couscous::check_invoke<A,couscous::StaticVars,A&>(hana::size_c<0>).value() << std::endl;
    std::cout << "Can be invoked: " << couscous::check_invoke<A,couscous::StaticVars>(hana::size_c<0>).value() << std::endl;
    //Implicit conversion are also supported
    std::cout << "Can be invoked: " << couscous::check_invoke<A,couscous::StaticMethods,A&,int,const std::string&>(hana::size_c<0>).value() << std::endl;
    std::cout << "Can be invoked: " << couscous::check_invoke<A,couscous::StaticMethods,unsigned,const std::string&>(hana::size_c<0>).value() << std::endl;
    //To check whether you can assign to variable/method you can use check_set <Type,Generator,RHS type,invokation types>
    std::cout << "Can be assigned: " << couscous::check_set<A,couscous::ObjVars,const char*,A&>(hana::size_c<0>).value() << std::endl;
    std::cout << "Can be assigned: " << couscous::check_set<A,couscous::StaticVars,long>(hana::size_c<0>).value() << std::endl;
    std::cout << "Can be assigned: " << couscous::check_set<A,couscous::ObjMethods,int&&,A>(hana::size_c<0>).value() << std::endl;
    hana::for_each(couscous::find_by_name<A,couscous::ObjMethods>(HANA_STR("method1")).value(), [](auto&& index) {
        std::cout << "Can be invoked: " << couscous::check_invoke<A,couscous::ObjMethods,A&,int>(index).value()
                  << "\tCan be invoked: " << couscous::check_invoke<A,couscous::ObjMethods,A&,const std::string&>(index).value() << std::endl;
    });
    hana::for_each(hana::tuple_c<std::size_t,0,1>, [](auto&& index) {
        std::cout << "Can be assigned: " << couscous::check_set<A,couscous::ObjMethods,int&&,A&>(index).value() << std::endl;
    });
    A obj;
    //To get var/result value of a method you can use get
    std::cout << hana::to<const char*>(couscous::member_name<A,couscous::ObjVars>(hana::size_c<0>).value()) << ": "
              << couscous::get<A,couscous::ObjVars>(hana::size_c<0>,obj).value() << std::endl;
    std::cout << hana::to<const char*>(couscous::member_name<A,couscous::StaticVars>(hana::size_c<0>).value()) << ": "
              << couscous::get<A,couscous::StaticVars>(hana::size_c<0>).value() << std::endl;
    std::cout << "num_accessor: " << couscous::get<A,couscous::ObjMethods>(couscous::find_by_name<A,couscous::ObjMethods>(HANA_STR("num_accessor")).value()[0_c],obj).value() << std::endl;
    std::cout << "num2_accessor: " << couscous::get<A,couscous::ObjMethods>(couscous::find_by_name<A,couscous::ObjMethods>(HANA_STR("num2_accessor")).value()[0_c],obj).value() << std::endl;
    //You can use set function if you want to assign a variable/method
    couscous::set<A,couscous::ObjVars>(hana::size_c<0>,"New string",obj);
    std::cout << hana::to<const char*>(couscous::member_name<A,couscous::ObjVars>(hana::size_c<0>).value()) << ": "
              << couscous::get<A,couscous::ObjVars>(hana::size_c<0>,obj).value() << std::endl;
    couscous::set<A,couscous::StaticVars>(hana::size_c<0>,100);
    std::cout << hana::to<const char*>(couscous::member_name<A,couscous::StaticVars>(hana::size_c<0>).value()) << ": "
              << couscous::get<A,couscous::StaticVars>(hana::size_c<0>).value() << std::endl;
    couscous::set<A,couscous::ObjMethods>(couscous::find_by_name<A,couscous::ObjMethods>(HANA_STR("num_accessor")).value()[0_c],200,obj);
    std::cout << "num_accessor: " << couscous::get<A,couscous::ObjMethods>(couscous::find_by_name<A,couscous::ObjMethods>(HANA_STR("num_accessor")).value()[0_c],obj).value() << std::endl;
    //Ideal couscous provides it's own for each implementation. Differences: you can pass aditional args to lambda function; will fold all result values in tuple
    auto tup = couscous::for_each([](auto&& x, auto&& offset) {
        return x + offset;
    },hana::tuple_c<std::size_t,1,2,3>,hana::size_c<5>);
    std::cout << (tup == hana::tuple_c<std::size_t,6,7,8>) << std::endl;
    //To concat tuples in one you can use boost::hana::concat (which will concat only two tuples) or multiple_concat (for multiple tuples)
    constexpr auto all_methods_indices = couscous::multiple_concat(couscous::find_by_name<A,couscous::ObjMethods>(HANA_STR("method1")).value(),
                                                                   couscous::find_by_name<A,couscous::ObjMethods>(HANA_STR("method2")).value(),
                                                                   couscous::find_by_name<A,couscous::ObjMethods>(HANA_STR("method3")).value());
    auto comp_indices = couscous::for_each([](auto&& index) {
        if constexpr (std::decay_t<decltype(couscous::check_invoke<A,couscous::ObjMethods,A&,int>(index).value())>::value) {
            return index;
        }
    },all_methods_indices);
    constexpr auto accessor_inds = hana::concat(couscous::find_by_name<A,couscous::ObjMethods>(HANA_STR("num_accessor")).value(),
                                                couscous::find_by_name<A,couscous::ObjMethods>(HANA_STR("num2_accessor")).value());
    //To get multiple vars/methods you can use gets or gets_tuple_args
    //Call boost::hana::concat to concat two tuples in one
    auto nums_tup = couscous::gets<A,couscous::ObjMethods>(accessor_inds,obj).value();
    std::cout << "Accessors gets: ";
    hana::for_each(nums_tup,[](auto&& x) {
        std::cout << x << '\t';
    });
    std::cout << std::endl;
    //You can also call procedures(void return type) as well
    couscous::gets<A,couscous::ObjMethods>(comp_indices,obj,-100);
    auto params = hana::make_tuple(hana::make_tuple(std::ref(obj),42),hana::make_tuple(std::ref(obj),"argument1"),
                                   hana::make_tuple(std::ref(obj),666),hana::make_tuple(std::ref(obj),"argument2"),
                                   hana::make_tuple(std::ref(obj),1337),hana::make_tuple(std::ref(obj),"argument3"));
    couscous::gets_tuple_args<A,couscous::ObjMethods>(all_methods_indices,params);
    //For multiple assigns: sets or sets_tuple_args
    couscous::sets<A,couscous::ObjMethods>(hana::concat(couscous::find_by_name<A,couscous::ObjMethods>(HANA_STR("num_accessor")).value(),
                                                        couscous::find_by_name<A,couscous::ObjMethods>(HANA_STR("num2_accessor")).value()),1000,obj);
    std::cout << "num_accessor: " << obj.num_accessor()  << "\tnum2_accessor: " << obj.num2_accessor() << std::endl;
    couscous::sets_tuple_args<A,couscous::ObjMethods>(accessor_inds,hana::make_tuple(100,200),hana::make_tuple(hana::make_tuple(std::ref(obj)),hana::make_tuple(std::ref(obj))));
    std::cout << "num_accessor: " << obj.num_accessor()  << "\tnum2_accessor: " << obj.num2_accessor() << std::endl;
}
