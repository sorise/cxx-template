#include <iostream>
#include <cassert>
#include <memory>
#include <vector>

#include "ponder/macro/autoget_set.hpp"
#include "ponder/generics/func/func.hpp"


int main()
{
    std::vector<int> vec{1, 2, 3, 4, 5};
    int& ref = get_ref_array<3>(vec);
    ref = 10;

    assert(vec[3] == 10);


    std::array<int, 5> arr{1, 2, 3, 4, 5};
    int& ref2 = get_ref_array<3>(arr);
    ref2 = 12;
    assert(arr[3] == 12);

    int arr2[] = {1, 2, 3, 4, 5};

    int& ref3 = get_ref_array<3>(arr2);
    ref3 = 13;
    assert(arr2[3] == 13);
}