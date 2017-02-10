#include <reflect.hpp>
#include <QtTest>

class MetadataTest : public QObject {
    Q_OBJECT
    int var1,var2;
    void foo1 ();
    void foo2 ();
public:
    METACLASS_DEFINITION(MetadataTest)
    REFLECT_VARIABLE(var1)
    REFLECT_VARIABLE(var2)
    REFLECT_METHOD(foo1)
    REFLECT_METHOD(foo2)
private slots:
    void class_name_method();
    void vars_names();
    void vars_static_names();
    void vars_all_names();
    void method_names();
    void method_static_names();
    void method_all_names();
    void counter_vars();
    void counter_static_vars();
    void counter_all_vars();
    void counter_method();
    void counter_static_method();
    void counter_all_method();
    void find_vars();
    void find_static_vars();
    void find_all_vars();
    void find_methods();
    void find_static_methods();
    void find_all_methods();
};

void MetadataTest::class_name_method() {
    QCOMPARE(reflect::get_class_name<decltype (*this)>(),HANA_STR("MetadataTest"));
}
void MetadataTest::vars_names() {
    QVERIFY(false);
}

void MetadataTest::vars_static_names() {
    QVERIFY(false);
}

void MetadataTest::vars_all_names() {
    QCOMPARE(reflect::get_variable_name<decltype (*this)>(::boost::hana::size_c<0>),HANA_STR("var1"));
    QCOMPARE(reflect::get_variable_name<decltype (*this)>(::boost::hana::size_c<1>),HANA_STR("var2"));
}

void MetadataTest::method_names() {
    QVERIFY(false);
}

void MetadataTest::method_static_names() {
    QVERIFY(false);
}

void MetadataTest::method_all_names() {
    QCOMPARE(reflect::get_method_name<decltype (*this)>(::boost::hana::size_c<0>),HANA_STR("foo1"));
    QCOMPARE(reflect::get_method_name<decltype (*this)>(::boost::hana::size_c<1>),HANA_STR("foo2"));
}

void MetadataTest::counter_vars() {
    QVERIFY(false);
}

void MetadataTest::counter_static_vars() {
    QVERIFY(false);
}

void MetadataTest::counter_all_vars() {
    QVERIFY(reflect::get_variables_count<decltype(*this)>() == 2);
}

void MetadataTest::counter_method() {
    QVERIFY(false);
}

void MetadataTest::counter_static_method() {
    QVERIFY(false);
}

void MetadataTest::counter_all_method() {
    QVERIFY(reflect::get_methods_count<decltype(*this)>() == 2);
}

void MetadataTest::find_vars() {
    QVERIFY(false);
}

void MetadataTest::find_static_vars() {
    QVERIFY(false);
}

void MetadataTest::find_all_vars() {
    QCOMPARE(reflect::find_variable_name<decltype (*this)>(HANA_STR("var1")),boost::hana::size_c<0>);
    QCOMPARE(reflect::find_variable_name<decltype (*this)>(HANA_STR("var2")),boost::hana::size_c<1>);
    QCOMPARE(reflect::find_variable_name<decltype (*this)>(HANA_STR("nothing")),boost::hana::nothing);
}

void MetadataTest::find_methods() {
    QVERIFY(false);
}

void MetadataTest::find_static_methods() {
    QVERIFY(false);
}

void MetadataTest::find_all_methods() {
    QVERIFY(reflect::find_method_name<decltype (*this)>(HANA_STR("foo1")) == boost::hana::make_tuple(boost::hana::size_c<0>));
    QVERIFY(reflect::find_method_name<decltype (*this)>(HANA_STR("foo2")) == boost::hana::make_tuple(boost::hana::size_c<1>));
    QVERIFY(reflect::find_method_name<decltype (*this)>(HANA_STR("nothing")) == boost::hana::make_tuple());
}

QTEST_MAIN(MetadataTest)
#include "main.moc"
