#include <reflect.hpp>
#include <QtTest>
#include <iostream>

class TestMethod : public QObject {
    Q_OBJECT
    bool value {false};
    void bool_method (bool cond) { QVERIFY(cond); }
    bool int_method () { return true;}
    void bool_method (bool& cond) { cond = value;}
    static void static_method (bool cond) { QVERIFY(cond); }
    void const_method(bool cond) const { QVERIFY(cond); }

public:
    METACLASS_DEFENITION(TestMethod)
    REFLECT_METHOD(bool_method,bool)
    REFLECT_METHOD(int_method)
    REFLECT_METHOD(bool_method,bool&)
    REFLECT_STATIC_METHOD(static_method,bool)
    REFLECT_CONST_METHOD(const_method,bool)
private slots:
    void invoke_method();
    void get_method_value();
    void get_method_types();
    void reference_method();
    void const_method();
    void constexpr_method();
    void static_method();
    void find_method();
    void try_emit_method();
};

class Constexpr_class {
public:
    METACLASS_DEFENITION(Constexpr_class)
    constexpr bool bool_method() { return true;}
    REFLECT_METHOD(bool_method)

};

void TestMethod::invoke_method() {
    reflect::invoke_method(*this,boost::hana::size_c<0>,true);
}

void TestMethod::get_method_value() {
    QCOMPARE (reflect::invoke_method(*this,boost::hana::size_c<1>),true);
}

void TestMethod::get_method_types() {
    QVERIFY(false);
}

void TestMethod::reference_method() {
    bool res = true;
    reflect::invoke_method(*this,boost::hana::size_c<2>,res);
    QCOMPARE (res,value);
}

void TestMethod::const_method() {
    reflect::invoke_method(*this,boost::hana::size_c<4>,true);
}

void TestMethod::constexpr_method() {
    Constexpr_class obj;
    constexpr bool res = reflect::invoke_method(obj,boost::hana::size_c<0>);
    QVERIFY(res);
    //constexpr Constexpr_class obj2;
    //res = reflect::invoke_method(obj2,boost::hana::size_c<0>);
    QVERIFY(false);
}

void TestMethod::static_method() {
    reflect::invoke_method(*this,boost::hana::size_c<3>,true);
}

void TestMethod::find_method() {
    QCOMPARE(boost::hana::size(reflect::find_method_name<decltype(*this)>(HANA_STR("bool_method"))),boost::hana::size_c<2>);
    QCOMPARE(boost::hana::size(reflect::find_method_name<decltype(*this)>(HANA_STR("int_method"))),boost::hana::size_c<1>);
    QCOMPARE(boost::hana::size(reflect::find_method_name<decltype(*this)>(HANA_STR("nothing"))),boost::hana::size_c<0>);
}

void TestMethod::try_emit_method() {
    QVERIFY(false);
}


QTEST_MAIN(TestMethod)
#include "main.moc"
