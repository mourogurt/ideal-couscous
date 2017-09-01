#include <QtTest>
#include <boost/hana/replicate.hpp>
#include <boost/type_index.hpp>
#include <reflect.hpp>

using namespace boost::hana::literals;

class VariableTest : public QObject {
  Q_OBJECT
public:
  int var{1};
  const int const_var{2};
  static int static_var;
  IN_METAINFO(VariableTest)
  REFLECT_OBJ_VAR(var)
  REFLECT_OBJ_VAR(const_var)
  REFLECT_STATIC_VAR(static_var)

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
  var = 1;
  QCOMPARE((reflect::utils::invoke<Type, reflect::ObjVars>(0_c, *this)), 1);
  QCOMPARE((reflect::utils::invoke<Type, reflect::ObjVars>(1_c, *this)), 2);
}

void VariableTest::get_static_variable() {
  static_var = 3;
  QCOMPARE((reflect::utils::invoke<Type, reflect::StaticVars>(0_c, *this)), 3);
}

void VariableTest::get_variable() {
  var = 1;
  static_var = 3;
  QCOMPARE((reflect::utils::invoke<Type, reflect::AllVars>(0_c, *this)), 1);
  QCOMPARE((reflect::utils::invoke<Type, reflect::AllVars>(1_c, *this)), 2);
  QCOMPARE((reflect::utils::invoke<Type, reflect::AllVars>(2_c, *this)), 3);
}
void VariableTest::set_obj_variable() {
  reflect::utils::invoke<Type, reflect::ObjVars>(0_c, *this) = 11;
  QCOMPARE((reflect::utils::invoke<Type, reflect::ObjVars>(0_c, *this)), 11);
}

void VariableTest::set_static_variable() {
  reflect::utils::invoke<Type, reflect::StaticVars>(0_c) = 12;
  QCOMPARE((reflect::utils::invoke<Type, reflect::StaticVars>(0_c)), 12);
}

void VariableTest::set_variable() {
  reflect::utils::invoke<Type, reflect::AllVars>(0_c, *this) = 13;
  reflect::utils::invoke<Type, reflect::AllVars>(2_c, *this) = 14;
  QCOMPARE((reflect::utils::invoke<Type, reflect::AllVars>(0_c, *this)), 13);
  QCOMPARE((reflect::utils::invoke<Type, reflect::AllVars>(2_c, *this)), 14);
}

QTEST_MAIN(VariableTest)
#include "main.moc"
