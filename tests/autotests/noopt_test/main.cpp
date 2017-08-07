#include <QtTest>
#include <boost/hana/repeat.hpp>
#include <iostream>
#include <reflect.hpp>

using namespace boost::hana::literals;

class NoOptionalTest : public QObject {
  Q_OBJECT
public:
  int var;
  static int static_var;
  int &foo(int a) {
    var += a;
    return var;
  }
  static double static_foo(int, double, const std::string &) { return 4.2; }
  using Type = NoOptionalTest;
  OUT_METAINFO(NoOptionalTest)
private slots:
  void check_reflected();
  void class_name_method();
  void vars_names();
  void method_names();
  void counter_vars();
  void counter_method();
  void find_vars();
  void find_methods();
  void get_types();
  void check_invoke();
  void check_set();
  void get();
  void set();
  void gets();
  void sets();
  void gets_tuple_args();
  void sets_tuple_args();
};

int NoOptionalTest::static_var{0};

METAINFO(NoOptionalTest)
REFLECT_OBJ_VAR(var)
REFLECT_STATIC_VAR(static_var)
REFLECT_OBJ_MTD(foo, int)
REFLECT_STATIC_MTD(static_foo, int, double, const std::string &)
END_METAINFO

void NoOptionalTest::check_reflected() {
  QCOMPARE(reflect::utils::check_reflected<Type>(), boost::hana::bool_c<true>);
}

void NoOptionalTest::class_name_method() {
  QCOMPARE(reflect::utils::class_name<Type>(), "NoOptionalTest"_s);
}

void NoOptionalTest::vars_names() {
  constexpr auto tup = reflect::metautils::for_each(
      reflect::metautils::gen_inds_tup<decltype(
          reflect::utils::count<Type, reflect::AllVars>())>(),
      [](auto &&id) {
        return reflect::utils::member_name<Type, reflect::AllVars>(id);
      });
  QCOMPARE(tup, boost::hana::make_tuple("var"_s, "static_var"_s));
}

void NoOptionalTest::method_names() {
  constexpr auto tup = reflect::metautils::for_each(
      reflect::metautils::gen_inds_tup<decltype(
          reflect::utils::count<Type, reflect::AllMethods>())>(),
      [](auto &&id) {
        return reflect::utils::member_name<Type, reflect::AllMethods>(id);
      });
  QCOMPARE(tup, boost::hana::make_tuple("foo"_s, "static_foo"_s));
}

void NoOptionalTest::counter_vars() {
  QCOMPARE((reflect::utils::count<Type, reflect::AllVars>()), 2_c);
}

void NoOptionalTest::counter_method() {
  QCOMPARE((reflect::utils::count<Type, reflect::AllMethods>()), 2_c);
}

void NoOptionalTest::find_vars() {
  QCOMPARE(
      (reflect::utils::find_by_name_first<Type, reflect::AllVars>("var"_s)),
      0_c);
  QCOMPARE((reflect::utils::find_by_name_first<Type, reflect::AllVars>(
               "static_var"_s)),
           1_c);
}

void NoOptionalTest::find_methods() {
  QCOMPARE(
      (reflect::utils::find_by_name<Type, reflect::AllMethods>("foo"_s)[0_c]),
      0_c);
  QCOMPARE((reflect::utils::find_by_name<Type, reflect::AllMethods>(
               "static_foo"_s)[0_c]),
           1_c);
}

void NoOptionalTest::get_types() {

  constexpr auto met_types = reflect::metautils::for_each(
      reflect::metautils::gen_inds_tup<decltype(
          reflect::utils::count<Type, reflect::AllMethods>())>(),
      [](auto &&index) {
        return reflect::utils::method_args<Type, reflect::AllMethods>(index);
      });
  QCOMPARE((boost::hana::make_tuple(
               boost::hana::tuple_t<NoOptionalTest, int>,
               boost::hana::tuple_t<int, double, const std::string &>)),
           met_types);
  constexpr auto ret_met_types = reflect::metautils::for_each(
      reflect::metautils::gen_inds_tup<decltype(
          reflect::utils::count<Type, reflect::AllMethods>())>(),
      [](auto &&index) {
        return reflect::utils::result_type<Type, reflect::AllMethods>(index);
      });
  QCOMPARE(ret_met_types, (boost::hana::tuple_t<int &, double>));
  constexpr auto ret_var_types = reflect::metautils::for_each(
      reflect::metautils::gen_inds_tup<decltype(
          reflect::utils::count<Type, reflect::AllVars>())>(),
      [](auto &&index) {
        return reflect::utils::result_type<Type, reflect::AllVars>(index);
      });
  QCOMPARE(ret_var_types, (boost::hana::tuple_t<int &&, int &&>));
}

void NoOptionalTest::check_invoke() {
  QCOMPARE(
      (reflect::utils::check_invoke<Type, reflect::AllVars, Type>(
          reflect::utils::find_by_name<Type, reflect::AllVars>("var"_s)[0_c])),
      boost::hana::bool_c<true>);
  QCOMPARE((reflect::utils::check_invoke<Type, reflect::AllVars>(
               reflect::utils::find_by_name<Type, reflect::AllVars>(
                   "static_var"_s)[0_c])),
           boost::hana::bool_c<true>);
  QCOMPARE((reflect::utils::check_invoke<Type, reflect::AllMethods, Type, int>(
               reflect::utils::find_by_name<Type, reflect::AllMethods>(
                   "foo"_s)[0_c])),
           boost::hana::bool_c<true>);
  QCOMPARE((reflect::utils::check_invoke<Type, reflect::AllMethods, int, double,
                                         const std::string &>(
               reflect::utils::find_by_name<Type, reflect::AllMethods>(
                   "static_foo"_s)[0_c])),
           boost::hana::bool_c<true>);
}

void NoOptionalTest::check_set() {
  QCOMPARE((reflect::utils::check_set<Type, reflect::AllVars, int, Type>(0_c)),
           boost::hana::bool_c<true>);
  QCOMPARE((reflect::utils::check_set<Type, reflect::AllVars, int &&>(1_c)),
           boost::hana::bool_c<true>);
  QCOMPARE((reflect::utils::check_set<Type, reflect::AllMethods, int &&, Type,
                                      int &&>(0_c)),
           boost::hana::bool_c<true>);
}

void NoOptionalTest::get() {
  var = 1337;
  QCOMPARE((reflect::utils::get<Type, reflect::ObjVars>(0_c, std::ref(*this))),
           1337);
  static_var = 7331;
  QCOMPARE((reflect::utils::get<Type, reflect::StaticVars>(0_c)), 7331);
  var = 42;
  QCOMPARE((reflect::utils::get<Type, reflect::ObjMethods>(0_c, *this, 3)), 45);
}

void NoOptionalTest::set() {
  reflect::utils::set<Type, reflect::ObjVars>(0_c, 23, std::ref(*this));
  QCOMPARE(var, 23);
  reflect::utils::set<Type, reflect::StaticVars>(0_c, 13);
  QCOMPARE(static_var, 13);
  reflect::utils::set<Type, reflect::ObjMethods>(0_c, 20, std::ref(*this), 0);
  QCOMPARE(var, 20);
}

void NoOptionalTest::gets() {
  var = 5;
  static_var = 10;
  auto tup = reflect::utils::gets<Type, reflect::AllVars>(
      boost::hana::tuple_c<long long, 0, 1>, std::ref(*this));
  QCOMPARE(tup, boost::hana::make_tuple(5, 10));
  auto func_tup = reflect::utils::gets<Type, reflect::AllMethods>(
      boost::hana::tuple_c<long long, 0, 1>, std::ref(*this), 10);
  QCOMPARE(func_tup, boost::hana::make_tuple(15, boost::hana::nothing));
}

void NoOptionalTest::sets() {
  reflect::utils::sets<Type, reflect::AllVars>(
      boost::hana::tuple_c<long long, 0, 1>, 100, std::ref(*this));
  QCOMPARE(var, 100);
  QCOMPARE(static_var, 100);
  reflect::utils::sets<Type, reflect::AllMethods>(
      boost::hana::tuple_c<long long, 0, 1>, 50, std::ref(*this), 0);
  QCOMPARE(var, 50);
}

void NoOptionalTest::gets_tuple_args() {
  var = 1;
  static_var = 2;
  auto var_tup = reflect::utils::gets_tuple_args<Type, reflect::AllVars>(
      boost::hana::tuple_c<long long, 0, 1>,
      boost::hana::make_tuple(boost::hana::make_tuple(std::ref(*this)),
                              boost::hana::make_tuple()));
  QCOMPARE(var_tup, boost::hana::make_tuple(1, 2));
  auto fun_tup = reflect::utils::gets_tuple_args<Type, reflect::AllMethods>(
      boost::hana::tuple_c<long long, 0, 1>,
      boost::hana::make_tuple(boost::hana::make_tuple(std::ref(*this), 10),
                              boost::hana::make_tuple(0, 0.0, "")));
  QCOMPARE(fun_tup, boost::hana::make_tuple(11, 4.2));
}

void NoOptionalTest::sets_tuple_args() {
  reflect::utils::sets_tuple_args<Type, reflect::AllVars>(
      boost::hana::tuple_c<long long, 0, 1>, boost::hana::make_tuple(25, 35),
      boost::hana::make_tuple(boost::hana::make_tuple(std::ref(*this)),
                              boost::hana::make_tuple()));
  QCOMPARE(var, 25);
  QCOMPARE(static_var, 35);
}

QTEST_MAIN(NoOptionalTest)
#include "main.moc"
