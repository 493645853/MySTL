#ifndef __CONSTRUCT_H__
#define __CONSTRUCT_H__

// 包含两个函数 construct, destroy
// construct    -> 对象的构造
// destroy      -> 对象的构析

#include <new>
#include <type_traits>
#include "utils.h"
#include "iterator.h"

namespace mySTL
{
    template <class T1>
    inline void construct(T1* ptr)
    {
        ::new (ptr) T1();
    }

    template <class T1, class T2>
    inline void construct(T1* ptr, const T2& value)
    {
        ::new (ptr) T1(value);
    }

    // 万能引用 + 完美转发
    template <class T, class... Args>
    inline void construct(T* ptr, Args&&... args)
    {
        ::new (ptr) T(mySTL::forward<Args>(args)...);
    }

    // destroy object
    template <class T>
    inline void __destroy_one(T* ptr, std::false_type)
    {
        if(!ptr) ptr->~T();  // 调用析构函数
    }

    template <class T>
    inline void __destroy_one(T, std::true_type) {}

    template <class ForwardIter>
    inline void __destroy_byIters(ForwardIter, ForwardIter, std::true_type) {}

    template <class ForwardIter>
    inline void __destroy_byIters(ForwardIter first, ForwardIter last, std::false_type)
    {
        for(;first!=last;++first)
            destroy(&*first);
    }

    // 自动判断类型是否有析构函数， 如果有就调用
    template <class T>
    inline void destroy(T* ptr)
    {
        __destroy_one(ptr, std::is_trivially_destructible<T>()); // 没办法还是用了标准库type traits,
        // 元编程实在太难了， 以后学会了再自己实现
    }

    template <class ForwardIter>
    inline void destroy(ForwardIter first, ForwardIter last)
    {
        __destroy_byIters(first, last, 
            std::is_trivially_destructible<typename mySTL::iterator_traits<ForwardIter>::value_type>());
    }

}
#endif // __CONSTRUCT_H__