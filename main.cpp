#include <iostream>
#include <cassert>
#include <memory>
#include <vector>

#include "ponder/macro/autoget_set.hpp"
#include "ponder/generics/func/func.hpp"


int main()
{
    auto v = ADD(10.15,95);

    std::string S1("ss1s");
    std::string S2("ss1s");
    Job _j1 {"123" ,10,  100};
    Job _j2 {"123" ,10,  100};
    auto v1 = CLASS_ADD( S1, S2);


    std::cout << std::is_class_v<std::string> << std::endl;
}