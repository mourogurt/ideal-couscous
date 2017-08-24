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
  void gets_variable();
  void sets_variable();
  void gets_tuple_args_variable();
  void sets_tuple_args_variable();
};

int VariableTest::static_var = 3;

void VariableTest::get_obj_variable() {
  var = 1;
  QCOMPARE((reflect::utils::get<Type, reflect::ObjVars>(0_c, *this)), 1);
  QCOMPARE((reflect::utils::get<Type, reflect::ObjVars>(1_c, *this)), 2);
}

void VariableTest::get_static_variable() {
  static_var = 3;
  QCOMPARE((reflect::utils::get<Type, reflect::StaticVars>(0_c, *this)), 3);
}

void VariableTest::get_variable() {
  var = 1;
  static_var = 3;
  QCOMPARE((reflect::utils::get<Type, reflect::AllVars>(0_c, *this)), 1);
  QCOMPARE((reflect::utils::get<Type, reflect::AllVars>(1_c, *this)), 2);
  QCOMPARE((reflect::utils::get<Type, reflect::AllVars>(2_c, *this)), 3);
}
void VariableTest::set_obj_variable() {
  reflect::utils::set<Type, reflect::ObjVars>(0_c, 11, *this);
  QCOMPARE((reflect::utils::get<Type, reflect::ObjVars>(0_c, *this)), 11);
  QCOMPARE(typeid(reflect::utils::set<Type, reflect::ObjVars>(0_c, 11, *this)),
           typeid(::boost::hana::bool_c<true>));
}

void VariableTest::set_static_variable() {
  reflect::utils::set<Type, reflect::StaticVars>(0_c, 12);
  QCOMPARE((reflect::utils::get<Type, reflect::StaticVars>(0_c)), 12);
  QCOMPARE(typeid(reflect::utils::set<Type, reflect::StaticVars>(0_c, 12)),
           typeid(::boost::hana::bool_c<true>));
}

void VariableTest::set_variable() {
  reflect::utils::set<Type, reflect::AllVars>(0_c, 13, *this);
  reflect::utils::set<Type, reflect::AllVars>(2_c, 14, *this);
  QCOMPARE((reflect::utils::get<Type, reflect::AllVars>(0_c, *this)), 13);
  QCOMPARE((reflect::utils::get<Type, reflect::AllVars>(2_c, *this)), 14);
}

void VariableTest::gets_variable() {
  var = 1;
  static_var = 3;
  auto tup = reflect::utils::gets<Type, reflect::AllVars>(
      reflect::metautils::gen_inds_tup<decltype(
          reflect::utils::count<Type, reflect::AllVars>())>(),
      *this);
  QCOMPARE(tup, ::boost::hana::make_tuple(1, 2, 3));
  auto tup2 = reflect::utils::gets<Type, reflect::AllVars>(
      reflect::metautils::gen_inds_tup<decltype(
          reflect::utils::count<Type, reflect::AllVars>())>());
  QCOMPARE(tup2, ::boost::hana::make_tuple(boost::hana::nothing,
                                           boost::hana::nothing, 3));
}

void VariableTest::sets_variable() {
  var = 1;
  static_var = 3;
  auto tup = reflect::utils::sets<Type, reflect::AllVars>(
      reflect::metautils::gen_inds_tup<decltype(
          reflect::utils::count<Type, reflect::AllVars>())>(),
      100, *this);
  QCOMPARE(var, 100);
  QCOMPARE(static_var, 100);
  QCOMPARE(tup, boost::hana::make_tuple(boost::hana::bool_c<true>,
                                        boost::hana::nothing,
                                        boost::hana::bool_c<true>));
}

void VariableTest::gets_tuple_args_variable() {
  var = 1;
  static_var = 3;
  constexpr auto all_vars_size =
      reflect::utils::count<Type, reflect::AllVars>();
  auto tup = reflect::utils::gets_tuple_args<Type, reflect::AllVars>(
      reflect::metautils::gen_inds_tup<decltype(all_vars_size)>(),
      boost::hana::replicate<boost::hana::tuple_tag>(
          boost::hana::make_tuple(std::ref(*this)), all_vars_size));
  QCOMPARE(tup, ::boost::hana::make_tuple(1, 2, 3));
  auto tup2 = reflect::utils::gets_tuple_args<Type, reflect::AllVars>(
      reflect::metautils::gen_inds_tup<decltype(all_vars_size)>(),
      boost::hana::replicate<boost::hana::tuple_tag>(
          boost::hana::make_tuple(std::ref(*this)), all_vars_size));
  QCOMPARE(tup2, ::boost::hana::make_tuple(1, 2, 3));
}

void VariableTest::sets_tuple_args_variable() {
  var = 1;
  static_var = 3;
  constexpr auto all_vars_size =
      reflect::utils::count<Type, reflect::AllVars>();
  constexpr auto indices =
      reflect::metautils::gen_inds_tup<decltype(all_vars_size)>();
  auto params = boost::hana::replicate<boost::hana::tuple_tag>(
      boost::hana::make_tuple(std::ref(*this)), all_vars_size);
  constexpr auto values = boost::hana::make_tuple(100, 200, 300);
  auto tup = reflect::utils::sets_tuple_args<Type, reflect::AllVars>(
      indices, values, params);
  QCOMPARE(tup, boost::hana::make_tuple(boost::hana::bool_c<true>,
                                        boost::hana::nothing,
                                        boost::hana::bool_c<true>));
  QCOMPARE(var, 100);
  QCOMPARE(static_var, 300);
  auto params2 = boost::hana::concat(
      boost::hana::replicate<boost::hana::tuple_tag>(
          boost::hana::make_tuple(std::ref(*this)), all_vars_size - 1_c),
      boost::hana::make_tuple(boost::hana::nothing));
  QCOMPARE(typeid(reflect::utils::sets_tuple_args<Type, reflect::AllVars>(
               indices, values, params2)),
           typeid(boost::hana::make_tuple(boost::hana::bool_c<true>,
                                          boost::hana::nothing,
                                          boost::hana::bool_c<true>)));
}

QTEST_MAIN(VariableTest)
#include "main.moc"
