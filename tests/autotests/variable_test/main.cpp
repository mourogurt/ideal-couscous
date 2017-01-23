#include <reflect.hpp>
#include <QtTest>
#include <iostream>

class TestVariable : public QObject {
    Q_OBJECT
    int var {1};
    const int const_var {2};
    static int static_var;
public:
    METACLASS_DEFENITION(TestVariable)
    REFLECT_VARIABLE(var)
    REFLECT_VARIABLE(const_var)
    REFLECT_STATIC_VARIABLE(static_var)

private slots:
    void get_variable();
    void set_variable();
    void find_variable();
    void variable_type();
    void variables_counter();
    void static_variable();
    void class_name();
    void var_name();
};

int TestVariable::static_var = 3;

void TestVariable::get_variable() {
    QCOMPARE(reflect::get_variable(*this,boost::hana::size_c<0>),1);
    QCOMPARE(reflect::get_variable(*this,boost::hana::size_c<1>),2);
}

void TestVariable::set_variable() {
    reflect::get_variable(*this,boost::hana::size_c<0>) = 0;
    QCOMPARE(reflect::get_variable(*this,boost::hana::size_c<0>),0);
}

void TestVariable::find_variable() {
    QCOMPARE(boost::hana::size(reflect::find_variable_name<decltype(*this)>(HANA_STR("var"))),boost::hana::size_c<1>);
    QCOMPARE(boost::hana::size(reflect::find_variable_name<decltype(*this)>(HANA_STR("const_var"))),boost::hana::size_c<1>);
    QCOMPARE(boost::hana::size(reflect::find_variable_name<decltype(*this)>(HANA_STR("nothing"))),boost::hana::size_c<0>);
}

void TestVariable::variable_type() {
    QCOMPARE(typeid(reflect::get_variable(*this,boost::hana::size_c<0>)),typeid(int));
    QCOMPARE(typeid(reflect::get_variable(*this,boost::hana::size_c<1>)),typeid(const int));
}

void TestVariable::variables_counter() {
    QVERIFY(reflect::get_variables_count<decltype(*this)>() == 3);
}

void TestVariable::static_variable() {
    QCOMPARE(reflect::get_variable(*this,boost::hana::size_c<2>),3);
}

void TestVariable::class_name() {
    QCOMPARE(reflect::get_class_name<decltype (*this)>(),HANA_STR("TestVariable"));
}

void TestVariable::var_name() {
    QCOMPARE(reflect::get_variable_name(*this,::boost::hana::size_c<0>),HANA_STR("var"));
}

QTEST_MAIN(TestVariable)
#include "main.moc"
