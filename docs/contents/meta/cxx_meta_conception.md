## [C++ 模板、泛型编程和元编程](#)
> **介绍**: C++ 模板、泛型编程和元编程是 C++ 语言中强大的编程概念，它们相互关联，但各自有特定的用途和特点。

----

- [1. 基本概念](#1-基本概念)
- [2. 模板启航](#2-模板启航)
- [3. 函数模板](#3-函数模板)
  - [3.1 创建一个模板](#31-创建一个模板)
  - [3.2 使用非类型参数作为模板参数](#32-使用非类型参数作为模板参数)
  - [3.3 函数模板实例化](#33-函数模板实例化)
  - [3.4 类型推断](#34-类型推断)
  - [3.5 模板参数约束](#35-模板参数约束)
  - [3.6 默认参数](#36-默认参数)
- [4. decltype](#4-decltype)
- [5. auto和decltype](#5-auto和decltype)

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
在C++中，模板不仅可以接受类型参数（如typename或class），还可以接受非类型参数。这些非类型参数用于指定值而不是
类型，可以在模板实例化时传递常量值。

如下是一个模板函数，通过模板参数索引获取数组元素，并且使用模板函数重载支持数组、vector、std::array，由此也可以看到
模板也是支持重载的。
```cpp
//_IDX 索引、_Ele_Ty 元素类型
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
**C++标准规定了非类型模板参数可以是以下几类**：
* 包括整型、枚举类型以及char类型。例如：`template<int N, char C>`
* 指针或引用。例如：`template<int* ptr>、template<int& ptr>` ，**但是指针和引用执行的或者引用的必须是全局变量、静态数据成员**。
* 指向成员的指针。例如：`template<int Job::*ptr>`
* `std::integral_constant` 包装特定类型的静态常量。它是 C++ 类型特征的基类。
* `constexpr` 对象在编译时具有常量值。
```cpp
//注意 ptr指向的地址必须为全局变量，不能为局部变量。
template<int* ptr>
struct PointerWrapper {
    void set(int value) {
        *ptr = value;
    }
};

//注意 ptr指向的引用必须为全局变量，不能为局部变量。
template<int& ptr>
struct refWrapper {
    void set(int value) {
        ptr = value;
    }
};

struct MyClass {
    int value;
};

template<int MyClass::* memberPtr>
struct MemberPointerWrapper {
    void set(MyClass& obj, int value) {
        obj.*memberPtr = value;
    }
};

MemberPointerWrapper<&MyClass::value> wrapper;

template<int N>
struct Factorial {
    static constexpr int value = N * Factorial<N - 1>::value;
};

template<>
struct Factorial<0> {
    static constexpr int value = 1;
};
```
**指针和引用实例化例子**

```cpp
int i = 10;

class A {
public:
    static int count;
    A() = default;
    A(int a, int b) : a(a), b(b) {}
private:
    int a;
    int b;
};

int A::count = 0;

int main()
{
    PointerWrapper<&i> ref_wrapper;
    PointerWrapper<&A::count> ref_upper;
}
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
requires has_plus_operator_v<T> && has_plus_operator_v<U> && std::is_class_v<T> && std::is_class_v<U>
auto CLASS_ADD(T tv1, U tv2) -> decltype(tv1 + tv2)
{
    return tv1 + tv2;
}
```

#### [3.6 默认参数](#)
函数模板可以提供默认的模板参数, 如下所示，_Container 默认为 deque<_Ty>，你也可以指定为 vector<_Ty>。

```cpp
_EXPORT_STD template <class _Ty, class _Container = std::deque<_Ty>>
std::queue<_Ty, _Container> create_queue(){
    //todo    
}

create_queue<std::string>(); //create_queue<std::string, std::deque<std::string>>
create_queue<double, std::vector<double>>();


int shit = 10;

template<typename Ref_Type, typename Ref_Type& ptr, typename Ref_Type default_value = 0>
void set_ref(const Ref_Type& value) {
    ptr = value;
}

template<typename Ref_Type, typename Ref_Type& ptr, typename Ref_Type default_value = 0>
void set_ref() {
    ptr = default_value;
}
set_ref<int, shit>();

std::cout << shit << std::endl; // 0
```

### [4. decltype](#)
decltype 是 C++11 引入的一个关键字，用于获取表达式的类型。
decltype 可以用于类型后置语法、获取函数参数类型、获取变量、函数返回值、数组元素、指针指向的类型等。
> 编译时类型推导，除了我们说过的auto关键字，还有本文的decltype

decltype与auto关键字一样，**用于进行编译时类型推导**，不过它与auto还是有一些区别的。

decltype的类型推导并不是像auto一样是从变量声明的初始化表达式获得变量的类型，而是**总是以一个普通表达式作为参数**，返回该表达式的类型,而且decltype并不会对表达式进行求值。


```cpp
decltype ( 实体 )	//(C++11 起)
decltype ( 表达式 )	//(C++11 起)
```

如果实参是其他类型为 T 的任何表达式，且
* 如果 表达式 是一个没有带括号的标记符表达式或者类成员访问表达式，那么的decltype（表达式）就是表达式所命名的实体的类型。此外，如果表达式是一个被重载的函数，则会导致编译错误。
* 如果 表达式 的值类别是亡值，将会 decltype 产生 T&&；
* 如果 表达式 的值类别是左值，将会 decltype 产生 T&；
* 如果 表达式 的值类别是纯右值，将会 decltype 产生 T。

```cpp
template<typename T1, typename T2>
?type? Anonymous(T1 x, T2 y){
    auto xpt = x + y;
    return xpt;
}
```

c++ 11 提供了decltype来解决这个问题 decltype被称作类型说明符，它的作用是选择并返回操作数的数据类型。


工作原理 decltype 并不会实际计算表达式的值，编译器分析表达式并得到它的类型。

#### [4.1 decltype + 变量](#)
decltype(var)完美保留了变量的类型。

1. decltype : 获取表达式的类型。在编译时推导出一个表达式的类型，并且**不会计算表达式的值** 。

```cpp
int age = 25;
const float score = 25;
const float & reference = score;

decltype(age) y; //将y 设置位 x的类型 即  int
decltype(score) t1; // const float 类型
decltype(score) t2; // const float & 类型

int scroes[] = {56,75,48,58,55};

decltype(scroes) numbers; //int 数组 类型 

std::cout <<  sizeof(numbers) << std::endl; //20
```
2. **两层括号变引用**
```cpp
int x = 5;
decltype ((x)) t = x; // int &
```
3. **重点:** **对指针的解引用会返回引用类型**
```cpp
const int val{15};
const int * const pt = &val; //指向常量的指针常量
decltype(*pt) dcl_p; //const int & 类型 这里会报错 因为引用必须初始化！
```

#### [4.2 decltype + 表达式](#)
当使用decltype(expr)的形式时，decltype会返回表达式结果对应的类型。

```cpp
int x = 0;
decltype(x) y = 1; 			// y ->  int
decltype(x+y) z = x + y; 		// z - >  int	
```

#### [4.3 decltype + 函数返回值](#)
C++中通过函数的返回值和形参列表，定义了一种名为函数类型的东西。它的作用主要是为了定义函数指针。

```cpp
int int_func();
int x = 0;
decltype(int_func()) a = 0; 					//a -> int
int& int_func_l();					
decltype(int_func_l()) b = x;					// b -> int&
int&& int_func_rr();
decltype(int_func_rr()) c = 0;					// c -> int&&
```

#### [4.4 作用于函数结果](#)
这个方法 使用auto
```cpp
template<typename T1, typename T2>
auto Anonymous(T1 x, T2 y){
    decltype(x + y) xpt = x + y;
    return xpt;
}
std::cout <<  Anonymous<float, int>(35.85, 10) << std::endl; //45.85
```

#### [4.5 decltype + 函数](#)
**decltype作用于函数的时候返回的是函数类型，而不是函数指针。** 如果你需要的函数指针而不是函数类型，你需要显示的加上 `*` 指示结果为指针！

```cpp
int add(int a, int b) {
	return a + b;
}

decltype(add) Sum; //Sum的类型是函数 int(int,int)
decltype(add) *SumPtr; //Sum的类型是 函数指针 int(*)(int,int)
```

### [5. auto和decltype](#)
auto 并不适用于所有的自动类型推导场景，在某些特殊情况下 auto 用起来非常不方便，甚至压根无法使用，所以 decltype 关键字也被引入到 C++11 中。

#### [5.1 auto的限制](#)
auto 并不是万能的，它只能用于声明变量，不能用于声明函数。
```cpp
auto k = 5.12; // double 类型
```

* 使用 auto 的时候 **必须对变量进行初始化**，这是 auto 的限制之一。
* **auto 不能在函数的参数中使用**，们在定义函数的时候只是对参数进行了声明，指明了参数的类型，但并没有给它赋值，只有在实际调用函数的时候才会给参数赋值；而 auto 要求必须对变量进行初始化，所以这是矛盾的。
* **auto 不能作用于类的非静态成员变量**（也就是没有 static 关键字修饰的成员变量）中，因为类型推断是在编译时进行的，而类的非静态成员变量是在运行时才创建的，所以类型推断无法完成。
* auto 关键字不能定义数组，比如下面的例子就是错误的
* auto 不能作用于模板参数，请看下面的例子：
```cpp
char url[] = "https://github.com/sorise";

auto str[] = url; //arr 为数组，所以不能使用 auto

auto g = url; //g 为 char *

template <typename T>
class A{
    //TODO:
};
int  main(){
    A<int> C1;
    A<auto> C2 = C1;  //错误
    return 0;
}
```

#### [5.2 对 cv 限定符的处理](#)
**cv 限定符** 是 const 和 volatile 关键字的统称, 在推导变量类型时，auto 和 decltype 对 cv 限制符的处理是不一样的。**decltype 会保留 cv 限定符，而 auto 有可能会去掉 cv 限定符**。

* const 关键字用来表示数据是只读的，也就是不能被修改；
* volatile 和 const 是相反的，它用来表示数据是可变的、易变的，目的是不让 CPU 将数据缓存到寄存器，而是从原始的内存中读取。

以下是 auto 关键字对 cv 限定符的推导规则：
* 如果表达式的类型不是指针或者引用，auto 会把 cv 限定符直接抛弃，推导成 non-const 或者 non-volatile 类型。
* 如果表达式的类型是指针或者引用，auto 将保留 cv 限定符。
```cpp
const int value = 12;

auto val = value; // int 类型
decltype(value) val_ty = value; //const int 类型

const int * const ptr  = &value;

auto pp_val = ptr; //const int *  没有保留顶层 const，但是保存了底层const
decltype(ptr) dpp_val = ptr; //const int * const 保留顶层 const

volatile double score = 95.5;

auto score_a = score; //double
decltype(score) score_b = score; // volatile double

volatile double * const ddp = &score;

auto ddp_a = ddp; //volatile double *  指针 保留了
decltype(ddp) ddp_b = ddp; //volatile double * const
```
当表达式的类型为引用时，auto 和 decltype 的推导规则也不一样；decltype 会保留引用类型，而 auto 会抛弃引用类型，直接推导出它的原始类型。请看下面的例子：
```cpp
float height = 1.75;
float & ref_height = height;
const float & ref_height_const = height;

decltype(ref_height) drh_ = ref_height; //float &
decltype(ref_height_const) drhc_ = ref_height_const; //const float &
auto drh_a = ref_height; //float
auto drh_b = ref_height_const; //float
```