#include <QtTest>
#include <reflect.hpp>

using namespace boost::hana::literals;

class Base {
public:
  OUT_METAINFO(Base)
  int a{0};
};

class ConstChild : public Base {
public:
  OUT_METAINFO(ConstChild)
  const int c = 100;
  int bar() const { return c; }
};

class StaticChild : public ConstChild {
public:
  OUT_METAINFO(StaticChild)
  static std::string static_var;
  static std::string &baz() { return static_var; }
};

std::string StaticChild::static_var = "";

class InClassChild : public StaticChild {
public:
  IN_METAINFO(InClassChild, StaticChild)
};

METAINFO(Base)
REFLECT_OBJ_VAR(a)
END_METAINFO
METAINFO(ConstChild, Base)
REFLECT_OBJ_VAR(c)
REFLECT_CONST_OBJ_MTD(bar)
END_METAINFO
METAINFO(StaticChild, ConstChild)
REFLECT_STATIC_VAR(static_var)
REFLECT_STATIC_MTD(baz)
END_METAINFO

class InheritanceTest : public QObject {
  Q_OBJECT
private slots:
  void parents_count();
  void vars_count();
  void methods_count();
  void parents_types();
  void vars_types();
  void parents_names();
  void get_from_parents();
  void set_from_parents();
};

void InheritanceTest::parents_count() {
  QCOMPARE(reflect::utils::parents_count<Base>(), 0_c);
  QCOMPARE(reflect::utils::parents_count<ConstChild>(), 1_c);
  QCOMPARE(reflect::utils::parents_count<StaticChild>(), 2_c);
  QCOMPARE(reflect::utils::parents_count<InClassChild>(), 3_c);
}
void InheritanceTest::vars_count() {
  QCOMPARE((reflect::utils::count<Base, reflect::AllVars>()), 1_c);
  QCOMPARE((reflect::utils::count<ConstChild, reflect::AllVars>()), 2_c);
  QCOMPARE((reflect::utils::count<StaticChild, reflect::AllVars>()), 3_c);
  QCOMPARE((reflect::utils::count<InClassChild, reflect::AllVars>()), 3_c);
}
void InheritanceTest::methods_count() {
  QCOMPARE((reflect::utils::count<Base, reflect::AllMethods>()), 0_c);
  QCOMPARE((reflect::utils::count<ConstChild, reflect::AllMethods>()), 1_c);
  QCOMPARE((reflect::utils::count<StaticChild, reflect::AllMethods>()), 2_c);
  QCOMPARE((reflect::utils::count<InClassChild, reflect::AllMethods>()), 2_c);
}
void InheritanceTest::parents_types() {
  QCOMPARE(reflect::utils::parents_types<Base>(), ::boost::hana::tuple_t<>);
  QCOMPARE(reflect::utils::parents_types<ConstChild>(),
           (::boost::hana::tuple_t<Base>));
  QCOMPARE(reflect::utils::parents_types<StaticChild>(),
           (::boost::hana::tuple_t<ConstChild, Base>));
  QCOMPARE(reflect::utils::parents_types<InClassChild>(),
           (::boost::hana::tuple_t<StaticChild, ConstChild, Base>));
}
void InheritanceTest::vars_types() {
  QCOMPARE(reflect::utils::result_type<ConstChild>(
               reflect::utils::find_by_name_first<ConstChild>("a"_s)),
           ::boost::hana::type_c<int &&>);
  QCOMPARE(reflect::utils::result_type<StaticChild>(
               reflect::utils::find_by_name_first<StaticChild>("a"_s)),
           ::boost::hana::type_c<int &&>);
  QCOMPARE(reflect::utils::result_type<StaticChild>(
               reflect::utils::find_by_name_first<StaticChild>("c"_s)),
           ::boost::hana::type_c<const int &&>);
  QCOMPARE(
      reflect::utils::result_type<InClassChild>(
          reflect::utils::find_by_name_first<InClassChild>("static_var"_s)),
      ::boost::hana::type_c<std::string &&>);
}

void InheritanceTest::parents_names() {
  constexpr auto get_names = [](auto &&obj) constexpr {
    return reflect::utils::class_name<typename decltype(+obj)::type>();
  };
  constexpr auto tup1 = reflect::metautils::for_each(
      reflect::utils::parents_types<Base>(), get_names);
  constexpr auto tup2 = reflect::metautils::for_each(
      reflect::utils::parents_types<ConstChild>(), get_names);
  constexpr auto tup3 = reflect::metautils::for_each(
      reflect::utils::parents_types<StaticChild>(), get_names);
  constexpr auto tup4 = reflect::metautils::for_each(
      reflect::utils::parents_types<InClassChild>(), get_names);
  QCOMPARE(tup1, boost::hana::make_tuple());
  QCOMPARE(tup2, boost::hana::make_tuple("Base"_s));
  QCOMPARE(tup3, boost::hana::make_tuple("ConstChild"_s, "Base"_s));
  QCOMPARE(tup4,
           boost::hana::make_tuple("StaticChild"_s, "ConstChild"_s, "Base"_s));
}

void InheritanceTest::get_from_parents() {
  InClassChild obj;
  obj.a = 10;
  QCOMPARE(
      (reflect::utils::get<InClassChild, reflect::AllVars>(
          reflect::utils::find_by_name_first<InClassChild, reflect::AllVars>(
              "a"_s),
          obj)),
      10);
  QCOMPARE(
      (reflect::utils::get<InClassChild, reflect::ConstMethods>(
          reflect::utils::find_by_name_first<InClassChild,
                                             reflect::ConstMethods>("bar"_s),
          obj)),
      100);
}

void InheritanceTest::set_from_parents() {
  InClassChild obj;
  reflect::utils::set<InClassChild, reflect::AllVars>(
      reflect::utils::find_by_name_first<InClassChild, reflect::AllVars>("a"_s),
      20, obj);
  QCOMPARE(obj.a, 20);
  auto res = reflect::utils::set<InClassChild, reflect::StaticMethods>(
      reflect::utils::find_by_name_first<InClassChild, reflect::StaticMethods>(
          "baz"_s),
      "Hello test");
  QCOMPARE(res, boost::hana::bool_c<true>);
  QCOMPARE(InClassChild::static_var, std::string("Hello test"));
}

QTEST_MAIN(InheritanceTest)
#include "main.moc"
