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
    void get_variable();
    void set_variable();
    void find_variable();
    void variable_type();
    void variable_counter();
    void static_variable();
};

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

void TestVariable::variable_counter() {
    using Counter_type = decltype(decltype(std::decay_t<decltype(*this)>::Variable_counter(counter<>{}))::value);
    QCOMPARE(Counter_type(decltype(std::decay_t<decltype(*this)>::Variable_counter(counter<>{}))::value),2);
}

void TestVariable::static_variable() {
    QVERIFY(false);
}

QTEST_MAIN(TestVariable)
#include "main.moc"
