#ifndef CXX_TEMPLATE_PARA_H
#define CXX_TEMPLATE_PARA_H

#include <vector>

namespace ponder::generics::cls{

    template<typename T, typename ...Args>
    class swagger{

    public:
        constexpr size_t args_count() noexcept{
            return sizeof...(Args);
        }

        constexpr std::array<size_t, sizeof...(Args) + 1> args_size(){
            return {sizeof(T), sizeof(Args)...};
        }
    };

}
#endif //CXX_TEMPLATE_PARA_H
