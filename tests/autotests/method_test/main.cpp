#include <QtTest>
#include <boost/hana/replicate.hpp>
#include <reflect.hpp>

using namespace boost::hana::literals;

class MethodTest : public QObject {
  Q_OBJECT
public:
  bool value{true};
  bool value2{false};
  bool bool_method(bool cond = false) { return cond; }
  bool bool_method() { return value; }
  void bool_method(bool &cond) { cond = value; }
  bool &bool_method2() { return value; }
  bool &bool_method3() { return value2; }
  static void static_method(bool cond) { QVERIFY(cond); }
  void const_method(bool cond) const { QVERIFY(cond); }

  IN_METAINFO(MethodTest)
  REFLECT_OBJ_MTD(bool_method, bool)
  REFLECT_OBJ_MTD(bool_method)
  REFLECT_OBJ_MTD(bool_method, bool &)
  REFLECT_OBJ_MTD(bool_method2)
  REFLECT_OBJ_MTD(bool_method3)
  REFLECT_STATIC_MTD(static_method, bool)
  REFLECT_CONST_OBJ_MTD(const_method, bool)

private slots:
  void obj_method();
  void const_method();
  void constexpr_method();
  void static_method();
};

class Constexpr_class {
public:
  constexpr auto bool_method() const { return true; }
  auto fail_method() { return false; }
  OUT_METAINFO(Constexpr_class)
};

METAINFO(Constexpr_class)
REFLECT_CONST_OBJ_MTD(bool_method)
REFLECT_OBJ_MTD(fail_method)
END_METAINFO

void MethodTest::obj_method() {
  value = true;
  value2 = false;
  QCOMPARE(typeid(reflect::utils::invoke<Type, reflect::ObjMethods>(
               0_c, std::ref(*this), true)),
           typeid(true));
  reflect::utils::invoke<Type, reflect::ObjMethods>(0_c, std::ref(*this), true);
  QCOMPARE(
      (reflect::utils::invoke<Type, reflect::ObjMethods>(1_c, std::ref(*this))),
      true);
  bool res = false;
  QCOMPARE(typeid(reflect::utils::invoke<Type, reflect::ObjMethods>(
               2_c, std::ref(*this), res)),
           typeid(void));
  reflect::utils::invoke<Type, reflect::ObjMethods>(2_c, std::ref(*this), res);
  QCOMPARE(res, true);
  reflect::utils::invoke<Type, reflect::ObjMethods>(3_c, std::ref(*this)) =
      false;
  QCOMPARE(value, false);
  reflect::utils::invoke<Type, reflect::ObjMethods>(4_c, std::ref(*this)) =
      true;
  QCOMPARE(value2, true);
}

void MethodTest::const_method() {
  QCOMPARE(typeid(reflect::utils::invoke<Type, reflect::ConstMethods>(
               0_c, std::ref(*this), true)),
           typeid(void));
  reflect::utils::invoke<Type, reflect::ConstMethods>(0_c, std::ref(*this),
                                                      true);
}

void MethodTest::constexpr_method() {
  const Constexpr_class obj;
  constexpr bool res =
      reflect::utils::invoke<Constexpr_class, reflect::ConstMethods>(0_c, obj);
  QCOMPARE(res, true);
}

void MethodTest::static_method() {
  value = true;
  value2 = false;
  QCOMPARE(typeid(reflect::utils::invoke<Type, reflect::StaticMethods>(
               0_c, std::ref(*this), true)),
           typeid(void));
  reflect::utils::invoke<Type, reflect::StaticMethods>(0_c, std::ref(*this),
                                                       true);
  QCOMPARE(
      typeid(reflect::utils::invoke<Type, reflect::StaticMethods>(0_c, true)),
      typeid(void));
  reflect::utils::invoke<Type, reflect::StaticMethods>(0_c, true);
}

QTEST_MAIN(MethodTest)
#include "main.moc"
