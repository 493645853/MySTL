#ifndef __LIST_H__
#define __LIST_H__

// 双向链表
#include <cstddef>
#include <cassert>
#include "allocator.h"
#include "utils.h"
#include "iterator.h"

namespace mySTL
{
    // list节点
    template <class T>
    struct __list_node
    {
        typedef __list_node<T>* node_pointer;
        node_pointer prev;
        node_pointer next;
        T data;
    };

    // list iterator
    template <class T>
    struct __list_iterator: public mySTL::iterator<mySTL::bidirectional_iterator_tag, T>
    {
        typedef __list_iterator<T> self;
        typedef __list_node<T>*            link_type;
        typedef size_t                     size_type;
        
        // type
        typedef T* pointer;
        typedef T& reference;

        link_type node; // 迭代器的node指针

        // 构造函数
        __list_iterator(link_type node) : node(node) {}
        __list_iterator() {}
        __list_iterator(const self& others):node(others.node) {}

        // 逻辑判断重载
        bool operator==(const self& others) const {return node==others.node;}
        bool operator!=(const self& others) const {return node!=others.node;}
        
        // 解引用
        reference operator*() const {return node->data;}

        // member access
        pointer operator->() const {return &(operator*());}

        // 迭代器累加1
        // ++*this
        self& operator++()
        {
            assert(node!=nullptr);
            node = node->next;
            return *this;
        }

        self& operator++(int)
        {
            self tmp = *this;
            ++*this;
            return tmp;
        }

        // 迭代器减1
        self& operator--()
        {
            assert(node!=nullptr);
            node = node->prev;
            return *this;
        }

        self& operator--(int)
        {
            self tmp = *this;
            --*this;
            return *this;
        }
    };

    template <class T>
    struct __list_const_iterator: public __list_iterator<T>
    {
        // redefine const
        typedef const T* pointer;
        typedef const T& reference;
        // 解引用
        reference operator*() const {return this->node->data;}
        // member access
        pointer operator->() const {return &(operator*());}
    };

    // list
    template <class T, class Alloc = mySTL::allocator<T>>
    class list
    {
    public:
        typedef __list_iterator<T> iterator;
        
    };

    
}
#endif // __LIST_H__