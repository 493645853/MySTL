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

    /**
     * @brief 根据模板推导， 判断条件
     * 
     */

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