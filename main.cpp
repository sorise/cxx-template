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

constexpr int max_size = 100; // 编译时确定的最大尺寸

template<int A, int B>
struct Add {
    constexpr static const int value = A + B;
};

template<size_t A, size_t B>
constexpr size_t Sub(){
    return A - B;
}

template<size_t N> constexpr size_t fibonacci =  fibonacci<N-1> + fibonacci<N-2>;
template<> constexpr size_t fibonacci<0> = 0;
template<> constexpr size_t fibonacci<1> = 1;

int main()
{
    std::cout <<std::format("value: {0}!\n", fibonacci<10>);
    return 0;
}
