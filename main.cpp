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

template<size_t N> constexpr double golden_ratio = fibonacci<N + 1> * 1.0  / fibonacci<N>;

template<typename T, T... ints>
void print_sequence(std::integer_sequence<T, ints...> int_seq)
{
    std::cout << "size: " << int_seq.size() << ", sequence: ";
    ((std::cout << ints << ' '), ...);
    std::cout << '\n';
}


template<class T>
std::enable_if<std::is_arithmetic<T>::value, long>::type
ADD_M(T&& v, T&& v2) {
    auto t1 = static_cast<long>(v + v2);
    auto t2 = static_cast<long>(v);
    return t1 % t2;
} // #1

int main()
{
    ::printf("number: %ld\n",ADD_M(15.1, 220.2));
    return 0;
}
