#include <iostream>
#include <cassert>
#include <vector>

#define SDEF(sname, ...) std::vector<int> sname __VA_OPT__(= { __VA_ARGS__ })

int main()
{
    SDEF(s1, 1, 2, 3);

    std::cout << s1[0] << std::endl;
}