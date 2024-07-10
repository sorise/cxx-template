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
#include "ponder/generics/cls/cls.hpp"

using namespace ponder::generics::cls;


int main()
{
    dog_counter d1;

    std::cout << "DogCount : " << d1.get_count() << std::endl;

    {
        dog_counter d2;
        std::cout << "DogCount : " << d1.get_count() << std::endl;
    }

    std::cout << "DogCount : " << d1.get_count() << std::endl;

    cat_counter c1, c2, c3, c4, c5[3];
    std::cout << "CatCount : " << c1.get_count() << std::endl;

    std::thread t1([](){
        cat_counter c1, c2, c3, c4, c5[3];
        std::cout << "t1 CatCount : " << c1.get_count() << std::endl;
    });

    std::thread t2([](){
        cat_counter c1, c2, c3, c4, c5[3];
        std::cout << "t2 CatCount : " << c1.get_count() << std::endl;
    });

    t1.join();
    t2.join();
}
