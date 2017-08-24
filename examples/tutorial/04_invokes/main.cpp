#include <boost/hana/for_each.hpp>
#include <functional>
#include <iostream>

#include <reflect.hpp>

namespace couscous {
inline namespace utils { using namespace ::reflect::utils; }
inline namespace metautils { using namespace ::reflect::metautils; }
inline namespace reflect { using namespace ::reflect; }
}

namespace hana = boost::hana;
using namespace hana::literals;

class A {
  int num{0};
  int num2{1};

public:
  std::string str{"Hello couscous"};
  static int static_num;
  int &num_accessor() { return num; }
  int &num2_accessor() { return num2; }
  double const_method() const { return 5.; }
  static void static_method(std::size_t num, const std::string &str) {
    for (auto i = 0u; i < num; ++i)
      std::cout << str << std::endl;
  }
  void method1(int n) {
    std::cout << "method1 invoked with argument: " << n << std::endl;
  }
  void method2(int n) {
    std::cout << "method2 invoked with argument: " << n << std::endl;
  }
  void method3(int n) {
    std::cout << "method3 invoked with argument: " << n << std::endl;
  }
  void method1(const std::string &str) {
    std::cout << "method1 invoked with argument: " << str << std::endl;
  }
  void method2(const std::string &str) {
    std::cout << "method2 invoked with argument: " << str << std::endl;
  }
  void method3(const std::string &str) {
    std::cout << "method3 invoked with argument: " << str << std::endl;
  }
  OUT_METAINFO(A)
};

int A::static_num = 2;

METAINFO(A)
REFLECT_OBJ_VAR(str)
REFLECT_STATIC_VAR(static_num)
REFLECT_OBJ_MTD(num_accessor)
REFLECT_OBJ_MTD(num2_accessor)
REFLECT_CONST_OBJ_MTD(const_method)
REFLECT_STATIC_MTD(static_method, std::size_t, const std::string &)
REFLECT_OBJ_MTD(method1, int)
// Warning: Indexing of elements depends on the order of calling macroses
REFLECT_OBJ_MTD(method1, const std::string &)
REFLECT_OBJ_MTD(method2, int)
REFLECT_OBJ_MTD(method2, const std::string &)
REFLECT_OBJ_MTD(method3, int)
REFLECT_OBJ_MTD(method3, const std::string &)
END_METAINFO

int main() {
  // To check whether you can call a method with given args types you can use
  // "check_invoke".
  std::cout << std::boolalpha << "Can be invoked: "
            << couscous::check_invoke<A, couscous::ObjVars, A &>(0_c)
            << std::endl;
  std::cout << "Can be invoked: "
            << couscous::check_invoke<A, couscous::ObjVars, const A &>(0_c)
            << std::endl;
  // third index: method1 with int
  std::cout << "Can be invoked: "
            << couscous::check_invoke<A, couscous::ObjMethods, A &, int>(3_c)
            << std::endl;
  // You can't call non const obj methods with const object
  std::cout << "Can be invoked: "
            << couscous::check_invoke<A, couscous::ObjMethods, const A &, int>(
                   3_c)

            << std::endl;
  std::cout << "Can be invoked: "
            << couscous::check_invoke<A, couscous::ConstMethods, const A &>(0_c)

            << std::endl;
  // You can call static vars/methods with or without object parameter
  std::cout << "Can be invoked: "
            << couscous::check_invoke<A, couscous::StaticVars, A &>(0_c)
            << std::endl;
  std::cout << "Can be invoked: "
            << couscous::check_invoke<A, couscous::StaticVars>(0_c)
            << std::endl;
  // Implicit conversion are also supported
  std::cout << "Can be invoked: "
            << couscous::check_invoke<A, couscous::StaticMethods, A &, int,
                                      const std::string &>(0_c)

            << std::endl;
  std::cout << "Can be invoked: "
            << couscous::check_invoke<A, couscous::StaticMethods, unsigned,
                                      const std::string &>(0_c)

            << std::endl;
  // To check whether you can assign to a variable/method you can use
  // "check_set" <Type,Generator,rhs type,invokation types>
  std::cout << "Can be assigned: "
            << couscous::check_set<A, couscous::ObjVars, const char *, A &>(0_c)

            << std::endl;
  std::cout << "Can be assigned: "
            << couscous::check_set<A, couscous::StaticVars, long>(0_c)
            << std::endl;
  std::cout << "Can be assigned: "
            << couscous::check_set<A, couscous::ObjMethods, int &&, A>(0_c)
            << std::endl;
  hana::for_each(
      couscous::find_by_name<A, couscous::ObjMethods>("method1"_s),
      [](auto &&index) {
        std::cout << "Can be invoked: "
                  << couscous::check_invoke<A, couscous::ObjMethods, A &, int>(
                         index)

                  << "\tCan be invoked: "
                  << couscous::check_invoke<A, couscous::ObjMethods, A &,
                                            const std::string &>(index)

                  << std::endl;
      });
  hana::for_each(hana::tuple_c<long long, 0, 1>, [](auto &&index) {
    std::cout << "Can be assigned: "
              << couscous::check_set<A, couscous::ObjMethods, int &&, A &>(
                     index)

              << std::endl;
  });
  A obj;
  // To get var/result value of a method you can use "get"
  std::cout << hana::to<const char *>(
                   couscous::member_name<A, couscous::ObjVars>(0_c))
            << ": " << couscous::get<A, couscous::ObjVars>(0_c, obj)
            << std::endl;
  std::cout << hana::to<const char *>(
                   couscous::member_name<A, couscous::StaticVars>(0_c))
            << ": " << couscous::get<A, couscous::StaticVars>(0_c) << std::endl;
  std::cout << "num_accessor: "
            << couscous::get<A, couscous::ObjMethods>(
                   couscous::find_by_name<A, couscous::ObjMethods>(
                       "num_accessor"_s)[0_c],
                   obj)

            << std::endl;
  std::cout << "num2_accessor: "
            << couscous::get<A, couscous::ObjMethods>(
                   couscous::find_by_name<A, couscous::ObjMethods>(
                       "num2_accessor"_s)[0_c],
                   obj)

            << std::endl;
  // You can use "set" function if you want to assign a variable/method
  couscous::set<A, couscous::ObjVars>(0_c, "New string", obj);
  std::cout << hana::to<const char *>(
                   couscous::member_name<A, couscous::ObjVars>(0_c))
            << ": " << couscous::get<A, couscous::ObjVars>(0_c, obj)
            << std::endl;
  couscous::set<A, couscous::StaticVars>(0_c, 100);
  std::cout << hana::to<const char *>(
                   couscous::member_name<A, couscous::StaticVars>(0_c))
            << ": " << couscous::get<A, couscous::StaticVars>(0_c) << std::endl;
  couscous::set<A, couscous::ObjMethods>(
      couscous::find_by_name<A, couscous::ObjMethods>("num_accessor"_s)[0_c],
      200, obj);
  std::cout << "num_accessor: "
            << couscous::get<A, couscous::ObjMethods>(
                   couscous::find_by_name<A, couscous::ObjMethods>(
                       "num_accessor"_s)[0_c],
                   obj)

            << std::endl;
  // Ideal couscous provides it's own for each implementation.
  // Differences: will fold all result values in tuple
  auto tup = couscous::for_each(hana::tuple_c<long long, 1, 2, 3>,
                                [](auto &&x) { return x + 5_c; });
  std::cout << (tup == hana::tuple_c<long long, 6, 7, 8>) << std::endl;
  // To concat tuples in one you can use "boost::hana::concat" (which will
  // concat only two tuples)
  // or "multiple_concat" (for multiple tuples)
  constexpr auto all_methods_indices = couscous::multiple_concat(
      couscous::find_by_name<A, couscous::ObjMethods>("method1"_s),
      couscous::find_by_name<A, couscous::ObjMethods>("method2"_s),
      couscous::find_by_name<A, couscous::ObjMethods>("method3"_s));
  auto comp_indices = couscous::for_each(all_methods_indices, [](auto &&index) {
    if
      constexpr(std::decay_t<decltype(
                    couscous::check_invoke<A, couscous::ObjMethods, A &, int>(
                        index))>::value) {
        return index;
      }
  });
  constexpr auto accessor_inds = hana::concat(
      couscous::find_by_name<A, couscous::ObjMethods>("num_accessor"_s),
      couscous::find_by_name<A, couscous::ObjMethods>("num2_accessor"_s));
  // To get multiple vars/methods you can use gets or "gets_tuple_args"
  auto nums_tup = couscous::gets<A, couscous::ObjMethods>(accessor_inds, obj);
  std::cout << "Accessors gets: ";
  hana::for_each(nums_tup, [](auto &&x) { std::cout << x << '\t'; });
  std::cout << std::endl;
  // You can also call procedures(void return type) as well
  couscous::gets<A, couscous::ObjMethods>(comp_indices, obj, -100);
  auto params = hana::make_tuple(hana::make_tuple(std::ref(obj), 42),
                                 hana::make_tuple(std::ref(obj), "argument1"),
                                 hana::make_tuple(std::ref(obj), 666),
                                 hana::make_tuple(std::ref(obj), "argument2"),
                                 hana::make_tuple(std::ref(obj), 1337),
                                 hana::make_tuple(std::ref(obj), "argument3"));
  couscous::gets_tuple_args<A, couscous::ObjMethods>(all_methods_indices,
                                                     params);
  // For multiple assigns: "sets" or "sets_tuple_args" functions
  couscous::sets<A, couscous::ObjMethods>(
      hana::concat(
          couscous::find_by_name<A, couscous::ObjMethods>("num_accessor"_s),
          couscous::find_by_name<A, couscous::ObjMethods>("num2_accessor"_s)),
      1000, obj);
  std::cout << "num_accessor: " << obj.num_accessor()
            << "\tnum2_accessor: " << obj.num2_accessor() << std::endl;
  couscous::sets_tuple_args<A, couscous::ObjMethods>(
      accessor_inds, hana::make_tuple(100, 200),
      hana::make_tuple(hana::make_tuple(std::ref(obj)),
                       hana::make_tuple(std::ref(obj))));
  std::cout << "num_accessor: " << obj.num_accessor()
            << "\tnum2_accessor: " << obj.num2_accessor() << std::endl;
}
