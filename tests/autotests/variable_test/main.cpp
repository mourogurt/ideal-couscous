#include <reflect.hpp>
#include <QtTest>

class VariableTest : public QObject {
    Q_OBJECT
    int var {1};
    const int const_var {2};
    static int static_var;
public:
    IN_CLASS_META_INFO(VariableTest)
    REFLECT_OBJ_VARIABLE(var)
    REFLECT_OBJ_VARIABLE(const_var)
    REFLECT_STATIC_VARIABLE(static_var)

private slots:
    void get_obj_variable();
    void get_static_variable();
    void get_variable();
    void set_obj_variable();
    void set_static_variable();
    void set_variable();
};

int VariableTest::static_var = 3;

void VariableTest::get_obj_variable() {
    QCOMPARE((reflect::utils::invoke<Type,reflect::ObjVars>(boost::hana::size_c<0>,*this)),1);
    QCOMPARE((reflect::utils::invoke<Type,reflect::ObjVars>(boost::hana::size_c<1>,*this)),2);
}

void VariableTest::get_static_variable() {
    QCOMPARE((reflect::utils::invoke<Type,reflect::StaticVars>(boost::hana::size_c<0>,*this)),3);
}

void VariableTest::get_variable() {
    QCOMPARE((reflect::utils::invoke<Type,reflect::AllVars>(boost::hana::size_c<0>,*this)),1);
    QCOMPARE((reflect::utils::invoke<Type,reflect::AllVars>(boost::hana::size_c<1>,*this)),2);
    QCOMPARE((reflect::utils::invoke<Type,reflect::AllVars>(boost::hana::size_c<2>,*this)),3);
}

void VariableTest::set_obj_variable() {
    reflect::utils::invoke<Type,reflect::ObjVars>(boost::hana::size_c<0>,*this) = 11;
    QCOMPARE((reflect::utils::invoke<Type,reflect::ObjVars>(boost::hana::size_c<0>,*this)),11);
}

void VariableTest::set_static_variable() {
    reflect::utils::invoke<Type,reflect::StaticVars>(boost::hana::size_c<0>) = 12;
    QCOMPARE((reflect::utils::invoke<Type,reflect::StaticVars>(boost::hana::size_c<0>)),12);
}

void VariableTest::set_variable() {
    reflect::utils::invoke<Type,reflect::AllVars>(boost::hana::size_c<0>,*this) = 13;
    reflect::utils::invoke<Type,reflect::AllVars>(boost::hana::size_c<2>,*this) = 14;
    QCOMPARE((reflect::utils::invoke<Type,reflect::AllVars>(boost::hana::size_c<0>,*this)),13);
    QCOMPARE((reflect::utils::invoke<Type,reflect::AllVars>(boost::hana::size_c<2>,*this)),14);
}

QTEST_MAIN(VariableTest)
#include "main.moc"
