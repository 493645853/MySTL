#ifndef __CONSTRUCT_H__
#define __CONSTRUCT_H__

// 包含两个函数 construct, destroy
// construct    -> 对象的构造
// destroy      -> 对象的构析

#include <new>
#include "utils.h"

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


}
#endif // __CONSTRUCT_H__