#include <iostream>
#include <vector>
#include <format>
#include <stack>
#include <map>

#include "ponder/macro/autoget_set.hpp"
#include "ponder/generics/func/func.hpp"
#include "ponder/generics/cls/vari.h"



template <typename T> class D {
public:
    template <typename W> static W STP; //静态成员变量模板声明
};

template <typename T>
template <typename W>
W D<T>::STP = 5;

int main()
{
    std::cout << "max int value:" <<ponder::generics::cls::max_value<int> << std::endl;
    std::cout << "min int value:" <<ponder::generics::cls::min_value<> << std::endl;





}