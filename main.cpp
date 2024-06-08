#include <iostream>
#include <cassert>
#include <memory>
#include <vector>
#include <queue>

#include "ponder/macro/autoget_set.hpp"
#include "ponder/generics/func/func.hpp"

int i = 10;

class A {
public:
    static int count;
    A() = default;
    A(int a, int b) : a(a), b(b) {}
private:
    int a;
    int b;
};

int A::count = 0;

template<int* ptr>
struct PointerWrapper {
    void set(int value) {
        *ptr = value;
    }
};

int main()
{
    PointerWrapper<&i> ref_wrapper;
    PointerWrapper<&A::count> ref_upper;
}