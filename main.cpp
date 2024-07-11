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



int main()
{
   std::shared_ptr<car> byd1 = std::make_shared<car>("xiaomi su7");
   std::shared_ptr<car> byd2 = byd1->get_shared();

   byd1->show_name();
   byd2->show_name();
}
