#include <iostream>
#include <cassert>
#include <vector>

#include "ponder/macro/toolkits.hpp"


int main()
{



    int lenv1 = PP_ARGC_COUNT();          // -> 0
    auto lenv2 = PP_ARGC_COUNT(foo);       // -> 1
    auto lenv3 = PP_ARGC_COUNT(foo());     // -> 1
    auto lenv4 = PP_ARGC_COUNT(());        // -> 1
    auto lenv5 = PP_ARGC_COUNT(()foo);     // -> 1
    auto lenv6 = PP_ARGC_COUNT(PP_EMPTY);  // -> 1
    auto lenv7 = PP_ARGC_COUNT(PP_COMMA);  // -> 1
    auto lenv8 = PP_ARGC_COUNT(, );        // -> 2
    auto lenv9 = PP_ARGC_COUNT(foo, bar);  // -> 2
    auto lenv10 = PP_ARGC_COUNT(, , , );    // -> 4

    std::cout << "what: " << lenv1 << std::endl;

}