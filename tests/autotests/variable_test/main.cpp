#include <meta_utils.hpp>
#include <QtTest>
#include <iostream>

class TestVariable : public QObject {
    Q_OBJECT
    int var {1};
    const int const_var {2};
public:
    METACLASS_DEFENITION(TestVariable)
    REFLECT_VARIABLE(var)
    REFLECT_VARIABLE(const_var)
private slots:
    void getVariable();
    void setVariable();
    void findVariable();
    void variableType();
};

void TestVariable::getVariable() {
    QCOMPARE(reflect::get_variable(*this,boost::hana::size_c<0>),1);
    QCOMPARE(reflect::get_variable(*this,boost::hana::size_c<1>),2);
}

void TestVariable::setVariable() {
    reflect::get_variable(*this,boost::hana::size_c<0>) = 0;
    QCOMPARE(reflect::get_variable(*this,boost::hana::size_c<0>),0);
}

void TestVariable::findVariable() {
    QCOMPARE(boost::hana::size(reflect::find_variable_name<decltype(*this)>(HANA_STR("var"))),boost::hana::size_c<1>);
    QCOMPARE(boost::hana::size(reflect::find_variable_name<decltype(*this)>(HANA_STR("const_var"))),boost::hana::size_c<1>);
    QCOMPARE(boost::hana::size(reflect::find_variable_name<decltype(*this)>(HANA_STR("nothing"))),boost::hana::size_c<0>);
}

void TestVariable::variableType() {
    QCOMPARE(typeid(reflect::get_variable(*this,boost::hana::size_c<0>)),typeid(int));
    QCOMPARE(typeid(reflect::get_variable(*this,boost::hana::size_c<1>)),typeid(const int));
}

QTEST_MAIN(TestVariable)
#include "main.moc"
