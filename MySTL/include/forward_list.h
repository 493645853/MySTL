#ifndef __FORWARD_LIST_H__
#define __FORWARD_LIST_H__

// 单向链表
#include "iterator.h"
#include "type_traits.h"
#include "allocator.h"

namespace mySTL
{
    // list 节点
    template <class T>
    struct __forward_list_node
    {
        typedef __forward_list_node<T>* node_pointer;
        node_pointer next;
        T data;
        void unlink() { next = this;}
    };

    // list iterator
    template <class T>
    struct __forward_list_iterator: public mySTL::iterator<mySTL::forward_iterator_tag, T>
    {
        typedef __forward_list_iterator<T> self;
        typedef __forward_list_node<T>*    link_type;
        typedef size_t                     size_type;
        typedef T*                         pointer;
        typedef T&                         reference;

        link_type node;

        // 构造函数
        __forward_list_iterator(link_type node) : node(node) {}
        __forward_list_iterator() {}
        __forward_list_iterator(const self& other):node(other.node) {}

        // 逻辑判断重载
        bool operator==(const self& other) const {return node==other.node;}
        bool operator!=(const self& other) const {return node!=other.node;}
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
    };

    // 单向链表
    template <class T>
    class forward_list 
    {
    public:
        typedef __forward_list_node<T>                      list_node;
        typedef list_node*                                  link_type;
        typedef mySTL::allocator<T>                         data_allocator;
        typedef mySTL::allocator<list_node>                 node_allocator;

        typedef typename data_allocator::value_type         value_type;
        typedef typename data_allocator::pointer            pointer;
        typedef typename data_allocator::const_pointer      const_pointer;
        typedef typename data_allocator::referece           reference;
        typedef typename data_allocator::const_reference    const_reference;
        typedef typename data_allocator::size_type          size_type;
        typedef typename data_allocator::difference_type    difference_type;

        typedef __forward_list_iterator<T>                  iterator;

    private:
        link_type __node;
        size_type __size;

    public:
        // TODO
    };
}

#endif // __FORWARD_LIST_H__