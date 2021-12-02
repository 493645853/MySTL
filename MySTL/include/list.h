#ifndef __LIST_H__
#define __LIST_H__

// 双向链表
#include <cstddef>
#include <cassert>
#include <initializer_list>
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
        void unlink() { prev = next = this;}
    };

    // list iterator
    template <class T>
    struct __list_iterator: public mySTL::iterator<mySTL::bidirectional_iterator_tag, T>
    {
        typedef __list_iterator<T>         self;
        typedef __list_node<T>*            link_type;
        typedef size_t                     size_type;
    
        // type
        typedef T* pointer;
        typedef T& reference;

        link_type node; // 迭代器的node指针

        // 构造函数
        __list_iterator(link_type node) : node(node) {}
        __list_iterator() {}
        __list_iterator(const self& other):node(other.node) {}

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
        
        typedef __list_iterator<T>                          iterator;
            
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

        // 拷贝构造, 分别是从顺序容器的iterators/initialization list/其他list实例中拷贝
        template <class InputIterator>
        list(InputIterator first, InputIterator last)
        {copy_init(first, last);}

        list(std::initializer_list<value_type> ilist)
        {copy_init(ilist.begin(), ilist.end());}

        list(const list& other)
        {copy_init(other.begin(), other.end());}

        // 移动构造
        list(list&& other) : __node(other.__node), __size(other.__size)
        {
            other.__node = nullptr;
            other.__size = 0;
        }

        // 拷贝赋值
        list& operator=(const list& other) // TODO
        {
            //避免自赋值, 检查地址是否一样
            if(this != &other)
            {

            }
            return *this;
        }
        
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
        /*** 访问接口 ***/
        iterator  begin() const {return __node->next;} 
        iterator  end()   const {return __node;}
        bool      empty() const {return __node->next==__node;}
        size_type size()  const {return __size;}
        // 返回首部元素
        reference front() const{assert(!empty()); return *begin();}
        // 返回尾部元素
        reference back()  const{assert(!empty()); return *(--end());}

    public:
        /*** 修改元素接口 ***/
        // assign操作 
        void assign(size_type n, const value_type& value); 
        void assign(std::initializer_list<value_type> ilist);
        template <class InputIterator>
        void assign(InputIterator first, InputIterator last); // TODO

        // 插入操作
        iterator insert(iterator pos, const_reference x); // 插入单个元素
        iterator insert(iterator pos, value_type&& x); // 支持移动构造的插入
        iterator insert(iterator pos, size_type n, const_reference x); // 插入n个相同元素
        template <class InputIterator> // TODO
        iterator insert(iterator pos, InputIterator first, InputIterator last); // 插入其他迭代器的元素值

        template <class... Args>
        void emplace_front(Args&&... args);
        template <class... Args>
        void emplace_back(Args&&... args);

        void push_back(const_reference x);
        void push_back(value_type&& x)    {emplace_back(mySTL::move(x));}
        void push_front(const_reference x);
        void push_front(value_type&& x)   {emplace_front(mySTL::move(x));}

        // 删除元素操作
        iterator erase(iterator pos);
        iterator erase(iterator first, iterator last);
        void pop_back();
        void pop_front();
        void clear();

        // resize操作
        void resize(size_type n); //TODO
        void resize(size_type n, const_reference value); // TODO

        // 交换两个链表数据
        void swap(list& other) noexcept;

    public:
        /*** list 相关特殊操作 ***/
        void splice(iterator pos, list& other); // TODO

        // 删除值==value的节点
        void remove(const_reference value); // TODO

        // 合并链表, 顺序由 comp==true决定
        template <class Compare>
        void merge(list& other, Compare comp); // TODO

        // 反转链表
        void reverse(); // TODO

    private: // helper function
        // 创建空节点， 初始化__size
        inline void empty_init();
        inline void fill_init(size_type n, const_reference value);
        template <class InputIterator>
        inline void copy_init(InputIterator first, InputIterator last);

        // create new node
        template <class ...Args>
        link_type create_node(Args&&... args);
        // destroy one node
        void      destroy_node(link_type node);

        // 根据iterator插入一段nodes
        inline iterator link_nodes_at(iterator pos, link_type first, link_type last);
        // 断开中间一段nodes
        inline void unlink_nodes(link_type& first, link_type& last);

        // assign
        void fill_assign(size_type n, const value_type& value);
        template <class InputIterator>
        void copy_assign(InputIterator first, InputIterator last); 

        // 插入多个元素
        iterator fill_insert(iterator pos, size_type n, const_reference value); 
        // 拷贝多个元素
        template <class InputIterator>
        iterator copy_insert(iterator pos, InputIterator first, InputIterator last); 
    };

    /**
     * @brief Implementation
     * 
     */

    template <class T>
    void list<T>::assign(size_type n, const value_type& value)
    {
        fill_assign(n, value);
    }

    template <class T>
    void list<T>::assign(std::initializer_list<value_type> ilist)
    {
        copy_assign(ilist.begin(), ilist.end());
    }


    
    // *** 插入元素 ***
    template <class T>
    typename list<T>::iterator list<T>::insert(iterator pos, const_reference x)
    {
        link_type tmp_node = create_node(x);
        ++__size;
        return link_nodes_at(pos, tmp_node, tmp_node);
    }

    template <class T>
    typename list<T>::iterator list<T>::insert(iterator pos, value_type&& x)
    {
        link_type tmp_node = create_node(mySTL::move(x));
        ++__size;
        return link_nodes_at(pos, tmp_node, tmp_node);
    }

    template <class T>
    typename list<T>::iterator list<T>::insert(iterator pos, size_type n, const_reference x)
    {
        return fill_insert(pos, n, x);
    }

    template<class T>
    template <class... Args>
    void list<T>::emplace_front(Args&&... args) 
    {
        link_type node = create_node(mySTL::forward<Args>(args)...);
        link_nodes_at(begin(), node, node);
        ++__size;
    }
    
    template <class T>
    template <class... Args>
    void list<T>::emplace_back(Args&&... args) 
    {
        link_type node = create_node(mySTL::forward<Args>(args)...);
        link_nodes_at(end(), node, node);
        ++__size;
    }

    template<class T>
    void list<T>::push_back(const_reference x) 
    {
        link_type node = create_node(x);
        link_nodes_at(end(), node, node);
        ++__size;
    }

    template<class T>
    void list<T>::push_front(const_reference x) 
    {
        link_type node = create_node(x);
        link_nodes_at(begin(), node, node);
        ++__size;
    }

    // *** 删除元素 ***

    // 删除单个元素
    template <class T>
    typename list<T>::iterator list<T>::erase(iterator pos)
    {
        assert(pos!=end());
        link_type next = pos.node->next;
        unlink_nodes(pos.node, pos.node);
        destroy_node(pos.node);
        --__size;
        if(__size==0) __node->unlink();
        return next;
    }
    
    // 删除多个元素 [first, last)
    //template <class T>
    template <class T>
    typename list<T>::iterator list<T>::erase(iterator first, iterator last)
    {
        if(first != last)
        {
            unlink_nodes(first.node, last.node->prev);
            while(first!=last)
            {
                destroy_node(first.node);
                ++first;
                --__size;
            }
            if(__size==0) __node->unlink();
        }
        return last;
    }

    template <class T>
    void list<T>::pop_back()
    {
        erase(--end());
    }

    template <class T>
    void list<T>::pop_front()
    {
        erase(++end());
    }

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
            __node->unlink();
            __size = 0;
        }
    }

    template <class T>
    void list<T>::swap(list<T>& other) noexcept
    {
        mySTL::swap(__node, other.__node);
        mySTL::swap(__size, other.__size);
    }

    // *** helper function ***
    template<class T>
    inline void list<T>::empty_init() 
    {
        __node = create_node();
        __node->unlink();
        __size = 0;
    }

    template<class T>
    inline void list<T>::fill_init(size_type n, const_reference value) 
    {
        empty_init();
        __size = n;
        try {
            while (n-- > 0) {
            link_type node = create_node(value);
            link_nodes_at(end(), node, node);
            }
        } catch (...) {
            clear();
            node_allocator::deallocate(__node);
            __node = nullptr;
            throw;
        }
    }

    template<class T>
    template <class InputIterator>
    inline void list<T>::copy_init(InputIterator first, InputIterator last) 
    {
        empty_init();
        try 
        {
            while (first != last) 
            {
                auto node = create_node(*first);
                link_nodes_at(end(), node, node);
                ++first;
                ++__size;
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

    template <class T>
    inline typename list<T>::iterator list<T>::link_nodes_at(iterator pos, link_type first,
                                  link_type last) 
    {
        pos.node->prev->next = first;
        first->prev = pos.node->prev;
        last->next = pos.node;
        pos.node->prev = last;
        return first;
    }

    template <class T>
    inline void list<T>::unlink_nodes(link_type& first, link_type& last)
    {
        first->prev->next = last->next;
        last->next->prev = first->prev;
    }

    template <class T>
    void list<T>::fill_assign(size_type n, const value_type& value)
    {
        iterator first = begin(),
                 last  = end();

        for(;n>0 && first!=last;n--,++first)
            *first = value;
 
        if(n>0) // 如果还有元素没赋值
            insert(last, n, value);
        else // 如果原来的list有多余元素
            erase(first, last);
    }

    template <class T>
    template <class InputIterator>
    void list<T>::copy_assign(InputIterator first, InputIterator last)
    {
        iterator thisFirst = begin(),
                 thisEnd   = end();

        for(;first!=last && thisFirst!=thisEnd; ++first, ++thisFirst)
            *thisFirst = *first;
        
        if(first!=last) // 如果还有元素没赋值
            insert(thisEnd, first, last);
        else // 如果原来的list有多余元素
            erase(thisFirst, thisEnd);
    }

    template <class T>
    typename list<T>::iterator list<T>::fill_insert(iterator pos, size_type n, const_reference value)
    {
        link_type pos_node = create_node(value);
        link_type curr = pos_node;
        for(int i = 1; i<n; i++)
        {
            link_type tmp = create_node(value);
            curr->next = tmp;
            tmp->prev = curr;
            curr = curr->next;
        }
        link_nodes_at(pos.node, pos_node, curr);
        __size += n;
        return pos_node;
    }

    template <class T>
    template <class InputIterator>
    typename list<T>::iterator list<T>::copy_insert(iterator pos, InputIterator first, InputIterator last)
    {
        link_type pos_node = create_node(*first);
        link_type curr = pos_node;
        ++first;
        while(first!=last)
        {
            link_type tmp = create_node(*first);
            curr->next = tmp;
            tmp->prev = curr;
            curr = curr->next;
            ++first;
            ++__size;
        }
        link_nodes_at(pos.node, pos_node, curr);
        return pos_node;
    }
}
#endif // __LIST_H__