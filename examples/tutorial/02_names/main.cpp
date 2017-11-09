#include <iostream>

#include <boost/hana/for_each.hpp>
#include <reflect.hpp>

namespace couscous {
inline namespace utils { using namespace ::reflect::utils; }
inline namespace metautils { using namespace ::reflect::metautils; }
inline namespace reflect { using namespace ::reflect; }
} // namespace couscous

namespace hana = boost::hana;
using namespace hana::literals;

struct SomeStruct {
  int some_var;
  template <class T> void foo(T &&);
  void bar();
  void bar(float);
  IN_METAINFO(SomeStruct)
  REFLECT_OBJ_VAR(some_var)
  // Template methods can't be reflected(because there is no pointer to template
  // method).
  REFLECT_OBJ_MTD(foo<int>, int &&)
  // What can't be said about the specialization of template methods.
  REFLECT_OBJ_MTD(foo<double>, double &&)
  REFLECT_OBJ_MTD(bar)
  REFLECT_OBJ_MTD(bar, float)
};

int main() {
  // Ideal couscous uses Boost::hana::string as compile-time strings
  constexpr auto str = BOOST_HANA_STRING("string");
  // Convert c-t string to const char* and pring it
  std::cout << hana::to<const char *>(str) << std::endl;
  // Also you can uses _s literal to create strings (located in
  // boost::hana::literals namespace)
  constexpr auto str2 = "string"_s;
  std::cout << hana::equal(str, str2) << std::endl;
  // All c-t indices in this library are boost::hana::llong<...>
  std::cout << hana::llong_c<42> << std::endl;
  // _c - c-t index literal
  std::cout << hana::equal(hana::llong_c<42>, 42_c) << std::endl;
  // member_name - get N-th name (ct-string) of member(var/method).
  std::cout << "Method name: "
            << hana::to<const char *>(
                   couscous::member_name<SomeStruct, couscous::AllMethods>(0_c))
            << std::endl;
  constexpr auto inds = couscous::gen_inds_tup<decltype(
      couscous::count<SomeStruct, couscous::AllMethods>())>();
  hana::for_each(
      inds,
      [](auto &&item) {
        std::cout
            << "Method name: "
            << hana::to<const char *>(
                   couscous::member_name<SomeStruct, couscous::AllMethods>(
                       item))
            << std::endl;
      }); // Now we generate tuple of indices by calling gen_inds_tup<N,Offset =
          // hana::llong<0>>() and call lambda for each item in tuple.
  // find_by_name - finds indices by name(ct-string) and return them in tuple.
  constexpr auto inds_tup =
      couscous::find_by_name<SomeStruct, couscous::AllMethods>("bar"_s);
  hana::for_each(inds_tup, [](auto &&item) {
    std::cout << "Index: " << item << '\t'
              << hana::to<const char *>(
                     couscous::member_name<SomeStruct, couscous::AllMethods>(
                         item))
              << std::endl;
  });
  // find_by_name_first - find first index by name (usefull for variables)
  constexpr auto ind = couscous::find_by_name_first<SomeStruct>("some_var"_s);
  std::cout << "Index of some_var: " << ind << std::endl;
}
