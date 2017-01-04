#ifndef META_INFORMATION_HPP
#define META_INFORMATION_HPP

#include "detail.hpp"

template< class Result, class Obj, class... Args >
class mem_fn_t {
    Result Obj::* p;
public:
    using result_type = Result;
    using obj_type = Obj;
    using type = Result Obj::*;
    constexpr static auto args_type {boost::hana::tuple_t<Args...>};
    constexpr mem_fn_t(type p):p(p){}
    constexpr decltype(auto) operator()(Obj&& obj,Args&&... args) const {
        return constexpr_invoke(p, obj, std::forward<Args>(args)...);
    }
    constexpr decltype(auto) operator()(const Obj& obj, Args&&... args) const {
        return constexpr_invoke(p, obj, std::forward<Args>(args)...);
    }
    constexpr decltype(auto) operator()(Obj& obj, Args&&... args) const {
        return constexpr_invoke(p, obj, std::forward<Args>(args)...);
    }
    template <class T>
    constexpr void set (Obj& obj, Args&&... args, T&& value) const {
        constexpr_invoke(p, obj, std::forward<Args>(args)...) = value;
    }
};

template< class R, class T, class... Args  >
constexpr auto constexpr_mem_fn(R T::* pm) -> mem_fn_t<R, T, Args...> {
    return {pm};
}

template<typename T, typename... Args>
struct MethodInfo {
    template<typename Ret>
    static constexpr auto get(Ret(T::*)(Args...)) -> Ret(T::*)(Args...);
};

template <class T>
struct is_reflected
{

    template <class C> static constexpr std::true_type check(decltype(&C::is_reflected));
    template <class> static constexpr std::false_type check(...);
    static constexpr bool value = std::is_same<std::true_type, decltype(check<T>(nullptr))>::value;
};

template <class T> constexpr bool is_reflected_v = is_reflected<T>::value;

template <class T, typename std::enable_if_t<is_reflected_v<T>,bool> = 0>
struct MetaClass {
    static constexpr auto name              {ClassName(static_cast<const T*>(nullptr))};
    static constexpr auto vars_names        {detail::vars_names_tuple<T>(std::make_index_sequence<decltype(T::Variable_counter(counter<>{}))::value>{})};
    static constexpr auto methods_names     {detail::methods_names_tuple<T>(std::make_index_sequence<decltype(T::Method_counter(counter<>{}))::value>{})};
    static constexpr auto vars_metadata     {detail::vars_ptr_tuple<T>(std::make_index_sequence<decltype(T::Variable_counter(counter<>{}))::value>{})};
    static constexpr auto methods_metadata  {detail::methods_ptr_tuple<T>(std::make_index_sequence<decltype(T::Method_counter(counter<>{}))::value>{})};
};

#define METACLASS_DEFENITION(TYPE) \
        using Type = TYPE; \
        static constexpr bool is_reflected () {return true;} \
        friend constexpr auto ClassName(const Type*) RETURN(#TYPE)\
        constexpr static counter<0> Variable_counter (counter<0>);\
        constexpr static counter<0> Method_counter (counter<0>);


#define RETURN(R) -> decltype(R) { return R; }

#define TUPLE_APPEND(STATE, COUNTER, ...) \
    friend constexpr auto STATE(counter<decltype(COUNTER(counter<>{}))::value>, \
            const Type*) \
        RETURN( boost::hana::make_tuple(__VA_ARGS__))

#define TUPLE_APPEND2(STATE, COUNTER, RESULT, NAME, ...) \
    TUPLE_APPEND(STATE, COUNTER,constexpr_mem_fn<RESULT, Type, ##__VA_ARGS__>(&Type::NAME)) \
    constexpr static counter<decltype(COUNTER(counter<>{}))::value+1u> COUNTER (counter<decltype(COUNTER(counter<>{}))::value+1u>);

#define REFLECT_VARIABLE(NAME) \
    TUPLE_APPEND (VariableNames,Variable_counter,HANA_STR(#NAME)) \
    TUPLE_APPEND2(VariablePtrs,Variable_counter,decltype(NAME),NAME) \

#define REFLECT_METHOD(NAME,...) \
    TUPLE_APPEND (MethodNames,Method_counter,HANA_STR(#NAME)) \
    TUPLE_APPEND2(MethodPtrs,Method_counter,incomplete_result_of<decltype(MethodInfo<Type, ##__VA_ARGS__>::get(&Type::NAME))(Type, ##__VA_ARGS__)>::type (__VA_ARGS__),NAME, ##__VA_ARGS__)


#endif // META_INFORMATION_HPP
