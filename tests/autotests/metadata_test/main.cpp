#include <reflect.hpp>
#include <QtTest>

class MetadataTest : public QObject {
    Q_OBJECT
    int var1,var2;
    static int static_var;
    void foo (int);
    int foo (int,double);
    static double static_foo(int,double, const std::string&);
public:
    //using Type = std::decay_t<decltype (*this)>;
    METACLASS_DEFINITION(MetadataTest)
    REFLECT_VARIABLE(var1)
    REFLECT_VARIABLE(var2)
    REFLECT_STATIC_VARIABLE(static_var)
    REFLECT_METHOD(foo,int)
    REFLECT_METHOD(foo,int,double)
    REFLECT_STATIC_METHOD(static_foo,int,double,const std::string&)
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

void MetadataTest::class_name_method() {
    QVERIFY(reflect::get_class_name<Type>() == HANA_STR("MetadataTest"));
}
void MetadataTest::vars_obj_names() {
    QVERIFY(false);
}

void MetadataTest::vars_static_names() {
    QVERIFY(false);
}

void MetadataTest::vars_names() {
    QCOMPARE(reflect::get_variable_name<Type>(::boost::hana::size_c<0>),HANA_STR("var1"));
    QCOMPARE(reflect::get_variable_name<Type>(::boost::hana::size_c<1>),HANA_STR("var2"));
    QCOMPARE(reflect::get_variable_name<Type>(::boost::hana::size_c<2>),HANA_STR("static_var"));
}

void MetadataTest::method_obj_names() {
    QVERIFY(false);
}

void MetadataTest::method_static_names() {
    QVERIFY(false);
}

void MetadataTest::method_names() {
    QCOMPARE(reflect::get_method_name<Type>(::boost::hana::size_c<0>),HANA_STR("foo"));
    QCOMPARE(reflect::get_method_name<Type>(::boost::hana::size_c<1>),HANA_STR("foo"));
    QCOMPARE(reflect::get_method_name<Type>(::boost::hana::size_c<2>),HANA_STR("static_foo"));
}

void MetadataTest::counter_obj_vars() {
    QVERIFY(reflect::get_variables_obj_count<Type>() == 2);
}

void MetadataTest::counter_static_vars() {
    QVERIFY(reflect::get_variables_static_count<Type>() == 1);
}

void MetadataTest::counter_vars() {
    QVERIFY(reflect::get_variables_count<Type>() == 3);
}

void MetadataTest::counter_obj_method() {
    QVERIFY(reflect::get_methods_obj_count<Type>() == 2);
}

void MetadataTest::counter_static_method() {
    QVERIFY(reflect::get_methods_static_count<Type>() == 1);
}

void MetadataTest::counter_method() {
    QVERIFY(reflect::get_methods_count<Type>() == 3);
}

void MetadataTest::find_obj_vars() {
    QCOMPARE(reflect::find_obj_variable_index<Type>(HANA_STR("var1")),boost::hana::size_c<0>);
    QCOMPARE(reflect::find_obj_variable_index<Type>(HANA_STR("var2")),boost::hana::size_c<1>);
    QCOMPARE(reflect::find_obj_variable_index<Type>(HANA_STR("static_var")),boost::hana::nothing);
}

void MetadataTest::find_static_vars() {
    QCOMPARE(reflect::find_static_variable_index<Type>(HANA_STR("var1")),boost::hana::nothing);
    QCOMPARE(reflect::find_static_variable_index<Type>(HANA_STR("var2")),boost::hana::nothing);
    QCOMPARE(reflect::find_static_variable_index<Type>(HANA_STR("static_var")),boost::hana::size_c<0>);
}

void MetadataTest::find_vars() {
    QCOMPARE(reflect::find_variable_index<Type>(HANA_STR("var1")),boost::hana::size_c<0>);
    QCOMPARE(reflect::find_variable_index<Type>(HANA_STR("var2")),boost::hana::size_c<1>);
    QCOMPARE(reflect::find_variable_index<Type>(HANA_STR("nothing")),boost::hana::nothing);
}

void MetadataTest::find_obj_methods() {
    QVERIFY(reflect::find_obj_method_index<Type>(HANA_STR("foo")) == (boost::hana::tuple_c<std::size_t,0,1>));
    QVERIFY(reflect::find_obj_method_index<Type>(HANA_STR("static_foo")) == boost::hana::nothing);
    QVERIFY(reflect::find_obj_method_index<Type>(HANA_STR("nothing")) == boost::hana::nothing);
}

void MetadataTest::find_static_methods() {
    QVERIFY(reflect::find_static_method_index<Type>(HANA_STR("foo")) == boost::hana::nothing);
    QVERIFY(reflect::find_static_method_index<Type>(HANA_STR("static_foo")) == (boost::hana::tuple_c<std::size_t,0>));
    QVERIFY(reflect::find_static_method_index<Type>(HANA_STR("nothing")) == boost::hana::nothing);
}

void MetadataTest::find_methods() {
    QVERIFY(reflect::find_method_index<Type>(HANA_STR("foo")) == (boost::hana::tuple_c<std::size_t,0,1>));
    QVERIFY(reflect::find_method_index<Type>(HANA_STR("static_foo")) == (boost::hana::tuple_c<std::size_t,2>));
    QVERIFY(reflect::find_method_index<Type>(HANA_STR("nothing")) == boost::hana::nothing);
}

void MetadataTest::get_obj_method_types() {
    QVERIFY(false);
}

void MetadataTest::get_static_method_types() {
    QVERIFY(false);
}

void MetadataTest::get_method_types() {
    QVERIFY (reflect::get_method_arg_types<Type>(::boost::hana::size_c<0>) == ::boost::hana::tuple_t<int>);
    QVERIFY (reflect::get_method_arg_types<Type>(::boost::hana::size_c<1>) == (::boost::hana::tuple_t<int,double>));
    QVERIFY (reflect::get_method_arg_types<Type>(::boost::hana::size_c<2>) == (::boost::hana::tuple_t<int,double,const std::string&>));
    QCOMPARE(typeid(reflect::method_arg_type_t<Type,0,0>),typeid(int));
    QCOMPARE(typeid(reflect::method_arg_type_t<Type,1,0>),typeid(int));
    QCOMPARE(typeid(reflect::method_arg_type_t<Type,1,1>),typeid(double));
    QCOMPARE(typeid(reflect::method_arg_type_t<Type,2,0>),typeid(int));
    QCOMPARE(typeid(reflect::method_arg_type_t<Type,2,1>),typeid(double));
    QCOMPARE(typeid(reflect::method_arg_type_t<Type,2,2>),typeid(const std::string&));
    QCOMPARE(typeid(reflect::method_arg_type_t<Type,0,0>),typeid(std::decay_t<decltype(reflect::method_arg_type<Type>(::boost::hana::size_c<0>,::boost::hana::size_c<0>))>::type));
    QCOMPARE(typeid(reflect::method_return_type_t<Type,0>),typeid(void));
    QCOMPARE(typeid(reflect::method_return_type_t<Type,1>),typeid(int));
    QCOMPARE(typeid(reflect::method_return_type_t<Type,2>),typeid(double));
}

void MetadataTest::check_obj_invoke_method() {
    QCOMPARE(reflect::check_invoke_obj_method<Type>(boost::hana::nothing),boost::hana::nothing);
    QCOMPARE((reflect::check_invoke_obj_method<Type,int>(reflect::find_obj_method_index<Type>(HANA_STR("foo")))), ::boost::hana::make_tuple(true,false));
    QCOMPARE((reflect::check_invoke_obj_method<Type,int,double>(reflect::find_obj_method_index<Type>(HANA_STR("foo")))), ::boost::hana::make_tuple(false,true));
    QCOMPARE((reflect::check_invoke_obj_method<Type,int&&,double,std::string>(reflect::find_obj_method_index<Type>(HANA_STR("static_foo")))), boost::hana::nothing);
}

void MetadataTest::check_static_invoke_method() {
    QCOMPARE(reflect::check_invoke_static_method<Type>(boost::hana::nothing),boost::hana::nothing);
    QCOMPARE((reflect::check_invoke_static_method<Type,int>(reflect::find_static_method_index<Type>(HANA_STR("foo")))), boost::hana::nothing);
    QCOMPARE((reflect::check_invoke_static_method<Type,int,double>(reflect::find_static_method_index<Type>(HANA_STR("foo")))), boost::hana::nothing);
    QCOMPARE((reflect::check_invoke_static_method<Type,int&&,double,std::string>(reflect::find_static_method_index<Type>(HANA_STR("static_foo")))), ::boost::hana::make_tuple(true));
}

void MetadataTest::check_invoke_method() {
    QCOMPARE(reflect::check_invoke_method<Type>(boost::hana::nothing),boost::hana::nothing);
    QCOMPARE((reflect::check_invoke_method<Type,int>(reflect::find_method_index<Type>(HANA_STR("foo")))), ::boost::hana::make_tuple(true,false));
    QCOMPARE((reflect::check_invoke_method<Type,int,double>(reflect::find_method_index<Type>(HANA_STR("foo")))), ::boost::hana::make_tuple(false,true));
    QCOMPARE((reflect::check_invoke_method<Type,int&&,double,std::string>(reflect::find_method_index<Type>(HANA_STR("static_foo")))), ::boost::hana::make_tuple(true));
}

QTEST_MAIN(MetadataTest)
#include "main.moc"
