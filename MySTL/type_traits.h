#ifndef __TYPE_TRAITS_H__
#define __TYPE_TRAITS_H__

// 模板元编程， 通过模板自动推导条件

namespace mySTL 
{
    // remove reference by using partial specialization
    template <class T>
    struct remove_reference
    {typedef T type;};

    template <class T>
    struct remove_reference<T&>
    {typedef T type;};

    template <class T>
    struct remove_reference<T&&>
    {typedef T type;};

    // remove pointer
    template <class T>
    struct remove_pointer
    {typedef T type;};

    template <class T>
    struct remove_pointer<T*>
    {typedef T type;};

    // integral constant to store the value
    // 因为模板参数只能是bool/int
    // 用于继承， 自动推导模板
    template <class T, T var>
    struct integral_constant
    {
        static constexpr T value = var;
        typedef T value_type;
        typedef integral_constant<T,var> type;
        constexpr operator value_type() const noexcept {return value;} // function ? 
    };
    template <class T, T var>
    constexpr T integral_constant<T, var>::value; // static define

    // compile true
    typedef integral_constant<bool, true> true_type;

    // complie false
    typedef integral_constant<bool, false> false_type;

    template <bool B>
        using bool_constant = integral_constant<bool, B>;

    /**
     * @brief 根据模板推导， 判断条件
     * 
     */
    
    // conditional判断， <bool, type1, type2>, if true, type=type1, else false = type2
    template <bool cond, class B1, class B2>
    struct conditional 
    {typedef B1 type;};

    template <class B1, class B2>
    struct conditional<false, B1, B2>
    {typedef B2 type;};

    // 用conditional实现 or
    template <class...> // 申明__or_的模板参数可变
    struct __or_;

    template <>
    struct __or_<> : public false_type {};

    template <class B1> // 注意B1应该继承 true_type/false type
    struct __or_<B1> : public B1 {};

    /* 注意or的逻辑， B1/B2只要一个为真，则结果为真，如果B1::value = true,
       则__or_::value = true, 如果B1::value = false, 则__or_::value = B2::value,
       最后结果由B2::value决定
    */
    template <class B1, class B2> 
    struct __or_<B1, B2> : public conditional<B1::value, B1, B2>::type {};

    template <class B1, class B2, class B3, class... Bn> // 我都惊了， 模板参数也能递归？
    struct __or_<B1, B2, B3, Bn...>
        : public conditional<B1::value, B1, __or_<B2, B3, Bn...>>::type {};

    // 用conditional实现and
    template <class...>
    struct __and_;

    template <>
    struct __and_<> : public true_type {};

    template <class B1>
    struct __and_<B1> : public B1 {};

    // 我又惊了， 这个and逻辑， if B1::value = true, 则结果由B2::value决定，else 直接返回false
    template <class B1, class B2>
    struct __and_<B1, B2> : public conditional<B1::value, B2, B1>::type {};

    template <class B1, class B2, class B3, class... Bn>
    struct __and_<B1, B2, B3, Bn...>
        : public conditional<B1::value, B2, __and_<B3, Bn...>>::type {};

    template <class B>
    struct __not_
        : public bool_constant<!bool(B::value)> {};

    // 

    // is same type
    template <class, class>
    struct is_same : public false_type {};

    template <class T>
    struct is_same<T, T> : public true_type {};

    // is lvalue reference
    template <class T>
    struct is_lvalue_reference:public false_type {};    // 假

    template <class T>
    struct is_lvalue_reference<T&>:public true_type {}; // 真

    // is rvalue reference
    template <class T>
    struct is_rvalue_reference:public false_type {};

    template <class T>
    struct is_rvalue_reference<T&&>:public true_type {};

}

#endif // __TYPE_TRAITS_H__