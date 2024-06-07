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
所谓模板，实际上是建立一个通用函数或类，其类内部的类型和函数的形参类型不具体指定，用一个虚拟的类型来代表。这种通用的方式称为模板。

C++语言中的模板有很多种，模板分为函数模板、类模板、变量模板、别名模板、可变参数模板、将模板作为模板参数等，**在一切开始之前希望
各位对** [**以上的C++基础知识有基本的了解**](https://github.com/sorise/Cxx-Learning/tree/main/Grammar)，因为它们是之后前进的基石。

> **注意**：不推荐对C++ 基本语法没有掌握的朋友继续阅读，因为阅读本章及其后续需要先了解C++语法。

### [3. 函数模板](#)
C++ 函数模板是一种让函数能够处理不同数据类型的机制。通过函数模板，程序员可以编写一个函数，并在需要时指定该函数可以接受的具体数据类型。这种机制大大提高了代码的重用性和灵活性。以下是有关 C++ 函数模板的基本知识内容。

函数模板定义形式
由以下三部分组成： **模板说明** + **函数定义** + **函数模板调用(模板实例化)**

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
如下是一个模板函数，通过模板参数索引获取数组元素，并且使用模板函数重载支持数组、vector、std::array，由此也可以看到
模板也是支持重载的。
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

#### 3.3 函数模板实例化
函数模板只是一个用于生成函数定义的方案,本身并不会生成函数定义(不被使用某个类型调用)， 编译器使用模板为特定类型生成函数定义时，得到的是模板实例。

C++ 现在支持 **隐式实例化** **显示实例化** **显式具体化**
* **隐式实例化**：**借助自动类型推导实现**，swap(1,2)这个过程就是一个隐式实例化的过程，它实例化生成了一个T为int的函数。因此我们得知，隐式实例化实际上就是模板函数根据传入的实参类型实例化一个函数的过程。
* **显式实例化**：**void swap(double & x, double& y);** 显示实例化也称为外部实例化。在不发生函数调用的时候讲函数模板实例化，或者在不适用类模板的时候将类模板实例化称之为模板显示实例化。
* **显式具体化**： template<> void swap(Job& x, Job& y),为特定类型实现， 需要在模板前加 **template<>**。
```cpp
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

struct Job
{
    char name[40];
    double salary;
    int floor;
};

template void swap<char>(char &, char &); //显式实例化

//显示具体化
template<> void swap<Job>(Job& x, Job& y) {
    auto tempSalary = x.salary;
    auto tempFloor = x.floor;

    x.salary = y.salary;
    x.floor = y.floor;
    y.salary = tempSalary;
    y.floor = tempFloor;
}


int test()
{
    int a = 10, b =20;
    swap<float>(a, b); //隐式实例化,编译器编译到此处时才生成函数定义
}
```

##### 3.3.1 第三代具体化 IOS/ANSI C++ 标准
* 对于给定函数名，可以有非模板函数，模板函数和显示具体化模板函数以及它们的重载版本
* 显式具体化的原型和定义应以 template<> 打头，并通过名称来制定类型
* 具体化优先于常规模板，而非模板函数优先于具体化和常规模板的原型！ 非模板函数 > 具体化模板 > 常规模板

#### 3.4 类型推断
C++ 模板参数的类型推断是一种机制，使编译器能够自动确定模板函数调用时使用的实际类型。这种机制使模板函数更具灵活性和易用性。以下是关于 C++ 模板参数类型推断的详细介绍。

```cpp
int aint = 10, bint = 20;
swap(aint, bint); //T 推导为 int

double adouble = 10.0, bdouble = 20.0;
swap(adouble, bdouble); //T 推导为 double

Job job1 = {"job1", 1000, 1};
Job job2 = {"job2", 2000, 2};
swap<Job>(job1,job2); //直接告诉编译器，T 为 Job 类型

std::array<int, 5> arr{1, 2, 3, 4, 5};
int& ref2 = get_ref_array<3>(arr);
//_IDX = 3, _Ele_Ty = int, _ARY_SIZE = 5
//_IDX 参数由用户给出，_Ele_Ty 和 _ARY_SIZE 由编译器推导得到
```

**自动类型推导并非万能**，有时候推导失败，需要手动指定类型， 还可以借助 decltype 推导类型。

其中V的类型无法被推到出来，因为V即可以为int，又可为double 或者 float。
```cpp
template <typename T,typename U,typename V>
V ADD(T tv1, U tv2) {
    return tv1 + tv2;
}

auto v = ADD(10.15,95);
```
**decltype 结合 auto 后置语法**

```cpp
template <typename T,typename U>
auto ADD(T tv1, U tv2) -> decltype(tv1 + tv2) {
    return tv1 + tv2;
}

auto v = ADD(10.15,95); //double 类型
```

#### 3.5 模板参数约束
有时候我们需要对模板参数进行约束，比如限定参数只能是数值类型，或者限定参数只能是类类型。
在C++ 20之前，我们可以使用元编程库提供的**std::is_arithmetic** 、**std::is_class**、**std::enable_if** 来实现。

```cpp
template <typename T,typename U,
    std::enable_if_t<std::is_arithmetic_v<T>, int> = 0,
    std::enable_if_t<std::is_arithmetic_v<U>, int> = 0
>
auto ADD(T tv1, U tv2) -> decltype(tv1 + tv2) {
    return tv1 + tv2;
}
```

要求模板参数为类或者结构体类型，但是必须支持operator+,实现如下所示：
```cpp
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
auto CLASS_ADD(T tv1, U tv2) ->
std::enable_if_t<
        std::is_class_v<T> && std::is_class_v<U> &&
        has_plus_operator_v<T> && has_plus_operator_v<U>,
        decltype(tv1 + tv2)
> {
    return tv1 + tv2;
}

Job _j1 {"123" ,10,  100};
Job _j2 {"123" ,10,  100};
auto v1 = CLASS_ADD( S1, S2); //编译通过
auto v2 = CLASS_ADD(_j1, _j2); //编译失败
```
**增加一个类型约束:** 使用 **C++20 requires** 关键字来增加类型约束, 该关键字可以防止污染模板参数列表。

requires 关键字是 C++20 引入的一个强大的特性，用于声明概念和在模板中指定约束条件。它可以显著提高模板编程的表达力和类型安全性，使代码更易读、更易维护。


概念（Concepts） 是一组约束，用于限制模板参数必须满足的条件,类似于类型特征（type traits），但更具表达力和易用性。
```cpp
template <typename T,typename U>
requires (has_plus_operator_v<T> && has_plus_operator_v<U> && std::is_class_v<T> && std::is_class_v<U>)
auto CLASS_ADD(T tv1, U tv2) -> decltype(tv1 + tv2)
{
    return tv1 + tv2;
}
```


