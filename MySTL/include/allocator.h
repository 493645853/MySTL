#ifndef __ALLOCATOR_H__
#define __ALLOCATOR_H__

#include <cstddef>
#include <new>
namespace mySTL
{
    /**
     * @brief 模板类： allocator
     * 
     * 
     * @tparam T 
     */
    template <class T>
    class allocator
    {
    public:
        typedef T           value_type;
        typedef T*          pointer;
        typedef const T*    const_pointer;
        typedef T&          referece;
        typedef const T&    const_reference;
        typedef size_t      size_type; 
        typedef ptrdiff_t   difference_type; // pointer diff type

    public:
        // allocate & deallocate, use static method for directly usage
        static pointer allocate();
        static pointer allocate(size_type n);

        static void deallocate(pointer ptr);
        static void deallocate(pointer ptr, size_type n);
    };

    /**
     * @brief Implementation
     * 
     */
    
    template <class T>
    T* allocator<T>::allocate()
    {
        return static_cast<pointer>(::operator new(sizeof(T)));
    }

    template <class T>
    T* allocator<T>::allocate(size_type n)
    {
        if(n==0) return nullptr;
        return static_cast<pointer>(::operator new(n*sizeof(T)));
    }

    template <class T>
    void allocator<T>::deallocate(pointer ptr)
    {
        if(!ptr) return;
        ::operator delete(ptr);
    }

    template <class T>
    void allocator<T>::deallocate(pointer ptr, size_type)
    {
        if(!ptr) return;
        ::operator delete(ptr);
    }

    
}
#endif // __ALLOCATOR_H__