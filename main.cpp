#include <iostream>
#include <cassert>
#include <memory>
#include <vector>

#include "ponder/macro/autoget_set.hpp"
#include "ponder/generics/func/func.hpp"


int main()
{
    auto v = ADD(10.15,95);

    std::string S1("ss1s");
    std::string S2("ss1s");
    Job _j1 {"123" ,10,  100};
    Job _j2 {"123" ,10,  100};
    auto v1 = CLASS_ADD( S1, S2);

    const int value = 12;

    auto val = value; // int 类型

    decltype(value) val_ty = value; //const int 类型

    const int * const ptr  = &value;

    auto pp_val = ptr; //const int *
    decltype(ptr) dpp_val = ptr; //const int * const

    volatile double score = 95.5;

    auto score_a = score; //double

    decltype(score) score_b = score; // volatile double

    volatile double * const ddp = &score;

    auto ddp_a = ddp; //volatile double *
    decltype(ddp) ddp_b = ddp; //volatile double * const

    float height = 1.75;
    float & ref_height = height;
    const float & ref_height_const = height;

    decltype(ref_height) drh_ = ref_height; //float &
    decltype(ref_height_const) drhc_ = ref_height_const; //const float &
    auto drh_a = ref_height; //float
    auto drh_b = ref_height_const; //float

    std::cout << std::is_class_v<std::string> << std::endl;
}