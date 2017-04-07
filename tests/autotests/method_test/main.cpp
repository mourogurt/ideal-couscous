#include <reflect.hpp>
#include <QtTest>

class MethodTest : public QObject {
    Q_OBJECT
    bool value {true};
    void bool_method (bool cond) { QVERIFY(cond); }
    bool bool_method () { return true;}
    void bool_method (bool& cond) { cond = value;}
    static void static_method (bool cond) { QVERIFY(cond); }
    void const_method(bool cond) const { QVERIFY(cond); }

public:
    IN_METAINFO(MethodTest)
    REFLECT_OBJ_MET(bool_method,bool)
    REFLECT_OBJ_MET(bool_method)
    REFLECT_OBJ_MET(bool_method,bool&)
    REFLECT_STATIC_MET(static_method,bool)
    REFLECT_CONST_OBJ_MET(const_method,bool)

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
    constexpr auto bool_method() const { return true;}
    OUT_METAINFO(Constexpr_class)
};

METAINFO(Constexpr_class)
REFLECT_CONST_OBJ_MET(bool_method)
END_METAINFO

void MethodTest::invoke_method() {
    reflect::utils::invoke<Type,reflect::ObjMethods>(boost::hana::size_c<0>,*this,true);
}

void MethodTest::get_method_value() {
    QCOMPARE ((reflect::utils::invoke<Type,reflect::ObjMethods>(boost::hana::size_c<1>,*this)),true);
}

void MethodTest::reference_method() {
    bool res = false;
    reflect::utils::invoke<Type,reflect::ObjMethods>(boost::hana::size_c<2>,*this,res);
    QCOMPARE (res,value);
}

void MethodTest::const_method() {
    reflect::utils::invoke<Type,reflect::ConstMethods>(boost::hana::size_c<0>,*this,true);
    const Constexpr_class obj;
    QVERIFY((reflect::utils::invoke<Constexpr_class,reflect::ConstMethods>(boost::hana::size_c<0>,obj)));
}

void MethodTest::constexpr_method() {
    Constexpr_class obj;
    constexpr bool res = reflect::utils::invoke<Constexpr_class,reflect::ObjMethods>(boost::hana::size_c<0>,obj);
    QVERIFY(res);
}

void MethodTest::static_method() {
    reflect::utils::invoke<Type,reflect::StaticMethods>(boost::hana::size_c<0>,true);
    reflect::utils::invoke<Type,reflect::StaticMethods>(boost::hana::size_c<0>,*this,true);
}

void MethodTest::try_invoke_method() {
    QVERIFY(false);
//    QVERIFY(boost::hana::at_c<0>(reflect::try_invoke_method(*this,reflect::find_method_index<Type>(HANA_STR("bool_method")))));
}


QTEST_MAIN(MethodTest)
#include "main.moc"
