## [C++ 模板、泛型编程和元编程](#)
> **介绍**: C++ 模板、泛型编程和元编程是 C++ 语言中强大的编程概念，它们相互关联，但各自有特定的用途和特点。

----



### [1. 基本概念](#)
C++ 模板、泛型编程和元编程是三个关联的概念，有区别也有联系，不可混为一谈。

**C++ 模板:** 是一种允许在编译时生成代码的机制，使得编写通用的、类型无关的代码成为可能，分为**函数模板**和**类模板**。
```cpp
#include <iostream>
#include <vector>
#include <algorithm>

//函数模板
template <typename T>
T add(T a, T b) {
    return a + b;
}
//类模板
template <typename T>
class Container {
private:
    T element;
public:
    Container(T arg) : element(arg) {
        
    }
    T getElement() {
        return element;
    }
};
```
**泛型编程**: 是一种编程范式，指使用模板编写能够处理不同数据类型的通用代码的一种编程范式。其核心思想是编写算法和数据结构，使它们能在不同的上下文中使用，而不需要修改代码。**C++ 模板是实现泛型编程的基础** 。
```cpp
//泛型编程
template <typename Container>
void print(const Container& container) {
    for (const auto& element : container) {
        std::cout << element << " ";
    }
    std::cout << std::endl;
}

int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5};
    print(vec);  // 泛型算法
    
    std::sort(vec.begin(), vec.end());  // STL 泛型算法
    print(vec);
    
    //编译时就计算完毕了，程序运行后 Factorial<5>::value 直接就是 120 没有函数调用
    std::cout << Factorial<5>::value << std::endl;  // 输出 120
    
    return 0;
}
```
**元编程**:元编程是一种编程技术，它允许在编译时生成代码。C++ 元编程主要是通过模板实现的。**元编程常用于编写高效的、在编译时就能完成计算的代码**。
```cpp
// 计算阶乘的模板元编程示例
template <int N>
struct Factorial {
    static const int value = N * Factorial<N - 1>::value;
};

template <>
struct Factorial<0> {
    static const int value = 1;
};

int main() {
    //编译时就计算完毕了，程序运行后 Factorial<5>::value 直接就是 120 没有函数调用
    std::cout << Factorial<5>::value << std::endl;  // 输出 120
    
    return 0;
}
```

**总结**
* **模板** 提供了一种在编译时生成通用代码的方法。
* **泛型编程** 是一种编程范式，利用模板编写类型无关的代码。
* **元编程** 是一种编译时编程技术，通过模板在编译时生成代码和进行计算。
> 这些技术使得 C++ 语言能够编写高效、灵活和可重用的代码，极大地增强了其功能和表现力。

### [2. 模板启航](#)
C++语言中的模板有很多种，模板分为函数模板、类模板、变量模板、别名模板、可变参数模板、将模板作为模板参数等，**在一切开始之前希望
各位对** [**以上的C++基础知识有基本的了解**](https://github.com/sorise/Cxx-Learning/tree/main/Grammar)，因为它们是之后前进的基石。

> **注意**：不推荐对C++ 基本语法没有掌握的朋友继续阅读，因为阅读本章及其后续需要先了解C++语法。

### [3. 函数模板](#)
C++ 函数模板是一种让函数能够处理不同数据类型的机制。通过函数模板，程序员可以编写一个函数，并在需要时指定该函数可以接受的具体数据类型。这种机制大大提高了代码的重用性和灵活性。以下是有关 C++ 函数模板的基本知识内容。

#### 3.1 创建一个模板
C++98 添加 typename 关键字之后，使用 typename 来从创建模板。之前使用关键字class来创建模板。

如下是创建一个函数模板实例，为标准库函数make_unique，用于创建一个数组类型的unique_ptr。其中借助了三个元编程库函数。
* [std::enable_if、std::enable_if_v](https://zh.cppreference.com/w/cpp/types/enable_if) 只有满足条件时，返回true，才能顺利编译通过。
* [std::extent、std::extent_v](https://zh.cppreference.com/w/cpp/types/extent) 获得数组维度。
* [std::is_array、std::is_array_v](https://zh.cppreference.com/w/cpp/types/is_array) 判断类型是否为数组。

以下要求模板参数为数组类型，且数组维度为0。
```cpp
template <typename T>
struct is_array : std::false_type {};

template <typename T>
struct is_array<T[]> : std::true_type {};

template <typename T, std::size_t N>
struct is_array<T[N]> : std::true_type {};

template <typename T>
inline constexpr bool is_array_v = is_array<T>::value;

_EXPORT_STD template <
        class _Ty, 
        std::enable_if_t<std::is_array_v<_Ty> && std::extent_v<_Ty> == 0, int> = 0
>
[[nodiscard]] constexpr std::unique_ptr<_Ty> make_unique(const size_t _Size) { 
    // make a unique_ptr
    using _Elem = std::remove_extent_t<_Ty>;
    return std::unique_ptr<_Ty>(new _Elem[_Size]());
}
```
使用例子：
```cpp
std::unique_ptr<int[]> scores = std::make_unique<int[]>(10);
scores[0] = 10;

std::cout << "scores[0] = " << scores[0] << std::endl;

//std::extent_v<_Ty> == 0 的目的禁止如下语句被编译通过
auto other = std::make_unique<int[10]>(10);
//std::is_array_v<_Ty> 禁止如下语句被编译通过
std::unique_ptr<int[]> another = std::make_unique<int>(10);
```

#### 3.2 使用非类型参数作为模板参数
如下是一个模板函数，通过模板参数索引获取数组元素，并且使用函数重载支持数组、vector、std::array。
```cpp
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
```
使用例子：
```cpp
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
```

**增加一个类型约束:** 使用 **C++20 requires** 关键字来增加类型约束。


