#include <reflect.hpp>
#include <QtTest>

class MethodTest : public QObject {
    Q_OBJECT
    bool value {false};
    void bool_method (bool cond) { QVERIFY(cond); }
    bool bool_method () { return true;}
    void bool_method (bool& cond) { cond = value;}
    static void static_method (bool cond) { QVERIFY(cond); }
    void const_method(bool cond) const { QVERIFY(cond); }

public:
    METACLASS_DEFINITION(MethodTest)
    REFLECT_METHOD(bool_method,bool)
    REFLECT_METHOD(bool_method)
    REFLECT_METHOD(bool_method,bool&)
    REFLECT_STATIC_METHOD(static_method,bool)
    REFLECT_CONST_METHOD(const_method,bool)
private slots:
    void invoke_method();
    void get_method_value();
    void reference_method();
    void const_method();
    void constexpr_method();
    void static_method();
    void try_invoke_method();
};

class Constexpr_class {
public:
    METACLASS_DEFINITION(Constexpr_class)
    constexpr bool bool_method() const { return true;}
    REFLECT_CONST_METHOD(bool_method)

};

void MethodTest::invoke_method() {
    reflect::invoke_method(*this,boost::hana::size_c<0>,true);
}

void MethodTest::get_method_value() {
    QCOMPARE (reflect::invoke_method(*this,boost::hana::size_c<1>),true);
}

void MethodTest::reference_method() {
    bool res = true;
    reflect::invoke_method(*this,boost::hana::size_c<2>,res);
    QCOMPARE (res,value);
}

void MethodTest::const_method() {
    reflect::invoke_method(*this,boost::hana::size_c<4>,true);
    const Constexpr_class obj;
    QVERIFY(reflect::invoke_method(obj,boost::hana::size_c<0>));
}

void MethodTest::constexpr_method() {
    Constexpr_class obj;
    constexpr bool res = reflect::invoke_method(obj,boost::hana::size_c<0>);
    QVERIFY(res);
}

void MethodTest::static_method() {
    //reflect::invoke_method(*this,boost::hana::size_c<3>,true);
    QVERIFY(false);
}

void MethodTest::try_invoke_method() {
    QVERIFY(boost::hana::at_c<0>(reflect::try_invoke_method(*this,reflect::find_method_index<Type>(HANA_STR("bool_method")))));
}


QTEST_MAIN(MethodTest)
#include "main.moc"
