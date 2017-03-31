#include <reflect.hpp>
#include <iostream>
#include <QtTest>

class MetadataTest : public QObject {
    Q_OBJECT
    int var1,var2;
    static int static_var;
    void foo (int);
    int foo (int,double) const;
    static double static_foo(int,double, const std::string&);
    using Type = MetadataTest;
public:
    OUT_METAINFO(MetadataTest)
private slots:
    void class_name_method();
    void vars_obj_names();
    void vars_static_names();
    void vars_names();
    void method_obj_names();
    void method_const_names();
    void method_static_names();
    void method_names();
    void counter_obj_vars();
    void counter_static_vars();
    void counter_vars();
    void counter_obj_method();
    void counter_const_method();
    void counter_static_method();
    void counter_method();
    void find_obj_vars();
    void find_static_vars();
    void find_vars();
    void find_obj_methods();
    void find_const_methods();
    void find_static_methods();
    void find_methods();
    void get_obj_method_types();
    void get_const_method_types();
    void get_static_method_types();
    void get_method_types();
    void check_obj_invoke_method();
    void check_const_invoke_method();
    void check_static_invoke_method();
    void check_invoke_method();
};

METAINFO(MetadataTest)
REFLECT_OBJ_VAR(var1)
REFLECT_OBJ_VAR(var2)
REFLECT_STATIC_VAR(static_var)
REFLECT_OBJ_MET(foo,int)
REFLECT_CONST_OBJ_MET(foo,int,double)
REFLECT_STATIC_MET(static_foo,int,double,const std::string&)
END_METAINFO

void MetadataTest::class_name_method() {
    QVERIFY(reflect::utils::get_class_name<Type>() == HANA_STR("MetadataTest"));
    QVERIFY(reflect::utils::get_class_name<int>() == boost::hana::nothing);
}
void MetadataTest::vars_obj_names() {
    auto bool_tup = reflect::metautils::for_each([this](auto&& index, auto&& tuple) {
        return (reflect::utils::get_name<Type,reflect::ObjVars>(index) == boost::hana::at(tuple,index));
    },reflect::metautils::generate_tuple_indices<decltype(reflect::utils::get_count<Type,reflect::ObjVars>())>(),
    boost::hana::make_tuple(HANA_STR("var1"),HANA_STR("var2")));
    QVERIFY (bool_tup == boost::hana::make_tuple(true,true));
    QVERIFY((reflect::utils::get_name<Type,reflect::ObjVars>(reflect::utils::get_count<Type,reflect::ObjVars>())) == boost::hana::nothing);
    QVERIFY((reflect::utils::get_name<Type,int>(boost::hana::size_c<0>)) == boost::hana::nothing);
}

void MetadataTest::vars_static_names() {
    auto bool_tup = reflect::metautils::for_each([this](auto&& index, auto&& tuple) {
        return (reflect::utils::get_name<Type,reflect::StaticVars>(index) == boost::hana::at(tuple,index));
    },reflect::metautils::generate_tuple_indices<decltype(reflect::utils::get_count<Type,reflect::StaticVars>())>(),
    boost::hana::make_tuple(HANA_STR("static_var")));
    QVERIFY (bool_tup == boost::hana::make_tuple(true));
    QVERIFY((reflect::utils::get_name<Type,reflect::StaticVars>(reflect::utils::get_count<Type,reflect::StaticVars>())) == boost::hana::nothing);
}

void MetadataTest::vars_names() {
    auto bool_tup = reflect::metautils::for_each([this](auto&& index, auto&& tuple) {
        return (reflect::utils::get_name<Type,reflect::AllVars>(index) == boost::hana::at(tuple,index));
    },reflect::metautils::generate_tuple_indices<decltype(reflect::utils::get_count<Type,reflect::AllVars>())>(),
    boost::hana::make_tuple(HANA_STR("var1"),HANA_STR("var2"),HANA_STR("static_var")));
    QVERIFY (bool_tup == boost::hana::make_tuple(true,true,true));
    QVERIFY((reflect::utils::get_name<Type,reflect::AllVars>(reflect::utils::get_count<Type,reflect::AllVars>())) == boost::hana::nothing);
}

void MetadataTest::method_obj_names() {
    auto bool_tup = reflect::metautils::for_each([this](auto&& index, auto&& tuple) {
        return (reflect::utils::get_name<Type,reflect::ObjMethods>(index) == boost::hana::at(tuple,index));
    },reflect::metautils::generate_tuple_indices<decltype(reflect::utils::get_count<Type,reflect::ObjMethods>())>(),
    boost::hana::make_tuple(HANA_STR("foo"),HANA_STR("foo")));
    QVERIFY (bool_tup == boost::hana::make_tuple(true,true));
    QVERIFY((reflect::utils::get_name<Type,reflect::ObjMethods>(reflect::utils::get_count<Type,reflect::ObjMethods>())) == boost::hana::nothing);
}

void MetadataTest::method_const_names() {
    auto bool_tup = reflect::metautils::for_each([this](auto&& index, auto&& tuple) {
        return (reflect::utils::get_name<Type,reflect::ConstMethods>(index) == boost::hana::at(tuple,index));
    },reflect::metautils::generate_tuple_indices<decltype(reflect::utils::get_count<Type,reflect::ConstMethods>())>(),
    boost::hana::make_tuple(HANA_STR("foo")));
    QVERIFY (bool_tup == boost::hana::make_tuple(true));
    QVERIFY((reflect::utils::get_name<Type,reflect::ConstMethods>(reflect::utils::get_count<Type,reflect::ConstMethods>())) == boost::hana::nothing);
}

void MetadataTest::method_static_names() {
    auto bool_tup = reflect::metautils::for_each([this](auto&& index, auto&& tuple) {
        return (reflect::utils::get_name<Type,reflect::StaticMethods>(index) == boost::hana::at(tuple,index));
    },reflect::metautils::generate_tuple_indices<decltype(reflect::utils::get_count<Type,reflect::StaticMethods>())>(),
    boost::hana::make_tuple(HANA_STR("static_foo")));
    QVERIFY (bool_tup == boost::hana::make_tuple(true));
    QVERIFY((reflect::utils::get_name<Type,reflect::StaticMethods>(reflect::utils::get_count<Type,reflect::StaticMethods>())) == boost::hana::nothing);
}

void MetadataTest::method_names() {
    auto bool_tup = reflect::metautils::for_each([this](auto&& index, auto&& tuple) {
        return (reflect::utils::get_name<Type,reflect::AllMethods>(index) == boost::hana::at(tuple,index));
    },reflect::metautils::generate_tuple_indices<decltype(reflect::utils::get_count<Type,reflect::AllMethods>())>(),
    boost::hana::make_tuple(HANA_STR("foo"),HANA_STR("foo"),HANA_STR("static_foo")));
    QVERIFY (bool_tup == boost::hana::make_tuple(true,true,true));
    QVERIFY((reflect::utils::get_name<Type,reflect::AllMethods>(reflect::utils::get_count<Type,reflect::AllMethods>())) == boost::hana::nothing);
}

void MetadataTest::counter_obj_vars() {
    QVERIFY((reflect::utils::get_count<Type,reflect::ObjVars>()) == boost::hana::size_c<2>);
    QVERIFY((reflect::utils::get_count<int,reflect::ObjVars>()) == boost::hana::nothing);
    QVERIFY((reflect::utils::get_count<Type,int>()) == boost::hana::nothing);
}

void MetadataTest::counter_static_vars() {
    QVERIFY((reflect::utils::get_count<Type,reflect::StaticVars>()) == boost::hana::size_c<1>);
}

void MetadataTest::counter_vars() {
    QVERIFY((reflect::utils::get_count<Type,reflect::AllVars>()) == boost::hana::size_c<3>);
}

void MetadataTest::counter_obj_method() {
    QVERIFY((reflect::utils::get_count<Type,reflect::ObjMethods>()) == boost::hana::size_c<2>);
}

void MetadataTest::counter_const_method() {
    QVERIFY((reflect::utils::get_count<Type,reflect::ConstMethods>()) == boost::hana::size_c<1>);
}

void MetadataTest::counter_static_method() {
    QVERIFY((reflect::utils::get_count<Type,reflect::StaticMethods>()) == boost::hana::size_c<1>);
}

void MetadataTest::counter_method() {
    QVERIFY((reflect::utils::get_count<Type,reflect::AllMethods>()) == boost::hana::size_c<3>);
}

void MetadataTest::find_obj_vars() {
    auto tuple = reflect::metautils::merge_tuple_of_tuples(reflect::metautils::for_each([this](auto&& name) {
        return reflect::utils::find_name<Type,reflect::ObjVars>(name);
    },boost::hana::make_tuple(HANA_STR("var1"),HANA_STR("var2"))));
    QVERIFY(tuple == (boost::hana::tuple_c<std::size_t,0,1>));
}

void MetadataTest::find_static_vars() {
    auto tuple = reflect::metautils::merge_tuple_of_tuples(reflect::metautils::for_each([this](auto&& name) {
        return reflect::utils::find_name<Type,reflect::StaticVars>(name);
    },boost::hana::make_tuple(HANA_STR("static_var"))));
    QVERIFY(tuple == (boost::hana::tuple_c<std::size_t,0>));
}

void MetadataTest::find_vars() {
    auto tuple = reflect::metautils::merge_tuple_of_tuples(reflect::metautils::for_each([this](auto&& name) {
        return reflect::utils::find_name<Type,reflect::AllVars>(name);
    },boost::hana::make_tuple(HANA_STR("var1"),HANA_STR("var2"),HANA_STR("static_var"))));
    QVERIFY(tuple == (boost::hana::tuple_c<std::size_t,0,1,2>));
    QVERIFY((reflect::utils::find_name<Type,reflect::AllVars>(HANA_STR("nothing"))) == boost::hana::tuple<>());
    QVERIFY((reflect::utils::find_name<Type,int>(HANA_STR("var1"))) == boost::hana::nothing);
    QVERIFY((reflect::utils::find_name<int,reflect::AllVars>(HANA_STR("var1"))) == boost::hana::nothing);
}

void MetadataTest::find_obj_methods() {
    auto tuple = reflect::metautils::merge_tuple_of_tuples(reflect::metautils::for_each([this](auto&& name) {
        return reflect::utils::find_name<Type,reflect::ObjMethods>(name);
    },boost::hana::make_tuple(HANA_STR("foo"))));
    QVERIFY(tuple == (boost::hana::tuple_c<std::size_t,0,1>));

}

void MetadataTest::find_const_methods() {
    auto tuple = reflect::metautils::merge_tuple_of_tuples(reflect::metautils::for_each([this](auto&& name) {
        return reflect::utils::find_name<Type,reflect::ConstMethods>(name);
    },boost::hana::make_tuple(HANA_STR("foo"))));
    QVERIFY(tuple == (boost::hana::tuple_c<std::size_t,0>));
}

void MetadataTest::find_static_methods() {
    auto tuple = reflect::metautils::merge_tuple_of_tuples(reflect::metautils::for_each([this](auto&& name) {
        return reflect::utils::find_name<Type,reflect::StaticMethods>(name);
    },boost::hana::make_tuple(HANA_STR("static_foo"))));
    QVERIFY(tuple == (boost::hana::tuple_c<std::size_t,0>));
}

void MetadataTest::find_methods() {
    auto tuple = reflect::metautils::merge_tuple_of_tuples(reflect::metautils::for_each([this](auto&& name) {
        return reflect::utils::find_name<Type,reflect::AllMethods>(name);
    },boost::hana::make_tuple(HANA_STR("foo"),HANA_STR("static_foo"))));
    QVERIFY(tuple == (boost::hana::tuple_c<std::size_t,0,1,2>));
}

void MetadataTest::get_obj_method_types() {
    auto tuple = reflect::metautils::for_each([this](auto&& index) {
        return reflect::utils::get_method_args<Type,reflect::ObjMethods>(index);
    },reflect::metautils::generate_tuple_indices<decltype(reflect::utils::get_count<Type,reflect::ObjMethods>())>());
    QVERIFY (tuple == ::boost::hana::make_tuple(::boost::hana::tuple_t<MetadataTest,int>,::boost::hana::tuple_t<MetadataTest,int,double>));
    auto res_tuple = reflect::metautils::for_each([this](auto&& index) {
        return reflect::utils::get_method_result_type<Type,reflect::ObjMethods>(index);
    },reflect::metautils::generate_tuple_indices<decltype(reflect::utils::get_count<Type,reflect::ObjMethods>())>());
    QVERIFY (res_tuple == (::boost::hana::tuple_t<void,int>));
}

void MetadataTest::get_const_method_types() {
    auto tuple = reflect::metautils::for_each([this](auto&& index) {
        return reflect::utils::get_method_args<Type,reflect::ConstMethods>(index);
    },reflect::metautils::generate_tuple_indices<decltype(reflect::utils::get_count<Type,reflect::ConstMethods>())>());
    QVERIFY (tuple == ::boost::hana::make_tuple(::boost::hana::tuple_t<MetadataTest,int,double>));
    auto res_tuple = reflect::metautils::for_each([this](auto&& index) {
        return reflect::utils::get_method_result_type<Type,reflect::ConstMethods>(index);
    },reflect::metautils::generate_tuple_indices<decltype(reflect::utils::get_count<Type,reflect::ConstMethods>())>());
    QVERIFY (res_tuple == (::boost::hana::tuple_t<int>));

}

void MetadataTest::get_static_method_types() {
    auto tuple = reflect::metautils::for_each([this](auto&& index) {
        return reflect::utils::get_method_args<Type,reflect::StaticMethods>(index);
    },reflect::metautils::generate_tuple_indices<decltype(reflect::utils::get_count<Type,reflect::StaticMethods>())>());
    QVERIFY (tuple == ::boost::hana::make_tuple(::boost::hana::tuple_t<int,double,const std::string&>));
    auto res_tuple = reflect::metautils::for_each([this](auto&& index) {
        return reflect::utils::get_method_result_type<Type,reflect::StaticMethods>(index);
    },reflect::metautils::generate_tuple_indices<decltype(reflect::utils::get_count<Type,reflect::StaticMethods>())>());
    QVERIFY (res_tuple == (::boost::hana::tuple_t<double>));
}

void MetadataTest::get_method_types() {
    auto tuple = reflect::metautils::for_each([this](auto&& index) {
        return reflect::utils::get_method_args<Type,reflect::AllMethods>(index);
    },reflect::metautils::generate_tuple_indices<decltype(reflect::utils::get_count<Type,reflect::AllMethods>())>());
    QVERIFY (tuple == ::boost::hana::make_tuple(::boost::hana::tuple_t<MetadataTest,int>,::boost::hana::tuple_t<MetadataTest,int,double>,
                                                ::boost::hana::tuple_t<int,double,const std::string&>));
    auto res_tuple = reflect::metautils::for_each([this](auto&& index) {
        return reflect::utils::get_method_result_type<Type,reflect::AllMethods>(index);
    },reflect::metautils::generate_tuple_indices<decltype(reflect::utils::get_count<Type,reflect::AllMethods>())>());
    QVERIFY (res_tuple == (::boost::hana::tuple_t<void,int,double>));
    QCOMPARE(typeid(reflect::MethodInfo<Type,0,reflect::AllMethods>::types),typeid(::boost::hana::tuple<Type,int>));
    QCOMPARE(typeid(reflect::MethodInfo<Type,0,reflect::AllMethods>::result_type),typeid(void));
    QCOMPARE(typeid(reflect::MethodInfo<Type,0,reflect::AllMethods>::arg0_type),typeid(Type));
    QCOMPARE(typeid(reflect::MethodInfo<Type,0,reflect::AllMethods>::arg1_type),typeid(int));
    QCOMPARE(typeid(reflect::MethodInfo<Type,reflect::utils::get_count<Type,reflect::AllMethods>(),reflect::AllMethods>::types),typeid(boost::hana::nothing));
    QCOMPARE(typeid(reflect::MethodInfo<Type,reflect::utils::get_count<Type,reflect::AllMethods>(),reflect::AllMethods>::result_type),typeid(boost::hana::nothing));
    QCOMPARE(typeid(reflect::MethodInfo<Type,0,reflect::AllMethods>::arg_type<reflect::MethodInfo<Type,0,reflect::AllMethods>::args_count>),typeid(boost::hana::nothing));

}

void MetadataTest::check_obj_invoke_method() {
    QVERIFY(false);
//    QCOMPARE(reflect::check_invoke_obj_method<Type>(boost::hana::nothing),boost::hana::nothing);
//    QCOMPARE((reflect::check_invoke_obj_method<Type,int>(reflect::find_obj_method_index<Type>(HANA_STR("foo")))), ::boost::hana::make_tuple(true,false));
//    QCOMPARE((reflect::check_invoke_obj_method<Type,int,double>(reflect::find_obj_method_index<Type>(HANA_STR("foo")))), ::boost::hana::make_tuple(false,true));
//    QCOMPARE((reflect::check_invoke_obj_method<Type,int&&,double,std::string>(reflect::find_obj_method_index<Type>(HANA_STR("static_foo")))), boost::hana::nothing);
}

void MetadataTest::check_const_invoke_method() {
    QVERIFY(false);
    //QCOMPARE(reflect::check_invoke_static_method<Type>(boost::hana::nothing),boost::hana::nothing);
    //QCOMPARE((reflect::check_invoke_static_method<Type,int>(reflect::find_static_method_index<Type>(HANA_STR("foo")))), boost::hana::nothing);
    //QCOMPARE((reflect::check_invoke_static_method<Type,int,double>(reflect::find_static_method_index<Type>(HANA_STR("foo")))), boost::hana::nothing);
    //QCOMPARE((reflect::check_invoke_static_method<Type,int&&,double,std::string>(reflect::find_static_method_index<Type>(HANA_STR("static_foo")))), ::boost::hana::make_tuple(true));
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
