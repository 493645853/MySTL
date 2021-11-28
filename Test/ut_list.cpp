#include "test_aux.h"
#include "list.h"
#include "iterator"
#include <iostream>
#include <string>


int main(int argc, char *argv[])
{
    mySTL::list<std::string> myList(10, "hh");
    // for(int i = -10;i<10;i++)
    // {
    //     myList.push_back(i);
    // }
    printContainer(myList);

    std::cout << std::endl;
    return 0;
}