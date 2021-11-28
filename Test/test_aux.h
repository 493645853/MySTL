#ifndef __TEST_AUX_H__
#define __TEST_AUX_H__

#include <chrono>
#include <iostream>
#define COUNT_FUN_TIME(FUN) \
    auto t1 = std::chrono::steady_clock::now();\
    FUN();\
    auto t2 = std::chrono::steady_clock::now();\
    std::cout << "Time Cost: " << std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1).count() \
              << " [s]";\


template <class T>
void printContainer(T& container)
{
    for(auto& i:container)
        std::cout << i << " ";
    std::cout << "\t [size]: " << container.size();
}
#endif // __TEST_AUX_H__