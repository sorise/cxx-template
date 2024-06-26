#include <iostream>
#include <vector>
#include <format>
#include <stack>
#include <map>
#include <algorithm>

#include "ponder/macro/autoget_set.hpp"
#include "ponder/generics/func/func.hpp"
#include "ponder/generics/cls/vari.h"
#include "ponder/generics/cls/para.h"

using namespace ponder::generics::cls;



int main()
{
    std::cout << "max int value:" <<ponder::generics::cls::max_value<int> << std::endl;
    std::cout << "min int value:" <<ponder::generics::cls::min_value<> << std::endl;

    Str_Map<double> scores {{"2021", 100.0},{"2022", 121.0},{"2023", 177.0}};

    std::for_each(scores.begin(), scores.end(), [](auto& item) {
       std::cout << std::format("{}:{}", item.first, item.second) << std::endl;
    });


    unit<int, double> u{100};
    std::cout << std::format("cell:{}", u.cell) << std::endl;
    u.grid = 189.14;
    std::cout << std::format("grid:{}", u.grid) << std::endl;

    graduate<int> me;
    me.set_score(100);
    me.get_score();

    object obj;
    obj.call();

    swagger<int, double,bool> sw;
    auto sizes = sw.args_size();

    for (auto v : sizes) {
        std::cout << std::format("size:{} ", v);
    }

}
