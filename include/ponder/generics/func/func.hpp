#ifndef CXX_TEMPLATE_FUNC_HPP
#define CXX_TEMPLATE_FUNC_HPP

#include <array>
#include <vector>
#include <memory>
#include <exception>
#include <stdexcept>



_EXPORT_STD template <class _Ty, std::enable_if_t<std::is_array_v<_Ty> && std::extent_v<_Ty> == 0, int> = 0>
[[nodiscard]] constexpr std::unique_ptr<_Ty> my_make_unique(const size_t _Size) { // make a unique_ptr
    using _Elem = std::remove_extent_t<_Ty>;
    return std::unique_ptr<_Ty>(new _Elem[_Size]());
}

template<size_t _IDX, typename _Ele_Ty>
[[nodiscard]] constexpr _Ele_Ty& get_ref_array(_Ele_Ty* collection){
    return collection[_IDX];
}


template<size_t _IDX, typename _Ele_Ty,size_t _ARY_SIZE>
[[nodiscard]] constexpr _Ele_Ty& get_ref_array(std::array<_Ele_Ty,_ARY_SIZE>& collection){
    static_assert(_IDX < _ARY_SIZE);
    return collection[_IDX];
}


template<size_t _IDX, typename _Ele_Ty>
[[nodiscard]] _Ele_Ty& get_ref_array(std::vector<_Ele_Ty>& collection){
    if (collection.size() <= _IDX){
        throw std::logic_error("index out of range");
    }
    return collection[_IDX];
}


#endif //CXX_TEMPLATE_FUNC_HPP
