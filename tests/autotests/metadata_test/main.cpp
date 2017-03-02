#include <reflect.hpp>
#include <iostream>
#include <QtTest>

class MetadataTest : public QObject {
    Q_OBJECT
    int var1,var2;
    static int static_var;
    void foo (int);
    int foo (int,double);
    static double static_foo(int,double, const std::string&);
public:
    using Type = MetadataTest;
    OUT_OF_CLASS_META_INFO(MetadataTest)
private slots:
    void class_name_method();
    void vars_obj_names();
    void vars_static_names();
    void vars_names();
    void method_obj_names();
    void method_static_names();
    void method_names();
    void counter_obj_vars();
    void counter_static_vars();
    void counter_vars();
    void counter_obj_method();
    void counter_static_method();
    void counter_method();
    void find_obj_vars();
    void find_static_vars();
    void find_vars();
    void find_obj_methods();
    void find_static_methods();
    void find_methods();
    void get_obj_method_types();
    void get_static_method_types();
    void get_method_types();
    void check_obj_invoke_method();
    void check_static_invoke_method();
    void check_invoke_method();
};

META_INFO(MetadataTest)
REFLECT_OBJ_VARIABLE(var1)
REFLECT_OBJ_VARIABLE(var2)
REFLECT_STATIC_VARIABLE(static_var)
END_META_INFO

void MetadataTest::class_name_method() {
    QVERIFY(reflect::utils::get_class_name<Type>() == HANA_STR("MetadataTest"));
}
void MetadataTest::vars_obj_names() {
    auto bool_tup = reflect::metautils::for_each([this](auto&& index, auto&& tuple) {
        return (reflect::utils::get_name<Type,reflect::ObjVars>(index) == boost::hana::at(tuple,index));
    },reflect::metautils::generate_tuple_indices<decltype(reflect::utils::get_count<Type,reflect::ObjVars>())>(),
    boost::hana::make_tuple(HANA_STR("var1"),HANA_STR("var2")));
    QVERIFY (bool_tup == boost::hana::make_tuple(true,true));
}

void MetadataTest::vars_static_names() {
    auto bool_tup = reflect::metautils::for_each([this](auto&& index, auto&& tuple) {
        return (reflect::utils::get_name<Type,reflect::StaticVars>(index) == boost::hana::at(tuple,index));
    },reflect::metautils::generate_tuple_indices<decltype(reflect::utils::get_count<Type,reflect::StaticVars>())>(),
    boost::hana::make_tuple(HANA_STR("static_var")));
    QVERIFY (bool_tup == boost::hana::make_tuple(true));
}

void MetadataTest::vars_names() {
    auto bool_tup = reflect::metautils::for_each([this](auto&& index, auto&& tuple) {
        return (reflect::utils::get_name<Type,reflect::AllVars>(index) == boost::hana::at(tuple,index));
    },reflect::metautils::generate_tuple_indices<decltype(reflect::utils::get_count<Type,reflect::AllVars>())>(),
    boost::hana::make_tuple(HANA_STR("var1"),HANA_STR("var2"),HANA_STR("static_var")));
    QVERIFY (bool_tup == boost::hana::make_tuple(true,true,true));
}

void MetadataTest::method_obj_names() {
    QVERIFY(false);
}

void MetadataTest::method_static_names() {
    QVERIFY(false);
}

void MetadataTest::method_names() {
    QVERIFY(false);
//    QCOMPARE(reflect::get_method_name<Type>(::boost::hana::size_c<0>),HANA_STR("foo"));
//    QCOMPARE(reflect::get_method_name<Type>(::boost::hana::size_c<1>),HANA_STR("foo"));
//    QCOMPARE(reflect::get_method_name<Type>(::boost::hana::size_c<2>),HANA_STR("static_foo"));
}

void MetadataTest::counter_obj_vars() {
    QVERIFY((reflect::utils::get_count<Type,reflect::ObjVars>()) == boost::hana::size_c<2>);
}

void MetadataTest::counter_static_vars() {
    QVERIFY((reflect::utils::get_count<Type,reflect::StaticVars>()) == boost::hana::size_c<1>);
}

void MetadataTest::counter_vars() {
    QVERIFY((reflect::utils::get_count<Type,reflect::AllVars>()) == boost::hana::size_c<3>);
}

void MetadataTest::counter_obj_method() {
    auto tuple = reflect::metautils::for_each([this](auto&& name) {
        return reflect::utils::find_name<Type,reflect::ObjVars>(name);
    },boost::hana::make_tuple(HANA_STR("var1"),HANA_STR("var2")));
}

void MetadataTest::counter_static_method() {
    QVERIFY(false);
//    QVERIFY(reflect::get_methods_static_count<Type>() == 1);
}

void MetadataTest::counter_method() {
    QVERIFY(false);
//    QVERIFY(reflect::get_methods_count<Type>() == 3);
}

void MetadataTest::find_obj_vars() {
    QVERIFY(false);
//    QCOMPARE(reflect::find_obj_variable_index<Type>(HANA_STR("var1")),boost::hana::size_c<0>);
//    QCOMPARE(reflect::find_obj_variable_index<Type>(HANA_STR("var2")),boost::hana::size_c<1>);
//    QCOMPARE(reflect::find_obj_variable_index<Type>(HANA_STR("static_var")),boost::hana::nothing);
}

void MetadataTest::find_static_vars() {
    QVERIFY(false);
//    QCOMPARE(reflect::find_static_variable_index<Type>(HANA_STR("var1")),boost::hana::nothing);
//    QCOMPARE(reflect::find_static_variable_index<Type>(HANA_STR("var2")),boost::hana::nothing);
//    QCOMPARE(reflect::find_static_variable_index<Type>(HANA_STR("static_var")),boost::hana::size_c<0>);
}

void MetadataTest::find_vars() {
    QVERIFY(false);
//    QCOMPARE(reflect::find_variable_index<Type>(HANA_STR("var1")),boost::hana::size_c<0>);
//    QCOMPARE(reflect::find_variable_index<Type>(HANA_STR("var2")),boost::hana::size_c<1>);
//    QCOMPARE(reflect::find_variable_index<Type>(HANA_STR("nothing")),boost::hana::nothing);
}

void MetadataTest::find_obj_methods() {
    QVERIFY(false);
//    QVERIFY(reflect::find_obj_method_index<Type>(HANA_STR("foo")) == (boost::hana::tuple_c<std::size_t,0,1>));
//    QVERIFY(reflect::find_obj_method_index<Type>(HANA_STR("static_foo")) == boost::hana::nothing);
//    QVERIFY(reflect::find_obj_method_index<Type>(HANA_STR("nothing")) == boost::hana::nothing);
}

void MetadataTest::find_static_methods() {
    QVERIFY(false);
//    QVERIFY(reflect::find_static_method_index<Type>(HANA_STR("foo")) == boost::hana::nothing);
//    QVERIFY(reflect::find_static_method_index<Type>(HANA_STR("static_foo")) == (boost::hana::tuple_c<std::size_t,0>));
//    QVERIFY(reflect::find_static_method_index<Type>(HANA_STR("nothing")) == boost::hana::nothing);
}

void MetadataTest::find_methods() {
    QVERIFY(false);
//    QVERIFY(reflect::find_method_index<Type>(HANA_STR("foo")) == (boost::hana::tuple_c<std::size_t,0,1>));
//    QVERIFY(reflect::find_method_index<Type>(HANA_STR("static_foo")) == (boost::hana::tuple_c<std::size_t,2>));
//    QVERIFY(reflect::find_method_index<Type>(HANA_STR("nothing")) == boost::hana::nothing);
}

void MetadataTest::get_obj_method_types() {
    QVERIFY(false);
}

void MetadataTest::get_static_method_types() {
    QVERIFY(false);
}

void MetadataTest::get_method_types() {
    QVERIFY(false);
//    QVERIFY (reflect::get_method_arg_types<Type>(::boost::hana::size_c<0>) == ::boost::hana::tuple_t<int>);
//    QVERIFY (reflect::get_method_arg_types<Type>(::boost::hana::size_c<1>) == (::boost::hana::tuple_t<int,double>));
//    QVERIFY (reflect::get_method_arg_types<Type>(::boost::hana::size_c<2>) == (::boost::hana::tuple_t<int,double,const std::string&>));
//    QCOMPARE(typeid(reflect::method_arg_type_t<Type,0,0>),typeid(int));
//    QCOMPARE(typeid(reflect::method_arg_type_t<Type,1,0>),typeid(int));
//    QCOMPARE(typeid(reflect::method_arg_type_t<Type,1,1>),typeid(double));
//    QCOMPARE(typeid(reflect::method_arg_type_t<Type,2,0>),typeid(int));
//    QCOMPARE(typeid(reflect::method_arg_type_t<Type,2,1>),typeid(double));
//    QCOMPARE(typeid(reflect::method_arg_type_t<Type,2,2>),typeid(const std::string&));
//    QCOMPARE(typeid(reflect::method_arg_type_t<Type,0,0>),typeid(std::decay_t<decltype(reflect::method_arg_type<Type>(::boost::hana::size_c<0>,::boost::hana::size_c<0>))>::type));
//    QCOMPARE(typeid(reflect::method_return_type_t<Type,0>),typeid(void));
//    QCOMPARE(typeid(reflect::method_return_type_t<Type,1>),typeid(int));
//    QCOMPARE(typeid(reflect::method_return_type_t<Type,2>),typeid(double));
}

void MetadataTest::check_obj_invoke_method() {
    QVERIFY(false);
//    QCOMPARE(reflect::check_invoke_obj_method<Type>(boost::hana::nothing),boost::hana::nothing);
//    QCOMPARE((reflect::check_invoke_obj_method<Type,int>(reflect::find_obj_method_index<Type>(HANA_STR("foo")))), ::boost::hana::make_tuple(true,false));
//    QCOMPARE((reflect::check_invoke_obj_method<Type,int,double>(reflect::find_obj_method_index<Type>(HANA_STR("foo")))), ::boost::hana::make_tuple(false,true));
//    QCOMPARE((reflect::check_invoke_obj_method<Type,int&&,double,std::string>(reflect::find_obj_method_index<Type>(HANA_STR("static_foo")))), boost::hana::nothing);
}

void MetadataTest::check_static_invoke_method() {
    QVERIFY(false);
    //QCOMPARE(reflect::check_invoke_static_method<Type>(boost::hana::nothing),boost::hana::nothing);
    //QCOMPARE((reflect::check_invoke_static_method<Type,int>(reflect::find_static_method_index<Type>(HANA_STR("foo")))), boost::hana::nothing);
    //QCOMPARE((reflect::check_invoke_static_method<Type,int,double>(reflect::find_static_method_index<Type>(HANA_STR("foo")))), boost::hana::nothing);
    //QCOMPARE((reflect::check_invoke_static_method<Type,int&&,double,std::string>(reflect::find_static_method_index<Type>(HANA_STR("static_foo")))), ::boost::hana::make_tuple(true));
}

void MetadataTest::check_invoke_method() {
    QVERIFY(false);
    //QCOMPARE(reflect::check_invoke_method<Type>(boost::hana::nothing),boost::hana::nothing);
    //QCOMPARE((reflect::check_invoke_method<Type,int>(reflect::find_method_index<Type>(HANA_STR("foo")))), ::boost::hana::make_tuple(true,false));
    //QCOMPARE((reflect::check_invoke_method<Type,int,double>(reflect::find_method_index<Type>(HANA_STR("foo")))), ::boost::hana::make_tuple(false,true));
    //QCOMPARE((reflect::check_invoke_method<Type,int&&,double,std::string>(reflect::find_method_index<Type>(HANA_STR("static_foo")))), ::boost::hana::make_tuple(true));
}

QTEST_MAIN(MetadataTest)
#include "main.moc"
