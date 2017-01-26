#include <reflect.hpp>
#include <QtTest>
#include <iostream>

class TestMethod : public QObject {
    Q_OBJECT
    bool value {false};
    void bool_method (bool cond) { QVERIFY(cond); }
    bool bool_method () { return true;}
    void bool_method (bool& cond) { cond = value;}
    static void static_method (bool cond) { QVERIFY(cond); }
    void const_method(bool cond) const { QVERIFY(cond); }

public:
    METACLASS_DEFINITION(TestMethod)
    REFLECT_METHOD(bool_method,bool)
    REFLECT_METHOD(bool_method)
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
    void try_invoke_method();
    void method_name();
    void methods_counter();
};

class Constexpr_class {
public:
    METACLASS_DEFINITION(Constexpr_class)
    constexpr bool bool_method() const { return true;}
    REFLECT_CONST_METHOD(bool_method)

};

void TestMethod::invoke_method() {
    reflect::invoke_method(*this,boost::hana::size_c<0>,true);
}

void TestMethod::get_method_value() {
    QCOMPARE (reflect::invoke_method(*this,boost::hana::size_c<1>),true);
}

void TestMethod::get_method_types() {
    QCOMPARE(typeid(reflect::method_arg_type_t<decltype (*this),0,0>),typeid(bool));
    QCOMPARE(typeid(reflect::method_return_type_t<decltype (*this),1>),typeid(bool));
    QCOMPARE(typeid(reflect::method_arg_type_t<decltype (*this),2,0>),typeid(decltype(reflect::method_arg_type<decltype(*this)>(::boost::hana::size_c<2>,::boost::hana::size_c<0>))));
    QCOMPARE(typeid(reflect::method_return_type_t<decltype (*this),1>),typeid(decltype(reflect::method_return_type<decltype(*this)>(::boost::hana::size_c<1>))));
}

void TestMethod::reference_method() {
    bool res = true;
    reflect::invoke_method(*this,boost::hana::size_c<2>,res);
    QCOMPARE (res,value);
}

void TestMethod::const_method() {
    reflect::invoke_method(*this,boost::hana::size_c<4>,true);
    const Constexpr_class obj;
    QVERIFY(reflect::invoke_method(obj,boost::hana::size_c<0>));
}

void TestMethod::constexpr_method() {
    Constexpr_class obj;
    constexpr bool res = reflect::invoke_method(obj,boost::hana::size_c<0>);
    QVERIFY(res);
}

void TestMethod::static_method() {
    reflect::invoke_method(*this,boost::hana::size_c<3>,true);
}

void TestMethod::find_method() {
    QCOMPARE(boost::hana::size(reflect::find_method_name<decltype(*this)>(HANA_STR("bool_method"))),boost::hana::size_c<3>);
    QCOMPARE(boost::hana::size(reflect::find_method_name<decltype(*this)>(HANA_STR("static_method"))),boost::hana::size_c<1>);
    QCOMPARE(boost::hana::size(reflect::find_method_name<decltype(*this)>(HANA_STR("nothing"))),boost::hana::size_c<0>);
}

void TestMethod::try_invoke_method() {
    QVERIFY(boost::hana::at_c<0>(reflect::try_invoke_method(*this,reflect::find_method_name<decltype(*this)>(HANA_STR("bool_method")))));
    QCOMPARE(reflect::check_invoke_method<decltype(*this)>(reflect::find_method_name<decltype(*this)>(HANA_STR("bool_method"))),::boost::hana::make_tuple(false,true,false));
}

void TestMethod::method_name() {
    QCOMPARE(reflect::get_method_name<decltype (*this)>(::boost::hana::size_c<0>),HANA_STR("bool_method"));
}

void TestMethod::methods_counter() {
    QVERIFY(reflect::get_methods_count<decltype(*this)>() == 5);
}


QTEST_MAIN(TestMethod)
#include "main.moc"
