#include <iostream>
#include <boost/hana.hpp>
#include <array>
#include "meta_utils.hpp"
#include <cstring>

struct Person {
    int age;
    std::string name;
    void hello () {
        std::cout << "Person Hello\n";
    }
    void hello (int& a) {
        std::cout << "Person Hello " << a << std::endl;
    }

    constexpr int test () {
        return 13;
    }
    METACLASS_DEFENITION(Person)
    REFLECT_VARIABLE(age)
    REFLECT_VARIABLE(name)
    REFLECT_METHOD(hello)
    REFLECT_METHOD(hello,int&)
    REFLECT_METHOD(test)

};

struct Person2 {
    int age;
    std::string name;
    Person test2;
    void hello () {
        std::cout << "Person2 Hello\n";
    }
    void hello (int a) {
        std::cout << "Person2 Hello " << a << std::endl;
    }
    void hello (std::string a, int&& b) {
        std::cout << "Person2 Hello " << a << b << std::endl;
    }
    METACLASS_DEFENITION(Person2)
    REFLECT_VARIABLE(age)
    REFLECT_VARIABLE(name)
    REFLECT_VARIABLE(test2)
    REFLECT_METHOD(hello)
    REFLECT_METHOD(hello,int)
    REFLECT_METHOD(hello,std::string,int&&)
    //REFLECT_METHOD(hello,double)
};

template <class T>
struct Templ_class {
    T t;
    METACLASS_DEFENITION(Templ_class)
    REFLECT_VARIABLE(t)
};

namespace hana = boost::hana;
using namespace hana::literals;

template <class T, typename std::enable_if_t<is_reflected_v<T>,bool> = 0>
void print_metadata (T& obj) {
    std::cout << "Class name: " << MetaClass<T>::name << std::endl;
    std:: cout << "Class vars names: ";
    utils::print_hana_string(MetaClass<T>::vars_names,std::cout);
    std::cout << std::endl;
    std::cout << "Class vars: " << obj << std::endl;
    std::cout << "Methods names: ";
    utils::print_hana_string(MetaClass<T>::methods_names,std::cout);
    std::cout << std::endl;
}

template <class T,typename std::enable_if_t<!is_reflected_v<T>,bool> = 0>
void print_metadata (T&) {
    std::cout << "Class: " << typeid(T).name() << " has no metadata!\n";
}

template <class T>
constexpr decltype(auto) make_type (const T& value) {
    return std::make_pair (typeid(value).hash_code(),hana::type_c<T>);
}

template<class Obj,class T, class Char, class Traits>
decltype(auto) find_var(T&& t, std::basic_ostream<Char,Traits>& out) {
    auto vals = reflect::find_variable_name<Obj>(t);
    if (boost::hana::size(vals) == boost::hana::size_c<0>)
        out << "No member named: " << boost::hana::to<const char*>(t) << " found!\n";
    else {
        out << "Found member: " << boost::hana::to<const char*>(t) << " at index: ";
        utils::print(vals,out);
        out << std::endl;
    }
    return out;
}

template<class Obj,class T, class Char, class Traits>
decltype(auto) find_method(T&& t, std::basic_ostream<Char,Traits>& out) {
    auto vals = reflect::find_method_name<Obj>(t);
    if (boost::hana::size(vals) == boost::hana::size_c<0>)
        out << "No method named: " << boost::hana::to<const char*>(t) << " found!\n";
    else {
        out << "Found method: " << boost::hana::to<const char*>(t) << " at indices: ";
        utils::print(vals,out);
        out << std::endl;
        out << "Method types of: " << boost::hana::to<const char*>(t) << ":\t";
        utils::for_each_tuple_args([](auto&& value, auto& out) {
            auto type_tup = (boost::hana::at(MetaClass<Obj>::methods_metadata,value)).args_type;
            std::cout << value << ": ";
            utils::for_each_tuple_args([](auto&& value, auto& out) {
                out << typeid(typename std::decay_t<decltype(value)>::type).name() << " ";
            },type_tup,out);
        },vals,out);
        out << std::endl;
    }
    return out;
}

int main() {
    find_var<Person>(HANA_STR("age"),std::cout);
    find_var<Person>(HANA_STR("nothing"),std::cout);
    find_method<Person2>(HANA_STR("hello"),std::cout);
    find_method<Person2>(HANA_STR("nothing"),std::cout);
    std::cout << std::endl;
    Person p {13,"str"};
    Person2 p2 {15,"str2", {16,"str3"}};
    Templ_class<int> templ {1234};
    Templ_class<std::string> templ2 {"str_test"};
    print_metadata (p);
    std::cout << std::endl;
    print_metadata (p2);
    std::cout << std::endl;
    print_metadata (templ);
    std::cout << std::endl;
    print_metadata (templ2);
    std::cout << std::endl;
    int i;
    print_metadata (i);
    std::cout << std::endl;
    std::cout << "Methods call:\n";
    reflect::emit(p,hana::size_t<0>());
    i = 1337;
    reflect::emit(p,hana::size_t<1>(),i);
    reflect::emit(p2,hana::size_t<0>());
    reflect::emit(p2,hana::size_t<1>(),42);
    constexpr auto i2 = reflect::emit(p,hana::size_t<2>());
    std::cout << "Result value of method: " << i2  << std::endl;
    return 0;
}
