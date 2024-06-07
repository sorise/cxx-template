#include <iostream>
#include <cassert>
#include <vector>

#include "ponder/macro/autoget_set.hpp"



int main()
{
    student kit;
    kit.set_name("kit");
    kit.set_age(17);

    std::cout << kit.get_name() << std::endl;
    std::cout << kit.get_age() << std::endl;
}