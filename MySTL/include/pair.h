#ifndef __PAIR_H__
#define __PAIR_H__

namespace mySTL 
{
    /**
     * @brief pair 
     * 保存2种不同类型数据
     * @tparam T1 
     * @tparam T2 
     */
    template <class T1, class T2>
    struct pair
    {
        typedef T1 first_type;
        typedef T2 second_type;

        first_type  first;    // 保存第一个数据
        second_type second;   // 保存第二个数据

        // default constructor
        


    };
}
#endif // __PAIR_H__