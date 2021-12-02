#include "utils.h"
#include <iostream>
#include <ostream>


int main(int argc, char *argv[])
{
    const int N = 4;
    int a[N] = {1,2,3,4};
    int b[N] = {4,3,2,1};

    // get array length
    std::cout << "array length: " << mySTL::getArrayLen(a) << std::endl;

    // swap by range
    mySTL::swap_range(a, a+4, b);

    for(int i = 0;i<N;i++)
    {
        std::cout << a[i] << " " << b[i] << std::endl;
    }
    
    // swap the array
    mySTL::swap<int,N>(a, b);
    for(int i = 0;i<N;i++)
    {
        std::cout << a[i] << " " << b[i] << std::endl;
    }
    return 0;
}
