#ifndef __LIST_H__
#define __LIST_H__

// 双向链表
#include <cstddef>
#include <cassert>
#include "allocator.h"
#include "utils.h"
#include "iterator.h"
#include "construct.h"

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
    template <class T>
    class list
    {
    public:
        
        typedef __list_node<T>                              list_node;
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
        

        typedef __list_iterator<T> iterator;
            
    private:
        link_type __node; // 虚拟节点, 对应end()
        size_type __size; // size of the list

    
    public:
        // 构造函数
        // 默认产生空链表
        list() 
        {empty_init();} 
        // n个默认节点
        explicit list(size_type n) 
        {fill_init(n, value_type());} 

        list(size_type n, const T& value)
        {fill_init(n, value_type(value));} 

        ~list()
        {
            if(__node)
            {
                clear();
                node_allocator::deallocate(__node);
                __node = nullptr;
                __size = 0;
            }
        }

    public:
        iterator begin() {return __node->next;}
        iterator end()   {return __node;}

        bool empty() const {return __node->next==__node;}
        size_type size() const {return __size;}

        // 访问元素
        reference front()
        {
            assert(!empty());
            return *begin();
        }

        reference back()
        {
            assert(!empty());
            return *(--end());
        }

    public:
        // 插入操作
        iterator insert(iterator pos, const_reference x); // 插入单个元素
        iterator insert(iterator pos, value_type&& x); // 支持移动构造的插入
        iterator insert(iterator pos, size_type n, const_reference x); // 插入n个相同元素
        iterator insert(iterator pos, size_type n, value_type&& x);// 支持移动构造的多插入
        template<class OtherIter>
        iterator insert(iterator pos, OtherIter first, OtherIter last); // 插入其他迭代器的元素值

        void push_back(const_reference x) {insert(end(), x);}
        void push_back(value_type&& x) {insert(end(), x);}
        void push_front(const_reference x) ;
        void push_front(value_type&& x);

        // 删除元素操作
        iterator erase(iterator pos);
        void pop_back();
        void pop_front();
        void clear();


    private: // helper function
        // 创建空节点， 初始化__size
        inline void empty_init()
        {
            __node = node_allocator::allocate();
            __node->prev = __node;
            __node->next = __node;
            __size = 0;
        }

        inline void fill_init(size_type n, const value_type& value)
        {
            empty_init();
            __size = n;
            try 
            {
                while(n-->0)
                {
                    link_type node = create_node(value);
                    link_nodes_at_back(node, node);
                }
            } 
            catch (...) 
            {
                clear();
                node_allocator::deallocate(__node);
                __node = nullptr;
                throw;
            }
        }

        // create new node
        template <class ...Args>
        link_type create_node(Args&&... args);
        // destroy one node
        void      destroy_node(link_type node);

        // 根据iterator插入一段nodes， 自动分辨插入位置
        inline iterator link_nodes_at(iterator pos, link_type first, link_type last)
        {
            if(pos==__node->next) // front
                link_nodes_at_front(first, last);
            else if(pos==__node) // back
                link_nodes_at_back(first, last);
            else
                link_nodes_in_between(pos.node, first, last);
            
            return first;
        }

        // 尾部连接一段nodes, 接口提供首尾指针
        inline void link_nodes_at_back(link_type& first, link_type& last)
        {
            last->next = __node;
            first->prev = __node->prev;
            __node->prev->next = first;
            __node->prev = last;
        }

        // 首部连接一段nodes, 接口提供首尾指针
        inline void link_nodes_at_front(link_type& first, link_type& last)
        {
            first->prev = __node;
            last->next = __node->next;
            __node->next->prev = last;
            __node->next = first;
        }

        // 中间连接一段nodes, 接口提供首尾指针
        inline void link_nodes_in_between(link_type& pos, link_type& first, link_type& last)
        {
            pos->prev->next = first;
            first->prev = pos->prev;
            last->next = pos;
            pos->prev = last;
        }

    };

    /**
     * @brief Implementation
     * 
     */
    
    // *** 插入元素 ***
    template<class T>
    typename list<T>::iterator list<T>::insert(iterator pos, const_reference x)
    {
        link_type tmp_node = create_node(x);
        ++__size;
        return link_nodes_at(pos, tmp_node, tmp_node);
    }

    template<class T>
    typename list<T>::iterator list<T>::insert(iterator pos, value_type&& x)
    {
        link_type tmp_node = create_node(x);
        ++__size;
        return link_nodes_at(pos, tmp_node, tmp_node);
    }


    // *** 删除元素 ***

    // 清空list
    template <class T>
    void list<T>::clear()
    {
        if(__size!=0)
        {
            auto curr = __node->next;
            for(auto next = curr->next; curr!=__node; curr = next, next = curr->next)
            {
                destroy_node(curr);
            }
            __node->prev = __node;
            __node->next = __node;
            __size = 0;
        }
    }


    // *** helper function ***

    // 创建节点, 接受任意个初始化参数
    template<class T>
    template<class ...Args>
    typename list<T>::link_type list<T>::create_node(Args&&... args)
    {
        link_type ptr = node_allocator::allocate(1); // 分配一个节点内存
        try 
        {
            construct(&ptr->data, mySTL::forward<Args>(args)...);
            ptr->next = nullptr;
            ptr->prev = nullptr;
        } 
        catch (...) 
        {
            node_allocator::deallocate(ptr);
            throw("Failed to create node");
        }
        return ptr;
    }

    // 删除节点
    template <class T>
    void list<T>::destroy_node(link_type node)
    {
        destroy(&node->data);//析构
        node_allocator::deallocate(node); // 删除内存空间
    }
}
#endif // __LIST_H__