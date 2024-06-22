//
// Created by 14270 on 24-6-19.
//

#ifndef CXX_TEMPLATE_VARI_H
#define CXX_TEMPLATE_VARI_H

namespace ponder::generics::cls{

    //最大值
    template<typename T>
    inline constexpr T max_value = T(0);

    template<>
    inline constexpr int max_value<int> = 2147483647;

    template<>
    inline constexpr double max_value<double> = 1.79769e+308;

    template<typename T = int >
    inline constexpr T min_value = T(0);

    template<>
    inline constexpr int min_value<int> = -2147483648;


    template<typename T, size_t N> T Variable[N];

}

#endif //CXX_TEMPLATE_VARI_H
