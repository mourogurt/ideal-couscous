#include <iostream>

// You can turn off most compile-time checks in utils namespace by defining the
// macro "COUSOUS_DISABLE_MOST_CT_CHECKS"
#define COUSOUS_DISABLE_MOST_CT_CHECKS
#include <reflect.hpp>

namespace couscous {
inline namespace utils { using namespace ::reflect::utils; }
inline namespace metautils { using namespace ::reflect::metautils; }
inline namespace reflect { using namespace ::reflect; }
}

namespace hana = boost::hana;
using namespace hana::literals;

class A {
public:
  int a{2};
  static double foo(int var) { return var; }
  int bar() { return a; }
  IN_METAINFO(A)
  REFLECT_OBJ_VAR(a)
  REFLECT_STATIC_MTD(foo, int)
  REFLECT_OBJ_MTD(bar)
};

int main() {
  // Be carefull, turning off ct checks leads to wall of errors if something bad
  // happens due to heavily-templated code (Example:
  // https://raw.githubusercontent.com/SuperV1234/camomilla/master/example_errors/gcc_before.png)
  // But it allows you to not dealing with hana::optional
  A obj;
  couscous::set<A, couscous::All>(couscous::find_by_name_first<A>("a"_s), 42,
                                  obj);
  std::cout << obj.a << std::endl;
  std::cout << couscous::get<A, couscous::All>(
                   couscous::find_by_name_first<A>("foo"_s), obj, 1337)
            << std::endl;
  // For one reasons get/set still have SOME(not all) ct-checks and gets*/sets*
  // function will return tuple of element if invoked successfully otherwise
  // boost::hana::nothing
  constexpr auto func_indices =
      hana::concat(couscous::find_by_name<A, couscous::AllMethods>("foo"_s),
                   couscous::find_by_name<A, couscous::AllMethods>("bar"_s));
  auto tup = couscous::gets<A, couscous::AllMethods>(func_indices, 123);
  std::cout << std::boolalpha << (tup == hana::make_tuple(123.0, hana::nothing))
            << std::endl;
  auto tup2 = couscous::gets<A, couscous::AllMethods>(func_indices, obj);
  std::cout << std::boolalpha << (tup2 == hana::make_tuple(hana::nothing, 42))
            << std::endl;
  return 0;
}
