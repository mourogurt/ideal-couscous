#include <iostream>
#include <reflect.hpp>
#include <experimental/any>
#include <unordered_map>
#include <functional>
#include <vector>

struct Example1 {
    void method () { std::cout << "Hello from Example1\n"; }
    METACLASS_DEFENITION(Example1)
    REFLECT_METHOD(method)
};

struct Example2 {
    void method () { std::cout << "Hello from Example2\n"; }
    void another_method() { std::cout << "another method from Example2\n"; }
    void another_method(int) { std::cout << "another method (int) from Example2\n"; }
    METACLASS_DEFENITION(Example2)
    REFLECT_METHOD(method)
    REFLECT_METHOD(another_method)
    REFLECT_METHOD(another_method,int)
};

struct Example3 {
    void method () { std::cout << "Hello from Example3\n"; }
    void another_method() { std::cout << "another method from Example3\n"; }
    METACLASS_DEFENITION(Example3)
    REFLECT_METHOD(method)
    REFLECT_METHOD(another_method)
};

struct Example4 {
    void another_method() { std::cout << "another method from Example4\n"; }
    const char* string_method() {return "Example4"; }
    METACLASS_DEFENITION(Example4)
    REFLECT_METHOD(another_method)
    REFLECT_METHOD(string_method)
};

struct Example5 {
    std::string str_var;
    const char* string_method() {return str_var.c_str(); }
    METACLASS_DEFENITION(Example5)
    REFLECT_VARIABLE(str_var)
    REFLECT_METHOD(string_method)
};

struct Example6 {
    std::string str_var {"nothing"};
    void method (std::string&& var) {std::cout << std::boolalpha << (str_var == var) << " Example6\n"; }
    METACLASS_DEFENITION(Example6)
    REFLECT_VARIABLE(str_var)
    REFLECT_METHOD(method,std::string&&)
};

template <class T>
void reflection_visitor_function (T&& var) {
    if constexpr (boost::hana::size(reflect::find_variable_name<T>(HANA_STR("str_var"))) != boost::hana::size_c<0>) {
        reflect::get_variable(var,boost::hana::at_c<0>(reflect::find_variable_name<T>(HANA_STR("str_var")))) = "test";
    }
    if constexpr (boost::hana::size(reflect::find_method_name<T>(HANA_STR("method"))) != boost::hana::size_c<0>) {
        reflect::try_invoke_method(var,reflect::find_method_name<T>(HANA_STR("method")));
        reflect::try_invoke_method(var,reflect::find_method_name<T>(HANA_STR("method")),"test");
    }
    if constexpr (boost::hana::size(reflect::find_method_name<T>(HANA_STR("another_method"))) != boost::hana::size_c<0>) {
        reflect::try_invoke_method(var,reflect::find_method_name<T>(HANA_STR("another_method")));
        reflect::try_invoke_method(var,reflect::find_method_name<T>(HANA_STR("another_method")),1);
    }
    if constexpr (boost::hana::size(reflect::find_method_name<T>(HANA_STR("string_method"))) != boost::hana::size_c<0>) {
        auto tup = reflect::try_invoke_method(var,reflect::find_method_name<T>(HANA_STR("string_method")));
        std::cout << "string method: " << boost::hana::at_c<0>(tup) << std::endl;
    }
}

class runtime_type_descriptor_table {
    std::unordered_map<std::size_t,std::function<void(std::experimental::any&&)>> desc_table_map;
public:
    template <class T>
    void add_type() {
        desc_table_map.insert(std::make_pair(typeid(T).hash_code(),[](auto&& var){
            reflection_visitor_function(std::experimental::any_cast<T>(var));
        }));
    }

    bool invoke_any (std::experimental::any&& var) {
        auto iter = desc_table_map.find(var.type().hash_code());
        if (iter != desc_table_map.end()) {
            iter->second(std::forward<std::experimental::any>(var));
            return true;
        } else
            return false;
    }
};

int main() {
    runtime_type_descriptor_table table;
    table.add_type<Example1>();
    table.add_type<Example2>();
    table.add_type<Example3>();
    table.add_type<Example4>();
    table.add_type<Example5>();
    table.add_type<Example6>();
    std::vector<std::experimental::any> any_vec;
    any_vec.push_back(Example1{});
    any_vec.push_back(Example2{});
    any_vec.push_back(Example3{});
    any_vec.push_back(Example4{});
    any_vec.push_back(Example5{});
    any_vec.push_back(Example6{});
    for (auto&& item : any_vec) {
        table.invoke_any(std::forward<std::experimental::any>(item));
    }
}
