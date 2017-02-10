#include <reflect.hpp>
#include <QtTest>

class TestVariable : public QObject {
    Q_OBJECT
    int var {1};
    const int const_var {2};
    static int static_var;
public:
    METACLASS_DEFINITION(TestVariable)
    REFLECT_VARIABLE(var)
    REFLECT_VARIABLE(const_var)
    REFLECT_STATIC_VARIABLE(static_var)

private slots:
    void get_variable();
    void set_variable();
    void variable_type();
    void static_variable();
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

void TestVariable::variable_type() {
    QCOMPARE(typeid(reflect::get_variable(*this,boost::hana::size_c<0>)),typeid(int));
    QCOMPARE(typeid(reflect::get_variable(*this,boost::hana::size_c<1>)),typeid(const int));
}

void TestVariable::static_variable() {
    QCOMPARE(reflect::get_variable(*this,boost::hana::size_c<2>),3);
}

QTEST_MAIN(TestVariable)
#include "main.moc"
