#include <iostream>
#include <reflect.hpp>
#include <boost/type_index.hpp>

struct ExampleClass {
    int var {0};
    const int const_var {1};
    static int static_var;
    void method (int a) {std::cout << "method " << a << std::endl;}
    void method (int a,std::string str) {std::cout << "method " << a << str << std::endl;}
    int  int_method () {return 1;}
    int const_method() const {return 1;}
    constexpr void constexpr_method() {}
    static void static_method() {}
    //Declaring metainformation
    METACLASS_DEFENITION(ExampleClass)
    //Reflecting variables
    REFLECT_VARIABLE(var)
    REFLECT_VARIABLE(const_var)
    REFLECT_STATIC_VARIABLE(static_var)
    REFLECT_METHOD(method,int)
    REFLECT_METHOD(method,int,std::string)
    REFLECT_METHOD(int_method)
    REFLECT_CONST_METHOD(const_method)
    REFLECT_METHOD(constexpr_method)
    REFLECT_STATIC_METHOD(static_method)

};

int ExampleClass::static_var {2};

using boost::typeindex::type_id_with_cvr;

//type and current index
template<class T, ::std::size_t I>
void print_methods_types_ind_impl () {
    //making integral constant
    constexpr auto ind = boost::hana::size_c<I>;
    //foreach (item is current index)
    reflect::utils::constexpr_foreach<reflect::get_methods_args_count<T,I>()>([&](auto&& item){
        //writing type of each variable
        std::cout << type_id_with_cvr<decltype(reflect::method_arg_type<T>(ind,item))>().pretty_name() << "\t";
        //Generating tuple of indexies from 0 to args_count - 1
    },reflect::utils::generate_tuple_indexies<reflect::get_methods_args_count<T,I>()>());
    //If no types in method
    if (reflect::get_methods_args_count<T,I>() == 0) std::cout << "No args types in method";
    std::cout << std::endl;
}

template<class T, ::std::size_t... Indices>
void print_methods_types_seq_impl (std::index_sequence<Indices...>&&) {
    //Unpacking indices
    using swallow = int[];
    (void)swallow{1,(print_methods_types_ind_impl<T,Indices>(),void(), int{})...};
}

template <class T>
void print_methods_types () {
    print_methods_types_seq_impl<T>(std::make_index_sequence<reflect::get_methods_count<T>()>());
}

template <class T>
void print_meta_information() {
    //Run only if T is reflected
    if constexpr (reflect::info::is_reflected_v<std::decay_t<T>>) {
        //Vars and methods counter
        std::cout << "Number of variables:\t" << reflect::get_variables_count<T>() << std::endl;
        std::cout << "Number of methods:\t" << reflect::get_methods_count<T>() << std::endl;
        //Class name
        std::cout << "Class name:\t" << ::boost::hana::to<const char*>(reflect::get_class_name<T>()) << std::endl;
        std::cout << "Class vars:\n";
        //Foreach (variables in class)
        reflect::utils::constexpr_foreach<reflect::get_variables_count<T>()>([&](auto&& item){
            //Write variable name, variable type
            std::cout << ::boost::hana::to<const char*>(reflect::get_variable_name<T>(item))
                      << ";\tVariable type:\t" << type_id_with_cvr<decltype(reflect::variable_type<T>(item))>().pretty_name()
                      << std::endl;
        },reflect::utils::generate_tuple_indexies<reflect::get_variables_count<T>()>());
        std::cout << std::endl;
        std::cout << "Class methods:\n";
        //Foreach methods
        reflect::utils::constexpr_foreach<reflect::get_methods_count<T>()>([](auto&& item){
            //Write method name, Arguments count and return type
            std::cout << "Name:\t" << ::boost::hana::to<const char*>(reflect::get_method_name<T>(item))
                      << ";\tArgs count:\t" << reflect::get_methods_args_count<T>(item)
                      << ";\tReturn type:\t" << type_id_with_cvr<decltype(reflect::method_return_type<T>(item))>().pretty_name()
                      << std::endl;
        },reflect::utils::generate_tuple_indexies<reflect::get_methods_count<T>()>());
        std::cout << "Methods args types:\n";
        //Write tuple args
        print_methods_types<T>();
        std::cout << std::endl;
    }
}

int main () {
    print_meta_information<ExampleClass>();
}
