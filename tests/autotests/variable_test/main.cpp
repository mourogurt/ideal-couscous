#include <reflect.hpp>
#include <QtTest>

class VariableTest : public QObject {
    Q_OBJECT
    //int var {1};
    //const int const_var {2};
    //static int static_var;
public:
//    METACLASS_DEFINITION(VariableTest)
//    REFLECT_VARIABLE(var)
//    REFLECT_VARIABLE(const_var)
//    REFLECT_STATIC_VARIABLE(static_var)

private slots:
    void get_variable();
    void set_variable();
    void variable_type();
    void static_variable();
};

//int VariableTest::static_var = 3;

void VariableTest::get_variable() {
    QVERIFY(false);
//    QCOMPARE(reflect::get_variable(*this,boost::hana::size_c<0>),1);
//    QCOMPARE(reflect::get_variable(*this,boost::hana::size_c<1>),2);
}

void VariableTest::set_variable() {
    QVERIFY(false);
//    reflect::get_variable(*this,boost::hana::size_c<0>) = 0;
//    QCOMPARE(reflect::get_variable(*this,boost::hana::size_c<0>),0);
}

void VariableTest::variable_type() {
    QVERIFY(false);
//    QCOMPARE(typeid(reflect::get_variable(*this,boost::hana::size_c<0>)),typeid(int));
//    QCOMPARE(typeid(reflect::get_variable(*this,boost::hana::size_c<1>)),typeid(const int));
}

void VariableTest::static_variable() {
    QVERIFY(false);
    //QCOMPARE(reflect::get_variable(*this,boost::hana::size_c<2>),3);
//    QVERIFY(false);
}

QTEST_MAIN(VariableTest)
#include "main.moc"
