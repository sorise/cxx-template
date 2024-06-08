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

struct Job
{
    char name[40];
    double salary;
    int floor;
};

template<typename T>
void swap(T& x, T& y) {
    auto temp = x;
    x = y;
    y = temp;
};

template<typename T>
void swap(T x[], T y[], int size) {
    T temp;
    for (int i = 0; i < size; i++)
    {
        temp = x[i];
        x[i] = y[i];
        y[i] = temp;
    }
}

void swap(int& x, int& y);
void swap(double & x, double& y);

template <typename T,typename U,
    std::enable_if_t<std::is_arithmetic_v<T>, int> = 0,
    std::enable_if_t<std::is_arithmetic_v<U>, int> = 0
>
auto ADD(T tv1, U tv2) -> decltype(tv1 + tv2) {
    return tv1 + tv2;
}

// 检查类型 T 是否支持 operator+
template <typename T>
auto has_plus_operator(int) -> decltype(std::declval<T>() + std::declval<T>(), std::true_type{}) {
    return std::true_type{};
}

template <typename T>
std::false_type has_plus_operator(...) {
    return std::false_type{};
}

template <typename T>
constexpr bool has_plus_operator_v = decltype(has_plus_operator<T>(0))::value;

template <typename T,typename U>
requires has_plus_operator_v<T> && has_plus_operator_v<U> && std::is_class_v<T> && std::is_class_v<U>
auto CLASS_ADD(T tv1, U tv2) -> decltype(tv1 + tv2)
{
    return tv1 + tv2;
}


template<int* ptr>
struct PointerWrapper {
    void set(int value) {
        *ptr = value;
    }
};

template<int& ptr>
struct refWrapper {
    void set(int value) {
        ptr = value;
    }
};

template<typename Ref_Type, Ref_Type& ptr>
void set_ref(const Ref_Type& value) {
    ptr = value;
}


#endif //CXX_TEMPLATE_FUNC_HPP
