#include <iostream>

#ifdef _WIN32
    #include <windows.h>
#else
    #error "only support windows"
#endif


#if defined(_WIN32)
    #include <windows.h>
#else
    #error "only support windows"
#endif


class A{
    [[nodiscard("show result")]] int test(){
        return 1;
    }
};

int main()
{

}