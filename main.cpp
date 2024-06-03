#include <iostream>



struct PackedStruct {
    char a;
    int b;
};

int main()
{
    PackedStruct s{'c', 12};
    std::cout << sizeof(s) << std::endl; //5
}