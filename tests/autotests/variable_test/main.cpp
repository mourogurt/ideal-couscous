#include <reflect.hpp>
#include <QtTest>
#include <boost/hana/replicate.hpp>

class VariableTest : public QObject {
    Q_OBJECT
public:
    int var {1};
    const int const_var {2};
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
    QCOMPARE((reflect::utils::get<Type,reflect::ObjVars>(boost::hana::size_c<0>,*this)).value(),1);
    QCOMPARE((reflect::utils::get<Type,reflect::ObjVars>(boost::hana::size_c<1>,*this)).value(),2);
}

void VariableTest::get_static_variable() {
    static_var = 3;
    QCOMPARE((reflect::utils::get<Type,reflect::StaticVars>(boost::hana::size_c<0>,*this)).value(),3);
}

void VariableTest::get_variable() {
    var = 1;
    static_var = 3;
    QCOMPARE((reflect::utils::get<Type,reflect::AllVars>(boost::hana::size_c<0>,*this)).value(),1);
    QCOMPARE((reflect::utils::get<Type,reflect::AllVars>(boost::hana::size_c<1>,*this)).value(),2);
    QCOMPARE((reflect::utils::get<Type,reflect::AllVars>(boost::hana::size_c<2>,*this)).value(),3);
    QCOMPARE((reflect::utils::get<int,reflect::AllVars>(boost::hana::size_c<0>,*this)),boost::hana::nothing);
    QCOMPARE((reflect::utils::get<Type,int>(boost::hana::size_c<0>,*this)),boost::hana::nothing);
    QCOMPARE((reflect::utils::get<Type,reflect::AllVars>(0,*this)),boost::hana::nothing);
    QCOMPARE((reflect::utils::get<Type,reflect::AllVars>(boost::hana::size_c<0>)),boost::hana::nothing);
}
void VariableTest::set_obj_variable() {
    reflect::utils::set<Type,reflect::ObjVars>(boost::hana::size_c<0>,11,*this);
    QCOMPARE((reflect::utils::get<Type,reflect::ObjVars>(boost::hana::size_c<0>,*this)).value(),11);
    QCOMPARE(typeid(reflect::utils::set<Type,reflect::ObjVars>(boost::hana::size_c<0>,11,*this)),typeid(boost::hana::just(::boost::hana::bool_c<true>)));
}

void VariableTest::set_static_variable() {
    reflect::utils::set<Type,reflect::StaticVars>(boost::hana::size_c<0>,12);
    QCOMPARE((reflect::utils::get<Type,reflect::StaticVars>(boost::hana::size_c<0>)).value(),12);
    QCOMPARE(typeid(reflect::utils::set<Type,reflect::StaticVars>(boost::hana::size_c<0>,12)),typeid(boost::hana::just(::boost::hana::bool_c<true>)));
}

void VariableTest::set_variable() {
    reflect::utils::set<Type,reflect::AllVars>(boost::hana::size_c<0>,13,*this);
    reflect::utils::set<Type,reflect::AllVars>(boost::hana::size_c<2>,14,*this);
    QCOMPARE((reflect::utils::get<Type,reflect::AllVars>(boost::hana::size_c<0>,*this)).value(),13);
    QCOMPARE((reflect::utils::get<Type,reflect::AllVars>(boost::hana::size_c<2>,*this)).value(),14);
    QCOMPARE(typeid(reflect::utils::get<Type,reflect::AllVars>(boost::hana::size_c<0>)),typeid(boost::hana::nothing));
    QCOMPARE(typeid(reflect::utils::get<Type,reflect::AllVars>(0,*this)),typeid(boost::hana::nothing));
    QCOMPARE(typeid(reflect::utils::get<Type,int>(boost::hana::size_c<0>,*this)),typeid(boost::hana::nothing));
    QCOMPARE(typeid(reflect::utils::get<int,reflect::AllVars>(boost::hana::size_c<0>,*this)),typeid(boost::hana::nothing));
    QCOMPARE(typeid(reflect::utils::set<Type,reflect::AllVars>(boost::hana::size_c<0>,13)),typeid(boost::hana::nothing));
    QCOMPARE(typeid(reflect::utils::set<Type,reflect::AllVars>(boost::hana::size_c<0>,*this)),typeid(boost::hana::nothing));
    QCOMPARE(typeid(reflect::utils::set<Type,reflect::AllVars>(0,13,*this)),typeid(boost::hana::nothing));
    QCOMPARE(typeid(reflect::utils::set<Type,int>(boost::hana::size_c<0>,13,*this)),typeid(boost::hana::nothing));
    QCOMPARE(typeid(reflect::utils::set<int,reflect::AllVars>(boost::hana::size_c<0>,13,*this)),typeid(boost::hana::nothing));
}

void VariableTest::gets_variable() {
    var = 1;
    static_var = 3;
    auto tup = reflect::utils::gets<Type,reflect::AllVars>(reflect::metautils::gen_inds_tup<decltype(reflect::utils::count<Type,reflect::AllVars>().value())>(),*this);
    QCOMPARE (tup, boost::hana::just(::boost::hana::make_tuple(1,2,3)));
    QCOMPARE (typeid(reflect::utils::gets<Type,reflect::AllVars>(boost::hana::size_c<0>,*this)),typeid(boost::hana::nothing));
    QCOMPARE (typeid(reflect::utils::gets<Type,int>(reflect::metautils::gen_inds_tup<decltype(reflect::utils::count<Type,reflect::AllVars>().value())>(),*this)),typeid(boost::hana::nothing));
    QCOMPARE (typeid(reflect::utils::gets<int,reflect::AllVars>(reflect::metautils::gen_inds_tup<decltype(reflect::utils::count<Type,reflect::AllVars>().value())>(),*this)),typeid(boost::hana::nothing));
    auto tup2 =  reflect::utils::gets<Type,reflect::AllVars>(reflect::metautils::gen_inds_tup<decltype(reflect::utils::count<Type,reflect::AllVars>().value())>());
    QCOMPARE (tup2, boost::hana::just(::boost::hana::make_tuple(boost::hana::nothing,boost::hana::nothing,3)));
}

void VariableTest::sets_variable() {
    var = 1;
    static_var = 3;
    auto tup = reflect::utils::sets<Type,reflect::AllVars>(reflect::metautils::gen_inds_tup<decltype(reflect::utils::count<Type,reflect::AllVars>().value())>(),100,*this);
    QCOMPARE(var,100);
    QCOMPARE(static_var,100);
    QCOMPARE(tup,boost::hana::just(boost::hana::make_tuple(boost::hana::bool_c<true>,boost::hana::nothing,boost::hana::bool_c<true>)));
    QCOMPARE (typeid(reflect::utils::sets<Type,reflect::AllVars>(boost::hana::size_c<0>,100,*this)),typeid(boost::hana::nothing));
    QCOMPARE (typeid(reflect::utils::sets<Type,int>(reflect::metautils::gen_inds_tup<decltype(reflect::utils::count<Type,reflect::AllVars>().value())>(),100,*this)),typeid(boost::hana::nothing));
    QCOMPARE (typeid(reflect::utils::sets<int,reflect::AllVars>(reflect::metautils::gen_inds_tup<decltype(reflect::utils::count<Type,reflect::AllVars>().value())>(),100,*this)),typeid(boost::hana::nothing));

}

void VariableTest::gets_tuple_args_variable() {
    var = 1;
    static_var = 3;
    constexpr auto all_vars_size = reflect::utils::count<Type,reflect::AllVars>().value();
    auto tup = reflect::utils::gets_tuple_args<Type,reflect::AllVars>(reflect::metautils::gen_inds_tup<decltype(all_vars_size)>(),
                                                                      boost::hana::replicate<boost::hana::tuple_tag>(boost::hana::make_tuple(std::ref(*this)),all_vars_size));
    QCOMPARE (tup, boost::hana::just(::boost::hana::make_tuple(1,2,3)));
    auto tup2 = reflect::utils::gets_tuple_args<Type,reflect::AllVars>(reflect::metautils::gen_inds_tup<decltype(all_vars_size)>(),
                                                                       boost  ::hana::concat(boost::hana::replicate<boost::hana::tuple_tag>(boost::hana::make_tuple(std::ref(*this)),all_vars_size - boost::hana::size_c<1>),
                                                                                             boost::hana::make_tuple(boost::hana::nothing)));
    QCOMPARE (tup2, boost::hana::just(::boost::hana::make_tuple(1,2,boost::hana::nothing)));
    QCOMPARE(typeid(reflect::utils::gets_tuple_args<Type,reflect::AllVars>(reflect::metautils::gen_inds_tup<decltype(all_vars_size)>(),boost::hana::replicate<boost::hana::tuple_tag>(boost::hana::make_tuple(std::ref(*this)),all_vars_size - boost::hana::size_c<1>))),
             typeid(boost::hana::nothing));
    QCOMPARE(typeid(reflect::utils::gets_tuple_args<Type,reflect::AllVars>(boost::hana::size_c<3>,boost::hana::replicate<boost::hana::tuple_tag>(boost::hana::make_tuple(std::ref(*this)),all_vars_size))),
             typeid(boost::hana::nothing));
    QCOMPARE(typeid(reflect::utils::gets_tuple_args<Type,int>(reflect::metautils::gen_inds_tup<decltype(all_vars_size)>(),boost::hana::replicate<boost::hana::tuple_tag>(boost::hana::make_tuple(std::ref(*this)),all_vars_size))),
             typeid(boost::hana::nothing));
    QCOMPARE(typeid(reflect::utils::gets_tuple_args<int,reflect::AllVars>(reflect::metautils::gen_inds_tup<decltype(all_vars_size)>(),boost::hana::replicate<boost::hana::tuple_tag>(boost::hana::make_tuple(std::ref(*this)),all_vars_size))),
             typeid(boost::hana::nothing));
}

void VariableTest::sets_tuple_args_variable() {
    var = 1;
    static_var = 3;
    constexpr auto all_vars_size = reflect::utils::count<Type,reflect::AllVars>().value();
    constexpr auto indices = reflect::metautils::gen_inds_tup<decltype(all_vars_size)>();
    auto params = boost::hana::replicate<boost::hana::tuple_tag>(boost::hana::make_tuple(std::ref(*this)),all_vars_size);
    constexpr auto values = boost::hana::make_tuple(100,200,300);
    auto tup = reflect::utils::sets_tuple_args<Type,reflect::AllVars>(indices,values,params);
    QCOMPARE(tup,boost::hana::just(boost::hana::make_tuple(boost::hana::bool_c<true>,boost::hana::nothing,boost::hana::bool_c<true>)));
    QCOMPARE(var,100);
    QCOMPARE(static_var,300);
    auto params2 = boost::hana::concat(boost::hana::replicate<boost::hana::tuple_tag>(boost::hana::make_tuple(std::ref(*this)),all_vars_size - boost::hana::size_c<1>),
                                           boost::hana::make_tuple(boost::hana::nothing));
    QCOMPARE(typeid(reflect::utils::sets_tuple_args<Type,reflect::AllVars>(indices,values,params2)),
             typeid(boost::hana::just(boost::hana::make_tuple(boost::hana::bool_c<true>,boost::hana::nothing,boost::hana::nothing))));
    QCOMPARE(typeid(reflect::utils::sets_tuple_args<Type,reflect::AllVars>(all_vars_size,values,params)),
             typeid(boost::hana::nothing));
    QCOMPARE(typeid(reflect::utils::sets_tuple_args<Type,int>(indices,values,params)),
             typeid(boost::hana::nothing));
    QCOMPARE(typeid(reflect::utils::sets_tuple_args<int,reflect::AllVars>(indices,values,params)),
             typeid(boost::hana::nothing));
}

QTEST_MAIN(VariableTest)
#include "main.moc"
