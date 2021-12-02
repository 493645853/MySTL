#ifndef __ITERATOR_H__
#define __ITERATOR_H__


#include <cstddef>

namespace mySTL
{
    /**
     * @brief 5种不同的迭代器类型
     * 空struct只用于编译器期间区别不同的迭代器类型
     */
    struct input_iterator_tag {};
    struct output_iterator_tag {};
    struct forward_iterator_tag: public input_iterator_tag {}; // 输入iter的超集
    struct bidirectional_iterator_tag: public forward_iterator_tag {}; // 单向iter的超集
    struct random_access_iterator_tag: public bidirectional_iterator_tag {}; // 双向iter的超集

    // iterator的5种标准属性， 定义iterator的时候继承这个类
    template <class Category, class T, class Distance = ptrdiff_t,
        class Pointer = T*, class Reference = T&>
    struct iterator
    {
        typedef Category  iterator_category;
        typedef T         value_type;
        typedef Pointer   pointer;
        typedef Reference reference;
        typedef Distance  difference_type;
    };

    // iterator traits, 用于编译器提取对应的iterator类型 （class或者原生指针）
    // iterator traits for class
    template <class Iterator>
    struct iterator_traits
    {
        typedef typename Iterator::iterator_category iterator_category;
        typedef typename Iterator::value_type        value_type;
        typedef typename Iterator::Pointer           pointer;
        typedef typename Iterator::Reference         reference;
        typedef typename Iterator::Distance          difference_type;
    };

    // iterator traits for raw pointer
    template <class T>
    struct iterator_traits<T*>
    {
        typedef random_access_iterator_tag  iterator_category;
        typedef T                           value_type;
        typedef T*                          pointer;
        typedef T&                          reference;
        typedef ptrdiff_t                   difference_type;
    };

    // iterator traits for const raw pointer
    template <class T>
    struct iterator_traits<const T*>
    {
        typedef random_access_iterator_tag  iterator_category;
        typedef T                           value_type;
        typedef const T*                    pointer;
        typedef const T&                    reference;
        typedef ptrdiff_t                   difference_type;
    };

    // 快速获得iterator性质 （分解为小函数，
    // 获取迭代器类型tag
    template <class Iterator>
    inline typename iterator_traits<Iterator>::iterator_category
    iterator_category(const Iterator&)
    {
        typedef typename iterator_traits<Iterator>::iterator_category Category;
        return Category(); // 临时对象
    }


    // 下面实现distance函数： 计算两个迭代器之前元素个数
    // 注意在函数中无法使用偏特化， 所以需要其他方式（形参）

    // 非连续距离， O（N）
    template <class InputIterator>
    inline typename iterator_traits<InputIterator>::difference_type
    __distance(InputIterator first, InputIterator last, input_iterator_tag)
    {
        typename iterator_traits<InputIterator>::difference_type n = 0;
        while(first!=last)
        {
            ++first;
            ++n;
        }
        return n;
    }

    // 指针距离， O（1）
    template <class RandomAccessIterator>
    inline typename iterator_traits<RandomAccessIterator>::difference_type
    __distance(RandomAccessIterator first, RandomAccessIterator last, random_access_iterator_tag)
    {
        return last - first;
    }

    template <class InputIterator>
    inline typename iterator_traits<InputIterator>::difference_type
    distance(InputIterator first, InputIterator last)
    {
        return __distance(first, last, iterator_category(first));
    }

    // 实现advance函数， 把迭代器向前/后进n位
    // 非连续内存前进， O（N）
    template <class InputIterator, class Distance>
    inline void __advance(InputIterator& iter, Distance n, input_iterator_tag)
    {
        while(n--) ++iter;
    }

    // 非连续前进（n>=0) 或者后退（n<0）， O（N）
    template <class BidirectionalIterator, class Distance>
    inline void __advance(BidirectionalIterator& iter, Distance n, bidirectional_iterator_tag)
    {
        if(n>=0)
            while(n--) ++iter;
        else
            while(n++) --iter;
    }

    template <class RandomAccessIterator, class Distance>
    inline void __advance(RandomAccessIterator& iter, Distance n, random_access_iterator_tag)
    {
        iter+=n;
    }

    template <class InputIterator, class Distance>
    inline void advance(InputIterator& iter, Distance n)
    {
        __advance(iter, n, iterator_category(iter));
    }
}
#endif // __ITERATOR_H__