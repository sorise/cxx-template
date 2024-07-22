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

template<typename T, typename =
std::enable_if_t<std::is_array_v<T>>>
void foo (T&& arg1, T&& arg2)
{
    //…
}


int main()
{

    return 0;
}
