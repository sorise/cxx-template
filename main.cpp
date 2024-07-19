#include <iostream>
#include <vector>
#include <format>
#include <stack>
#include <map>
#include <algorithm>
#include <concepts>

#include "ponder/macro/autoget_set.hpp"
#include "ponder/generics/func/func.hpp"
#include "ponder/generics/cls/vari.h"
#include "ponder/generics/cls/para.h"
#include "ponder/generics/cls/cls.hpp"
#include "ponder/generics/cls/conception.h"

using namespace ponder::generics::cls;

template<typename T>
void muse_constant(const T& arg){
    std::cout << typeid(arg).name() << std::endl;
}

template<typename T>
requires std::negation<std::bool_constant<std::is_const_v<T>>>::value
void muse_ref(T& arg)
{
    if constexpr(std::is_const_v<T>){
        std::cout << "const " << typeid(arg).name() << std::endl;
    }else{
        std::cout << typeid(arg).name() << std::endl;
    }
}

int main()
{
    const int v = 0;
    muse_ref(v); //const int

    int vs[] = {1,2,3,4,5,6};
    muse_ref(vs); //int [6]

    const std::string s = "hello";
    muse_ref(s); //const std::string

    int i = 0;
    muse_ref(i);  //int
    return 0;
}
