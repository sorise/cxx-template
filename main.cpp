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

void process(int&& i){
    std::cout << "&&i = " << i << std::endl;
}

void process(int& i){
    std::cout << "&i = " << i << std::endl;
}

void process(const int& i){
    std::cout << "ci = " << i << std::endl;
}

template<typename T>
void universal_process(T&& t){
    process(std::forward<T>(t));
}

int main()
{

    int a;
    int& r = a;
    int&& rr = std::move(a);//std::move()函数位于头文件中，该函数名字具有迷惑性，它并不搬移任何东西
    //唯一的功能就是将一个左值强制转化为右值引用，然后实现移动语义
    std::cout << &r << std::endl;
    std::cout << &rr << std::endl;  //我们知道右值不能取地址，不能被修改，而这里都能正常打印
    //证明结论：右值引用变量的属性会被编译器识别成左值

}
