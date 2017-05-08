#include <iostream>

#define STRING_MAXLEN 128                                            //Max length of ct-string(also this parameter specifies the max size of the name of the vars/methods)
                                                                     //Default value: 64 (if STRING_MAXLEN is not defined). It's important to define it BEFORE include reflection headers
#include <reflect.hpp>                                               //Main ideal couscous header

namespace couscous {
inline namespace utils {using namespace ::reflect::utils; }          //Namespace related to all reflect functions
inline namespace metautils { using namespace ::reflect::metautils; } //Namespace related to all metaprogramming functions
inline namespace reflect {using namespace ::reflect; }               //ideal couscous namespace
}

namespace hana = boost::hana;

struct InStructReflect {
    int var;
    void foo();
    void foo(int);
    static int bar(double);
    IN_METAINFO(InStructReflect)     //In class meta information(also it means that struct is reflected)
    REFLECT_OBJ_VAR(var)            //Reflect object variable
    REFLECT_OBJ_MTD(foo)            //Reflect object method (first param - method name, second - method input types(nothing if method doesn't require input args))
    REFLECT_OBJ_MTD(foo,int)        //Overloaded method can be easily reflected too
    REFLECT_STATIC_MTD(bar,double)  //Reflect static method
};
//Note: You can easily reflect method prototypes and use all reflection functions which don't call these methods directly
//Warning: volatile vars can't be reflected

struct OutStructReflect {
    static std::string str;
    auto baz() const {
        return 10;
    }
    OUT_METAINFO(OutStructReflect)   //Out of class meta information
};
//Note: The same rules works with static vars (You can reflect them even if they still uninitialized)

METAINFO(OutStructReflect)          //Metainformation for OutStructReflect
REFLECT_STATIC_VAR(str)             //Reflect static var
REFLECT_CONST_OBJ_MTD(baz)          //Reflect const method
END_METAINFO                        //End of OutStructReflect metainformation
//Note: Out of class meta information must be used when you have methods with trailing return type

struct NotReflected;                //Not reflected struct

template <class T>
struct InTemplateStruct {
    T var;
    T foo();
    IN_METAINFO(InTemplateStruct)   //Template class/struct can also be reflected
    REFLECT_OBJ_VAR(var)
    REFLECT_OBJ_MTD(foo)
};

template <class... Args>
struct OutTemplateStruct {
    int var;
    void foo(Args&&...);
    OUT_METAINFO(OutTemplateStruct)
};

TEMPLATE_METAINFO(OutTemplateStruct,class... Args,Args...)  //Second parameter needed to define all templetes which will be used; third parameter - spetialization for metainformation
REFLECT_OBJ_MTD(foo,Args&&...)
END_METAINFO

TEMPLATE_METAINFO(OutTemplateStruct,,int)                  //Spetialize Metainformation for OutTemplateStruct<int>
REFLECT_OBJ_VAR(var)
END_METAINFO

int main () {
    std::cout << std::boolalpha << "InStructReflect reflected: " << couscous::check_reflected<InStructReflect>() << "\tOutStructReflect reflected: " << couscous::check_reflected<OutStructReflect>()
              << "\nNotReflected reflected: "<< couscous::check_reflected<NotReflected>() << "\tint reflected: " << couscous::check_reflected<int>() << std::endl; //Check if type is reflected(returns boost::hana::bool_c<true/false>
    constexpr auto something = couscous::class_name<InStructReflect>(); // class_name returns boost::hana::optional of ct-string if type reflected, otherwise - boost::hana::nothing
    std::cout << "Optional contains something: " << hana::is_just(something) << "\tOptional is empty: " << hana::is_nothing(something) << std::endl;
    constexpr auto nothing = couscous::class_name<NotReflected>();
    std::cout << "Optional contains something: " << hana::is_just(nothing) << "\tOptional is empty: " << hana::is_nothing(nothing) << std::endl;
    std::cout << "Class name: " << hana::to<const char*>(something.value()) << std::endl; //get ct-string from optional and convert to const char*
    constexpr auto optional_char_str = hana::transform(something,hana::to<const char*>); //Other way to "interact" with optionals is hana::transform(now it returns optional of const char*)
    constexpr auto still_nothing = hana::transform(nothing,hana::to<const char*>); //Will return boost::hana::nothing
    std::cout << "Class name: " << optional_char_str.value() << std::endl;
    std::cout << "Optional is empty: " << hana::is_nothing(still_nothing) << std::endl;
    //Generators: responsible to select scope of metainformation(all,vars/methods,obj vars/methods,static vars/methods, const methods)
    std::cout << "Count of all methods: " << couscous::get_opt_val(couscous::count<InStructReflect,couscous::AllMethods>())
              << "\tCount of static variables: "  << couscous::get_opt_val(couscous::count<OutStructReflect,couscous::StaticVars>()) << std::endl; //count function returns size of selected generator(AllMethods,StaticVars)
    //get_opt_val(t) - returns: t.value() if t - is_just == true, otherwise: t
    //Metainformation spetialization example
    std::cout << "Count of all methods: " << couscous::get_opt_val(couscous::count<OutTemplateStruct<std::string>,couscous::AllMethods>()) << std::endl;
    std::cout << "Count of all variables: " << couscous::get_opt_val(couscous::count<OutTemplateStruct<int>,couscous::AllVars>()) << std::endl;
    return 0;
}
