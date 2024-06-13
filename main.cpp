#include <iostream>
#include <vector>
#include <format>

#include "ponder/macro/autoget_set.hpp"
#include "ponder/generics/func/func.hpp"

int shit = 10;

template<typename T>
class MyClass {
    int i = 0;
public:
    template<class T1>
    requires std::is_arithmetic_v<T1>
    explicit MyClass(T1 v1){
        this->i = static_cast<int>(v1);
        std::cout << std::format("call copy constructor template func {}\n", i);
    }

    template<class T1>
    requires std::is_arithmetic_v<T1>
    explicit MyClass(const T1& v1){
        this->i = static_cast<int>(v1);
        std::cout << std::format("call copy constructor template func {}\n", i);
    }

    template<class U>
    requires std::is_arithmetic_v<U>
    MyClass<T> & operator =(const MyClass<U>& other){
        this->i = other.get_i();
        std::cout << std::format("call operator = template func {}\n", i);
        return *this;
    }

    [[nodiscard]] int get_i() const {
        return i;
    }
};


int main()
{
    MyClass<int> a(5.12);
    MyClass<double> b(10.12);
    a = b;

    std::cout << std::format("a.i = {}\n", a.get_i());


}