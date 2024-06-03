#include <iostream>
#include <cassert>
#include <vector>



#define ABDEF 1
#undef ABDEF

#ifdef ABDEF
    #define ABDEF_VALUE 1
#else
    #define ABDEF_VALUE 0
#endif


int main()
{
    std::cout << "ABDEF_VALUE = " << ABDEF_VALUE << std::endl;
}