//
// Created by 14270 on 2024/7/24.
//

#ifndef CXX_TEMPLATE_TRAITS_H
#define CXX_TEMPLATE_TRAITS_H
#include <type_traits>

namespace ponder::generic::meta {
    template<typename T>
    struct AccumulationTraits;

    template<>
    struct AccumulationTraits<char> {
        using AccT = int;
    };

    template<>
    struct AccumulationTraits<signed short> {
        using AccT = int;
    };

    template<>
    struct AccumulationTraits<unsigned short> {
        using AccT = unsigned int;
    };

    template<>
    struct AccumulationTraits<int> {
        using AccT = long;
    };

    template<>
    struct AccumulationTraits<unsigned int> {
        using AccT = unsigned long;
    };

    template<>
    struct AccumulationTraits<float> {
        using AccT = double;
    };

    template<>
    struct AccumulationTraits<double> {
        using AccT = double;
    };

    template<typename T>
    requires std::is_arithmetic_v<T>
    T accum (T const* beg, T const* end)
    {
        using ResultType = typename AccumulationTraits<T>::AccT ; // assume this actually creates a zero value
        ResultType total{};
        while (beg != end) {
            total += *beg;
            ++beg;
        }
        return total;
    }
}
#endif //CXX_TEMPLATE_TRAITS_H
