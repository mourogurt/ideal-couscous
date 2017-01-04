#include <meta_utils.hpp>
#include <QtTest>
#include <iostream>

class TestMethod : public QObject {
    Q_OBJECT
    bool value {false};
    void simple_method (bool cond) { QVERIFY(cond); }
    bool int_method () { return true;}
    void change_method (bool& cond) { cond = value;}

public:
    METACLASS_DEFENITION(TestMethod)
    REFLECT_METHOD(simple_method,bool)
    REFLECT_METHOD(int_method)
    REFLECT_METHOD(change_method,bool&)
private slots:
    void emit_method();
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

void TestMethod::emit_method() {
    reflect::emit_method(*this,boost::hana::size_c<0>,true);
}

void TestMethod::get_method_value() {
    QCOMPARE (reflect::emit_method(*this,boost::hana::size_c<1>),true);
}

void TestMethod::get_method_types() {
    QVERIFY(false);
}

void TestMethod::reference_method() {
    bool res = true;
    reflect::emit_method(*this,boost::hana::size_c<2>,res);
    QCOMPARE (res,value);
}

void TestMethod::const_method() {
    QVERIFY(false);
}

void TestMethod::constexpr_method() {
    Constexpr_class obj;
    constexpr bool res = reflect::emit_method(obj,boost::hana::size_c<0>);
    QVERIFY(res);
}

void TestMethod::static_method() {
    QVERIFY(false);
}

void TestMethod::find_method() {
    QVERIFY(false);
}

void TestMethod::try_emit_method() {
    QVERIFY(false);
}


QTEST_MAIN(TestMethod)
#include "main.moc"
