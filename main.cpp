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
#include "ponder/generics/cls/conception.h"

using namespace ponder::generics::cls;




int main()
{
    using F = void(int,float,std::vector<float>);
    static_assert(std::is_same_v<function_traits<F>::return_type, void>);
    static_assert(function_traits<F>::args_size == 3);
    static_assert(std::is_same_v<function_traits<F>::arg<2>, std::vector<float>>);
    return 0;
}
