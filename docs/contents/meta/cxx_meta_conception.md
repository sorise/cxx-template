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

### [2 模板启航](#)
C++语言中的模板有很多种，模板分为函数模板、类模板、变量模板、别名模板、可变参数模板、将模板作为模板参数等，**在一切开始之前希望
各位对** [**以上的C++基础知识又基本了解**](https://github.com/sorise/Cxx-Learning/tree/main/Grammar)，它们是前进的基石。