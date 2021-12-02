#ifndef __UTILS_H__
#define __UTILS_H__

// 实现一些通用工具，如 move, forward, swap, pair

#include <cstddef>
#include "type_traits.h"

namespace mySTL
{
    // move, convert any value to rvalue
    // T&& 是万能引用， 既可以引用左值， 也可以引用右值， 注意template申明
    template <class T>
    typename mySTL::remove_reference<T>::type&& move(T&& arg) noexcept
    {
        return static_cast<typename mySTL::remove_reference<T>::type&&>(arg);
    }

    // forward, convert lvalue to the original type
    // 完美转发的含义： 保留传入参数本来的类型 （左值右值）
    // 如果有函数f(int&& a), 调用f(1)后， 1在函数内部会变为左值， 因为分配了内存
    // 如果用f(forward(1))后， 就不会改变 （触发了引用折叠， && && = &&）
    // 详细可见 https://zhuanlan.zhihu.com/p/161039484
    template <class T>
    T&& forward(typename mySTL::remove_reference<T>::type& arg) noexcept
    {
        return static_cast<T&&>(arg);
    }

    // forward, convert rvalue to the specified type
    template <class T>
    T&& forward(typename mySTL::remove_reference<T>::type&& arg) noexcept
    {
        static_assert(!mySTL::is_lvalue_reference<T>::value, "Right value reference should be used.");
        return static_cast<T&&>(arg);
    }

    // swap
    template <class T>
    void swap(T& lhs, T& rhs)
    {
        auto tmp(mySTL::move(lhs));
        lhs = mySTL::move(rhs);
        rhs = mySTL::move(tmp);
    }

    // swap by range using the iterator
    // 注意， 在输入时， 指针本身依旧会被拷贝一次， 但是指向的内容不会被拷贝
    template <class ForwardIter1, class ForwardIter2>
    ForwardIter2 swap_range(ForwardIter1 first1, ForwardIter1 last1, ForwardIter2 first2)
    {
        for(;first1 != last1; ++first1, ++first2)
        {
            mySTL::swap(*first1, *first2);
        }
        return first2;
    }

    // array swap
    // 注意形参的array写法， T（&a)[N] 可以自动推导N，优于 T a[N] (N在这种情况下必须显式申明)
    template <class T, size_t N>
    void swap(T(&a)[N], T(&b)[N])
    {
        mySTL::swap_range(a, a+N, b);
    }

    // get array length
    template <class T, size_t N>
    constexpr size_t getArrayLen(T(&arr)[N]) {return N;}

    
}

#endif // __UTILS_H__