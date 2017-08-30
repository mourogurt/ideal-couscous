#include <boost/hana/for_each.hpp>
#include <iostream>

#include <reflect.hpp>

namespace couscous {
inline namespace utils { using namespace ::reflect::utils; }
inline namespace metautils { using namespace ::reflect::metautils; }
inline namespace reflect { using namespace ::reflect; }
}

namespace hana = boost::hana;
using namespace hana::literals;

// Speaking about inheritance in ideal couscous you should know some things

class Base {
public:
  int base_var{0};
  void base_func() {}
  // All parent classes must be reflected using OUT_METAINFO macros
  OUT_METAINFO(Base)
};

METAINFO(Base)
REFLECT_OBJ_VAR(base_var)
REFLECT_OBJ_MTD(base_func)
END_METAINFO

class NotReflected {};

class Child1 : public Base {
public:
  // To specify parent classes, just append them in METAINFO/IN_METAINFO as last
  // arguments macros, but don't append them in OUT_METAINFO
  OUT_METAINFO(Child1)
  float child_var{0};
  std::string child1_str{""};
};

METAINFO(Child1, Base)
REFLECT_OBJ_VAR(child_var)
REFLECT_OBJ_VAR(child1_str)
END_METAINFO

class Child2 final : public Child1 {
public:
  // If derived class is final, you can reflect him using IN_METAINFO
  IN_METAINFO(Child2, Child1)
};

class Child3 : public NotReflected {
public:
  // Don't specify parent classes that aren't reflected in METAINFO/IN_METAINFO
  IN_METAINFO(Child3)
};

int main() {
  // Now we can get some information about our parents
  std::cout << "Yep, Base class have no parents:\t"
            << couscous::parents_count<Base>() << std::endl;
  std::cout << "Unlike Child1:\t" << couscous::parents_count<Child1>()
            << std::endl;
  std::cout << "Or Child2:\t" << couscous::parents_count<Child2>() << std::endl;
  std::cout << "We can't get any meta information from NotReflected, so Child3 "
               "will not have any parents:\t"
            << couscous::parents_count<Child3>() << std::endl;
  // Ok, now let's print Child2's all parent class names and the their types
  hana::for_each(reflect::utils::parents_types<Child2>(), [](auto &&obj) {
    std::cout
        << "Name:\t"
        << hana::to<const char *>(
               reflect::utils::class_name<typename decltype(+obj)::type>())
        << "\t\tType:\t" << typeid(typename decltype(+obj)::type).name()
        << std::endl;
  });
  // Also you can find all reflected variables and use it like they are in
  // derived class
  Child2 obj;
  obj.base_var = 100;
  std::cout << "base_var from Base class:\t"
            << couscous::get<Child2, couscous::AllVars>(
                   couscous::find_by_name_first<Child2, couscous::AllVars>(
                       "base_var"_s),
                   obj)
            << std::endl;
  return 0;
}
