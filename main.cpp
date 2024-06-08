#include <iostream>
#include <cassert>
#include <memory>
#include <vector>
#include <queue>

#include "ponder/macro/autoget_set.hpp"
#include "ponder/generics/func/func.hpp"

int shit = 10;


int main()
{
    set_ref<int, shit>();

    std::cout << shit << std::endl; // 0
}