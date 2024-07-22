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
void foo (T const& arg1, T const& arg2)
{
    //todo
}

int main()
{
    foo("hi", "guy"); //ERROR
    return 0;
}
