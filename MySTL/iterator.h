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

}
#endif // __ITERATOR_H__