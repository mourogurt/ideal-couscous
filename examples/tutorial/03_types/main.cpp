#include <iostream>

#include <boost/hana/for_each.hpp>
#include <reflect.hpp>
#include <typeinfo>

namespace couscous {
inline namespace utils { using namespace ::reflect::utils; }
inline namespace metautils { using namespace ::reflect::metautils; }
inline namespace reflect { using namespace ::reflect; }
}

namespace hana = boost::hana;
using namespace hana::literals;

template <class T> struct SomeStruct {
  T a;
  static int b;
  char *foo() {
    char *c = nullptr;
    return c;
  }
  void foo(int) {}
  void foo(int) const {}
  static void bar() {}
  static void bar(int) {}
  IN_METAINFO(SomeStruct)
  REFLECT_OBJ_VAR(a)
  REFLECT_STATIC_VAR(b)
  REFLECT_OBJ_MTD(foo)
  REFLECT_OBJ_MTD(foo, int)
  REFLECT_CONST_OBJ_MTD(foo, int)
  REFLECT_STATIC_MTD(bar)
  REFLECT_STATIC_MTD(bar, int)
};

template <class T> int SomeStruct<T>::b = 0;

template <class... Args> struct OtherStruct {
  auto foo(Args &&...);
  constexpr auto bar() { return ""; }
  OUT_METAINFO(OtherStruct)
};

TEMPLATE_METAINFO(OtherStruct, class... Args, Args...)
REFLECT_OBJ_MTD(foo, Args &&...)
REFLECT_OBJ_MTD(bar)
END_METAINFO

template <class... Args> auto OtherStruct<Args...>::foo(Args &&...) {
  return 1;
}

int main() {
  // To get a method result/variable type you can use "result_type" function,
  // whitch will return boost::hana::type_c<...>.
  std::cout << std::boolalpha << "Result type is an int&&: "
            << (couscous::result_type<SomeStruct<int>, couscous::ObjVars>(
                    0_c) == hana::type_c<int &&>)
            << std::endl;
  std::cout << "Result type is a char*: "
            << (couscous::result_type<SomeStruct<int>, couscous::ObjMethods>(
                    0_c) == hana::type_c<char *>)
            << std::endl;
  std::cout << "Result type is a int: "
            << (couscous::result_type<OtherStruct<>, couscous::ObjMethods>(
                    0_c) == hana::type_c<int>)
            << std::endl;
  std::cout << "Result type is a const char*: "
            << (couscous::result_type<OtherStruct<>, couscous::ObjMethods>(
                    1_c) == hana::type_c<const char *>)
            << std::endl;
  // Also we can use "result_type" in the "MethodInfo" helper struct which will
  // return types
  std::cout << "Result type is a char*: "
            << (hana::type_c<couscous::MethodInfo<
                    SomeStruct<int>, 0, couscous::ObjMethods>::result_type> ==
                hana::type_c<char *>)
            << std::endl;
  // Types of method's parameters.
  // Count: to get amount of an input args you can call "methods_args_count" or
  // "MethodInfo<...>::args_count".
  // Beware, that the first parameter of an object method - reference to object.
  std::cout << "Amount of obj method arguments: "
            << couscous::methods_args_count<
                   OtherStruct<int, std::string, double, double>,
                   couscous::ObjMethods>(0_c)

            << std::endl;
  std::cout << "Amount of static method arguments: "
            << couscous::MethodInfo<SomeStruct<float>, 0,
                                    couscous::StaticMethods>::args_count
            << std::endl;
  // Foreach: boost::hana provides for each function for foldable containers
  hana::for_each(hana::make_tuple(0, '1', "234", 5.5),
                 [](auto &&x) { std::cout << x << '\t'; });
  std::cout << std::endl;
  // Types: to get input method args types you can call "method_args" which will
  // return a tuple of boost::hana::type_c<...>.
  std::cout << "Arguments type: ";
  hana::for_each(
      couscous::method_args<OtherStruct<char, int, float>,
                            couscous::ObjMethods>(0_c),
      [](auto &&x) {
        std::cout << typeid(typename std::decay_t<decltype(x)>::type).name()
                  << '\t';
      });
  std::cout << std::endl;
  //"MethodInfo<...>::types" will return type of a boost::hana::tuple<...>.
  std::cout << (typeid(couscous::MethodInfo<SomeStruct<std::string>, 1,
                                            couscous::ObjMethods>::types) ==
                typeid(hana::tuple<SomeStruct<std::string>, int>))
            << std::endl;
  // Also you can get each input arg type separatly by calling "method_arg" or
  // "MethodInfo::arg_type".
  std::cout << (couscous::method_arg<SomeStruct<float>, couscous::ObjMethods>(
                    2_c, 1_c) == hana::type_c<int>)
            << std::endl;
  // MethodInfo example
  std::cout << (typeid(
                    couscous::MethodInfo<OtherStruct<int, float, double>, 0,
                                         couscous::ObjMethods>::arg_type<1>) ==
                typeid(int))
            << std::endl;
}
