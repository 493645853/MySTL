#include <iostream>
#include "allocator.h"


int main(int argc, char *argv[])
{
    int n = 10;
    int *p = mySTL::allocator<int>::allocate(n);
    for(int i=0;i<n;i++)
        std::cout << *(p+i) << " ";
    std::cout << std::endl;
    mySTL::allocator<int>::deallocate(p);
    return 0;
}