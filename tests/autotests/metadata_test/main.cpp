#include <QtTest>
#include <reflect.hpp>

using namespace boost::hana::literals;

class MetadataTest : public QObject {
  Q_OBJECT
public:
  int var1, var2;
  static int static_var;
  int &foo(int);
  void foo(int, double) const;
  static double static_foo(int, double, const std::string &);
  using Type = MetadataTest;
  OUT_METAINFO(MetadataTest)
private slots:
  void check_reflected();
  void class_name_method();
  void vars_obj_names();
  void vars_static_names();
  void vars_names();
  void method_obj_names();
  void method_const_names();
  void method_static_names();
  void method_names();
  void counter_obj_vars();
  void counter_static_vars();
  void counter_vars();
  void counter_obj_method();
  void counter_const_method();
  void counter_static_method();
  void counter_method();
  void find_obj_vars();
  void find_static_vars();
  void find_vars();
  void find_obj_methods();
  void find_const_methods();
  void find_static_methods();
  void find_methods();
  void get_obj_method_types();
  void get_const_method_types();
  void get_static_method_types();
  void get_method_types();
  void check_obj_invoke();
  void check_const_invoke();
  void check_static_invoke();
  void check_invoke();
  void check_set();
};

METAINFO(MetadataTest)
REFLECT_OBJ_VAR(var1)
REFLECT_OBJ_VAR(var2)
REFLECT_STATIC_VAR(static_var)
REFLECT_OBJ_MTD(foo, int)
REFLECT_CONST_OBJ_MTD(foo, int, double)
REFLECT_STATIC_MTD(static_foo, int, double, const std::string &)
END_METAINFO

void MetadataTest::check_reflected() {
  QCOMPARE(reflect::utils::check_reflected<Type>(), boost::hana::bool_c<true>);
  QCOMPARE(reflect::utils::check_reflected<int>(), boost::hana::bool_c<false>);
}

void MetadataTest::class_name_method() {
  QVERIFY(reflect::utils::class_name<Type>().value() == "MetadataTest"_s);
  QVERIFY(reflect::utils::class_name<int>() == boost::hana::nothing);
}
void MetadataTest::vars_obj_names() {
  auto bool_tup = reflect::metautils::for_each(
      reflect::metautils::gen_inds_tup<decltype(
          reflect::utils::count<Type, reflect::ObjVars>().value())>(),
      [tuple(boost::hana::make_tuple("var1"_s, "var2"_s))](auto &&index) {
        return (reflect::utils::member_name<Type, reflect::ObjVars>(index)
                    .value() == boost::hana::at(tuple, index));
      });
  QVERIFY(bool_tup == boost::hana::make_tuple(true, true));
  QVERIFY((reflect::utils::member_name<Type, reflect::ObjVars>(
              reflect::utils::count<Type, reflect::ObjVars>().value())) ==
          boost::hana::nothing);
  QVERIFY((reflect::utils::member_name<Type, int>(0_c)) ==
          boost::hana::nothing);
}

void MetadataTest::vars_static_names() {
  auto bool_tup = reflect::metautils::for_each(
      reflect::metautils::gen_inds_tup<decltype(
          reflect::utils::count<Type, reflect::StaticVars>().value())>(),
      [tuple(boost::hana::make_tuple("static_var"_s))](auto &&index) {
        return (reflect::utils::member_name<Type, reflect::StaticVars>(index)
                    .value() == boost::hana::at(tuple, index));
      });
  QVERIFY(bool_tup == boost::hana::make_tuple(true));
  QVERIFY((reflect::utils::member_name<Type, reflect::StaticVars>(
              reflect::utils::count<Type, reflect::StaticVars>())) ==
          boost::hana::nothing);
}

void MetadataTest::vars_names() {
  auto bool_tup = reflect::metautils::for_each(
      reflect::metautils::gen_inds_tup<decltype(
          reflect::utils::count<Type, reflect::AllVars>().value())>(),
      [tuple(boost::hana::make_tuple("var1"_s, "var2"_s, "static_var"_s))](
          auto &&index) {
        return (reflect::utils::member_name<Type, reflect::AllVars>(index)
                    .value() == boost::hana::at(tuple, index));
      });
  QVERIFY(bool_tup == boost::hana::make_tuple(true, true, true));
  QVERIFY((reflect::utils::member_name<Type, reflect::AllVars>(
              reflect::utils::count<Type, reflect::AllVars>().value())) ==
          boost::hana::nothing);
}

void MetadataTest::method_obj_names() {
  auto bool_tup = reflect::metautils::for_each(
      reflect::metautils::gen_inds_tup<decltype(
          reflect::utils::count<Type, reflect::ObjMethods>().value())>(),
      [tuple(boost::hana::make_tuple("foo"_s, "foo"_s))](auto &&index) {
        return (reflect::utils::member_name<Type, reflect::ObjMethods>(index)
                    .value() == boost::hana::at(tuple, index));
      });
  QVERIFY(bool_tup == boost::hana::make_tuple(true, true));
  QVERIFY((reflect::utils::member_name<Type, reflect::ObjMethods>(
              reflect::utils::count<Type, reflect::ObjMethods>().value())) ==
          boost::hana::nothing);
}

void MetadataTest::method_const_names() {
  auto bool_tup = reflect::metautils::for_each(
      reflect::metautils::gen_inds_tup<decltype(
          reflect::utils::count<Type, reflect::ConstMethods>().value())>(),
      [tuple(boost::hana::make_tuple("foo"_s))](auto &&index) {
        return (reflect::utils::member_name<Type, reflect::ConstMethods>(index)
                    .value() == boost::hana::at(tuple, index));
      });
  QVERIFY(bool_tup == boost::hana::make_tuple(true));
  QVERIFY((reflect::utils::member_name<Type, reflect::ConstMethods>(
              reflect::utils::count<Type, reflect::ConstMethods>().value())) ==
          boost::hana::nothing);
}

void MetadataTest::method_static_names() {
  auto bool_tup = reflect::metautils::for_each(
      reflect::metautils::gen_inds_tup<decltype(
          reflect::utils::count<Type, reflect::StaticMethods>().value())>(),
      [tuple(boost::hana::make_tuple("static_foo"_s))](auto &&index) {
        return (reflect::utils::member_name<Type, reflect::StaticMethods>(index)
                    .value() == boost::hana::at(tuple, index));
      });
  QVERIFY(bool_tup == boost::hana::make_tuple(true));
  QVERIFY((reflect::utils::member_name<Type, reflect::StaticMethods>(
              reflect::utils::count<Type, reflect::StaticMethods>().value())) ==
          boost::hana::nothing);
}

void MetadataTest::method_names() {
  auto bool_tup = reflect::metautils::for_each(
      reflect::metautils::gen_inds_tup<decltype(
          reflect::utils::count<Type, reflect::AllMethods>().value())>(),
      [tuple(boost::hana::make_tuple("foo"_s, "foo"_s, "static_foo"_s))](
          auto &&index) {
        return (reflect::utils::member_name<Type, reflect::AllMethods>(index)
                    .value() == boost::hana::at(tuple, index));
      });
  QVERIFY(bool_tup == boost::hana::make_tuple(true, true, true));
  QVERIFY((reflect::utils::member_name<Type, reflect::AllMethods>(
              reflect::utils::count<Type, reflect::AllMethods>().value())) ==
          boost::hana::nothing);
}

void MetadataTest::counter_obj_vars() {
  QVERIFY((reflect::utils::count<Type, reflect::ObjVars>()) ==
          boost::hana::just(2_c));
}

void MetadataTest::counter_static_vars() {
  QVERIFY((reflect::utils::count<Type, reflect::StaticVars>()) ==
          boost::hana::just(1_c));
}

void MetadataTest::counter_vars() {
  QVERIFY((reflect::utils::count<Type, reflect::AllVars>()) ==
          boost::hana::just(3_c));
}

void MetadataTest::counter_obj_method() {
  QVERIFY((reflect::utils::count<Type, reflect::ObjMethods>()) ==
          boost::hana::just(2_c));
}

void MetadataTest::counter_const_method() {
  QVERIFY((reflect::utils::count<Type, reflect::ConstMethods>()) ==
          boost::hana::just(1_c));
}

void MetadataTest::counter_static_method() {
  QVERIFY((reflect::utils::count<Type, reflect::StaticMethods>()) ==
          boost::hana::just(1_c));
}

void MetadataTest::counter_method() {
  QVERIFY((reflect::utils::count<Type, reflect::AllMethods>()) ==
          boost::hana::just(3_c));
  QVERIFY((reflect::utils::count<int, reflect::ObjVars>()) ==
          boost::hana::nothing);
  QVERIFY((reflect::utils::count<Type, int>()) == boost::hana::nothing);
}

void MetadataTest::find_obj_vars() {
  auto tuple = reflect::metautils::for_each(
      boost::hana::make_tuple("var1"_s, "var2"_s), [](auto &&name) {
        return reflect::utils::find_by_name_first<Type, reflect::ObjVars>(name)
            .value();
      });
  QVERIFY(tuple == (boost::hana::tuple_c<long long, 0, 1>));
}

void MetadataTest::find_static_vars() {
  auto tuple = reflect::metautils::for_each(
      boost::hana::make_tuple("static_var"_s), [](auto &&name) {
        return reflect::utils::find_by_name_first<Type, reflect::StaticVars>(
                   name)
            .value();
      });
  QVERIFY(tuple == (boost::hana::tuple_c<long long, 0>));
}

void MetadataTest::find_vars() {
  auto tuple = reflect::metautils::for_each(
      boost::hana::make_tuple("var1"_s, "var2"_s, "static_var"_s),
      [](auto &&name) {
        return reflect::utils::find_by_name_first<Type, reflect::AllVars>(name)
            .value();
      });
  QVERIFY(tuple == (boost::hana::tuple_c<long long, 0, 1, 2>));
  QVERIFY((reflect::utils::find_by_name<Type, reflect::AllVars>("nothing"_s)) ==
          boost::hana::just(boost::hana::make_tuple()));
  QVERIFY((reflect::utils::find_by_name<Type, int>("var1"_s)) ==
          boost::hana::nothing);
  QVERIFY((reflect::utils::find_by_name<int, reflect::AllVars>("var1"_s)) ==
          boost::hana::nothing);
}

void MetadataTest::find_obj_methods() {
  auto tuple =
      reflect::metautils::merge_tuple_of_tuples(reflect::metautils::for_each(
          boost::hana::make_tuple("foo"_s), [](auto &&name) {
            return reflect::utils::find_by_name<Type, reflect::ObjMethods>(name)
                .value();
          }));
  QVERIFY(tuple == (boost::hana::tuple_c<long long, 0, 1>));
}

void MetadataTest::find_const_methods() {
  auto tuple =
      reflect::metautils::merge_tuple_of_tuples(reflect::metautils::for_each(
          boost::hana::make_tuple("foo"_s), [](auto &&name) {
            return reflect::utils::find_by_name<Type, reflect::ConstMethods>(
                       name)
                .value();
          }));
  QVERIFY(tuple == (boost::hana::tuple_c<long long, 0>));
}

void MetadataTest::find_static_methods() {
  auto tuple =
      reflect::metautils::merge_tuple_of_tuples(reflect::metautils::for_each(
          boost::hana::make_tuple("static_foo"_s), [](auto &&name) {
            return reflect::utils::find_by_name<Type, reflect::StaticMethods>(
                       name)
                .value();
          }));
  QVERIFY(tuple == (boost::hana::tuple_c<long long, 0>));
}

void MetadataTest::find_methods() {
  auto tuple =
      reflect::metautils::merge_tuple_of_tuples(reflect::metautils::for_each(
          boost::hana::make_tuple("foo"_s, "static_foo"_s), [](auto &&name) {
            return reflect::utils::find_by_name<Type, reflect::AllMethods>(name)
                .value();
          }));
  QVERIFY(tuple == (boost::hana::tuple_c<long long, 0, 1, 2>));
}

void MetadataTest::get_obj_method_types() {
  auto tuple = reflect::metautils::for_each(
      reflect::metautils::gen_inds_tup<decltype(
          reflect::utils::count<Type, reflect::ObjMethods>().value())>(),
      [](auto &&index) {
        return reflect::utils::method_args<Type, reflect::ObjMethods>(index)
            .value();
      });
  QVERIFY(tuple ==
          ::boost::hana::make_tuple(::boost::hana::tuple_t<Type, int>,
                                    ::boost::hana::tuple_t<Type, int, double>));
  auto res_tuple = reflect::metautils::for_each(
      reflect::metautils::gen_inds_tup<decltype(
          reflect::utils::count<Type, reflect::ObjMethods>().value())>(),
      [](auto &&index) {
        return reflect::utils::result_type<Type, reflect::ObjMethods>(index)
            .value();
      });
  QVERIFY(res_tuple == (::boost::hana::tuple_t<int &, void>));
}

void MetadataTest::get_const_method_types() {
  auto tuple = reflect::metautils::for_each(
      reflect::metautils::gen_inds_tup<decltype(
          reflect::utils::count<Type, reflect::ConstMethods>().value())>(),
      [](auto &&index) {
        return reflect::utils::method_args<Type, reflect::ConstMethods>(index)
            .value();
      });
  QVERIFY(tuple ==
          ::boost::hana::make_tuple(::boost::hana::tuple_t<Type, int, double>));
  auto res_tuple = reflect::metautils::for_each(
      reflect::metautils::gen_inds_tup<decltype(
          reflect::utils::count<Type, reflect::ConstMethods>().value())>(),
      [](auto &&index) {
        return reflect::utils::result_type<Type, reflect::ConstMethods>(index)
            .value();
      });
  QVERIFY(res_tuple == (::boost::hana::tuple_t<void>));
}

void MetadataTest::get_static_method_types() {
  auto tuple = reflect::metautils::for_each(
      reflect::metautils::gen_inds_tup<decltype(
          reflect::utils::count<Type, reflect::StaticMethods>().value())>(),
      [](auto &&index) {
        return reflect::utils::method_args<Type, reflect::StaticMethods>(index)
            .value();
      });
  QVERIFY(tuple ==
          ::boost::hana::make_tuple(
              ::boost::hana::tuple_t<int, double, const std::string &>));
  auto res_tuple = reflect::metautils::for_each(
      reflect::metautils::gen_inds_tup<decltype(
          reflect::utils::count<Type, reflect::StaticMethods>().value())>(),
      [](auto &&index) {
        return reflect::utils::result_type<Type, reflect::StaticMethods>(index)
            .value();
      });
  QVERIFY(res_tuple == (::boost::hana::tuple_t<double>));
}

void MetadataTest::get_method_types() {
  auto tuple = reflect::metautils::for_each(
      reflect::metautils::gen_inds_tup<decltype(
          reflect::utils::count<Type, reflect::AllMethods>().value())>(),
      [](auto &&index) {
        return reflect::utils::method_args<Type, reflect::AllMethods>(index)
            .value();
      });
  QVERIFY(tuple ==
          ::boost::hana::make_tuple(
              ::boost::hana::tuple_t<Type, int>,
              ::boost::hana::tuple_t<Type, int, double>,
              ::boost::hana::tuple_t<int, double, const std::string &>));
  auto res_tuple = reflect::metautils::for_each(
      reflect::metautils::gen_inds_tup<decltype(
          reflect::utils::count<Type, reflect::AllMethods>().value())>(),
      [](auto &&index) {
        return reflect::utils::result_type<Type, reflect::AllMethods>(index)
            .value();
      });
  QVERIFY(res_tuple == (::boost::hana::tuple_t<int &, void, double>));
  QCOMPARE(
      typeid(reflect::utils::MethodInfo<Type, 0, reflect::AllMethods>::types),
      typeid(::boost::hana::tuple<Type, int>));
  QCOMPARE(typeid(reflect::utils::MethodInfo<Type, 0,
                                             reflect::AllMethods>::result_type),
           typeid(int &));
  QCOMPARE(
      typeid(
          reflect::utils::MethodInfo<Type, 0, reflect::AllMethods>::arg0_type),
      typeid(Type));
  QCOMPARE(
      typeid(
          reflect::utils::MethodInfo<Type, 0, reflect::AllMethods>::arg1_type),
      typeid(int));
  QCOMPARE(
      typeid(reflect::utils::MethodInfo<
             Type, reflect::utils::count<Type, reflect::AllMethods>().value(),
             reflect::AllMethods>::types),
      typeid(boost::hana::nothing));
  QCOMPARE(
      typeid(reflect::utils::MethodInfo<
             Type, reflect::utils::count<Type, reflect::AllMethods>().value(),
             reflect::AllMethods>::result_type),
      typeid(boost::hana::nothing));
  QCOMPARE(
      typeid(reflect::utils::MethodInfo<Type, 0, reflect::AllMethods>::arg_type<
             reflect::utils::MethodInfo<Type, 0,
                                        reflect::AllMethods>::args_count>),
      typeid(boost::hana::nothing));
}

void MetadataTest::check_obj_invoke() {
  QVERIFY(
      (reflect::utils::check_invoke<Type, reflect::ObjMethods, Type, int>(0_c))
          .value() == ::boost::hana::bool_c<true>);
  QVERIFY((reflect::utils::check_invoke<Type, reflect::ObjMethods, Type,
                                        std::string>(0_c))
              .value() == ::boost::hana::bool_c<false>);
  QVERIFY((reflect::utils::check_invoke<Type, reflect::ObjMethods, Type, int,
                                        double>(1_c))
              .value() == ::boost::hana::bool_c<true>);
  QVERIFY(
      (reflect::utils::check_invoke<Type, reflect::ObjMethods>(1_c)).value() ==
      ::boost::hana::bool_c<false>);
  QVERIFY((reflect::utils::check_invoke<Type, reflect::ObjVars, Type>(0_c))
              .value() == ::boost::hana::bool_c<true>);
  QVERIFY((reflect::utils::check_invoke<Type, reflect::ObjVars>(0_c)).value() ==
          ::boost::hana::bool_c<false>);
}

void MetadataTest::check_const_invoke() {
  QVERIFY((reflect::utils::check_invoke<Type, reflect::ConstMethods, Type, int,
                                        double>(0_c))
              .value() == ::boost::hana::bool_c<true>);
  QVERIFY(
      (reflect::utils::check_invoke<Type, reflect::ConstMethods, Type, double>(
           0_c))
          .value() == ::boost::hana::bool_c<false>);
  QVERIFY((reflect::utils::check_invoke<Type, reflect::ConstMethods, Type &,
                                        int, double>(0_c))
              .value() == ::boost::hana::bool_c<true>);
  QVERIFY((reflect::utils::check_invoke<Type, reflect::ConstMethods,
                                        const Type &, int, double>(0_c))
              .value() == ::boost::hana::bool_c<true>);
}

void MetadataTest::check_static_invoke() {
  QVERIFY((reflect::utils::check_invoke<Type, reflect::StaticMethods,
                                        ::std::reference_wrapper<Type>, int,
                                        double, const std::string &>(0_c))
              .value() == ::boost::hana::bool_c<true>);
  QVERIFY((reflect::utils::check_invoke<Type, reflect::StaticMethods, int,
                                        double, const std::string &>(0_c))
              .value() == ::boost::hana::bool_c<true>);
  QVERIFY(
      (reflect::utils::check_invoke<Type, reflect::StaticMethods, Type, double>(
           0_c))
          .value() == ::boost::hana::bool_c<false>);
  QVERIFY((reflect::utils::check_invoke<Type, reflect::StaticVars, Type>(0_c))
              .value() == ::boost::hana::bool_c<true>);
  QVERIFY(
      (reflect::utils::check_invoke<Type, reflect::StaticVars>(0_c)).value() ==
      ::boost::hana::bool_c<true>);
}

void MetadataTest::check_invoke() {
  QVERIFY((reflect::utils::check_invoke<Type, reflect::AllMethods, Type, int &>(
               0_c))
              .value() == ::boost::hana::bool_c<true>);
  QVERIFY(
      (reflect::utils::check_invoke<Type, reflect::AllMethods, Type, int &&>(
           0_c))
          .value() == ::boost::hana::bool_c<true>);
  QVERIFY(
      (reflect::utils::check_invoke<Type, reflect::AllMethods, Type, long>(0_c))
          .value() == ::boost::hana::bool_c<true>);
  QVERIFY((reflect::utils::check_invoke<Type, reflect::AllMethods>(
              reflect::utils::count<Type, reflect::AllMethods>().value())) ==
          ::boost::hana::nothing);
  QVERIFY((reflect::utils::check_invoke<int, reflect::AllMethods>(0_c)) ==
          ::boost::hana::nothing);
  QVERIFY((reflect::utils::check_invoke<Type, int>(0_c)) ==
          ::boost::hana::nothing);
}

void MetadataTest::check_set() {
  QCOMPARE(
      (reflect::utils::check_set<Type, reflect::AllMethods, int, Type, int>(
          0_c)),
      boost::hana::just(boost::hana::bool_c<true>));
  QCOMPARE(
      (reflect::utils::check_set<Type, reflect::AllMethods, int, Type, int>(
          1_c)),
      boost::hana::just(boost::hana::bool_c<false>));
  QCOMPARE((reflect::utils::check_set<Type, reflect::AllMethods, int, Type, int,
                                      double>(1_c)),
           boost::hana::just(boost::hana::bool_c<false>));
  QCOMPARE((reflect::utils::check_set<Type, int, int, Type, int>(0_c)),
           boost::hana::nothing);
  QCOMPARE(
      (reflect::utils::check_set<Type, reflect::AllMethods, int, Type, int>(0)),
      boost::hana::nothing);
  QCOMPARE((reflect::utils::check_set<int, reflect::AllMethods, int, Type, int>(
               0_c)),
           boost::hana::nothing);
}

QTEST_MAIN(MetadataTest)
#include "main.moc"
