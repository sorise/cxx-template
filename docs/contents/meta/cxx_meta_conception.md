## [C++ 元编程概念](#)
> **介绍**: 模板元编程产生的源程序是在编译期间执行的程序，可以生成在运行期间执行的程序，可以在编译期执行两种计算：数值计算和类型计算。

----

### [1. C++ 元编程](#)
C++元编程（C++ Metaprogramming）是一种编程技巧，它允许在编译时解决一些问题和计算，生成或修改代码，而不是在运行时执行这些操作。

元编程的核心在于利用C++编译器的模板机制和类型系统的能力，来编写可以在编译期间执行的**代码生成代码**。

**C++元编程的特征**:
* **类型和模板作为数据**：在C++中，类型和模板可以被视为数据，这意味着你可以使用类型和模板来表示数值、布尔值、甚至是复杂的结构，然后在编译期间对这些“数据”进行操作。
* **静态计算**：元编程可以用于在编译期间执行计算，例如数学运算、条件判断、循环等，这些计算的结果被嵌入到最终的二进制代码中，无需在运行时重复执行。
* **代码生成**：元编程可以用来生成代码，比如根据模板参数的不同生成不同的代码路径，或者根据一些条件自动生成函数的重载版本。
* **类型推导和检查**：元编程可以用来推导类型或检查类型属性，例如检查类型是否具有某个成员函数，或者推导类型是整数还是浮点数。

**C++元编程的用途**:
* **性能优化**：在编译期间计算出结果可以避免运行时的计算开销，例如矩阵乘法的优化。
* **类型安全**：在编译时检查类型兼容性，避免运行时错误，例如使用 **SFINAE(Substitution Failure Is Not An Error)** 来有条件地启用模板特化。
* **代码简化**：自动处理代码的冗余部分，减少手动编码的工作量，例如使用模板元编程自动生成大量的重载函数。
* **自定义容器和算法**：构建具有特殊要求的容器或算法，例如基于元编程的智能指针。

元编程的例子,一个简单的元编程例子是使用模板元编程来实现加法。
```c++
template<int A, int B>
struct Add {
    constexpr static const int value = A + B;
};

template<size_t A, size_t B>
constexpr size_t Sub(){
    return A - B;
}

template<size_t N> constexpr size_t fibonacci =  fibonacci<N-1> + fibonacci<N-2>;
template<> constexpr size_t fibonacci<0> = 0;
template<> constexpr size_t fibonacci<1> = 1;

template<size_t N> constexpr double golden_ratio = fibonacci<N + 1> * 1.0  / fibonacci<N>;

int main()
{
    static_assert(Add<3, 5>::value == 8, "Addition failed.");
    std::cout.precision(std::numeric_limits<double>::max_digits10);
    std::cout <<std::format("value: {0}!\n", fibonacci<10>);
    std::cout <<std::format("golden_ratio: {0}!\n", golden_ratio<20>);
    //golden_ratio: 1.6180339985218033!
    std::cout <<std::format("golden_ratio: {0}!\n", golden_ratio<50>);
    //golden_ratio: 1.618033988749895!
    return 0;
}
```
在这个例中，**Add** 模板在编译时计算两个整数的和，并将结果存储在 **value** 静态成员中。
**static_assert** 用于验证编译时计算的结果是否正确。

#### [1.1 编译器能够操作的变量和类型](#)
元编程无法控制运行时的代码，它不能操作普通的变量，不能使用运行时关键字，可以采用的语法元素只能是如下集中：

* enum、static、const 用来定义编译器整数常量
* typedef, using 定义元数据
* `T`、`Args...` 声明元数据类型
* template 定义元函数
* `::` 域运算符，解析作用域。

#### [1.2 编译器 IF 例子](#)
```cpp
template<bool FLAG>
struct IF{};

template<>
struct IF<true>{
    static void Func() { std::cout << "true carry-out!\n"; };
};

template<>
struct IF<false>{
    static void Func() { std::cout << "false carry-out!\n"; };
};


int main() {
    
    IF<true>::Func();  //true carry-out!
    IF<false>::Func(); //false carry-out!

    return 0;
}
```

#### [1.3 编译器 SWITCH 例子](#)
```cpp
template<int Case>
struct SWITCH{
    static void Func() { std::cout << "default value: 0\n"; };
};

template<>
struct SWITCH<10>{
    static void Func() { std::cout << "value: 10\n"; };
};

template<>
struct SWITCH<20>{
    static void Func() { std::cout << "value: 20\n"; };
};

template<>
struct SWITCH<30>{
    static void Func() { std::cout << "value: 30\n"; };
};

int main() {
    SWITCH<10>::Func();  //value: 10
    SWITCH<20>::Func();  //value: 20
    SWITCH<30>::Func();  //value: 30
    SWITCH<110>::Func(); //value: 0

    return 0;
}
```
#### [1.4 编译器 for 循环](#)
for循环本质是利用模板递归：
```cpp
template <typename T,int N>
struct FOR{
    static inline void each(T t){
        std::cout << t[N] << std::endl;
        FOR<T, N -1>::each(t);
    }
};

template<typename T>
struct FOR<T,0>{
    static inline void each(T t){
        std::cout << t[0] << std::endl;
    }
};

//完成对 names的循环
int main() {
    std::vector<std::string> names{"remix", "gege", "kun", "2nb", "tome"};
    FOR<std::vector<std::string>, 4>::each(names);
    return 0;
}
```

### [2. Type traits](#)
类型特征/萃取（Type Traits）是C++标准库中的一部分，主要在 **\<type_traits\>** 头文件中定义，用于在编译时查询和操作类型信息。

类型特征提供了大量模板类，这些模板类可以用于检测或转换类型属性，从而在模板元编程中发挥重要作用。

**类型萃取无须侵入式修改已有类型便能查询该类型相关的特征**，对于无法修改的基本类型也能复用泛型算法。可以将 **trait** 看作
一个**小对象**，其主要目的是为其他对象或算法传递用于确定“策略”或“实现细节”的信息


类型特征可以分为几大类：
* **类型查询**：检查基础类型类别、复合类型类别，用于检查类型是否具有某种属性、受支持操作例如:
  * std::is_integral用于检查类型是否为整数类型。
  * std::is_arithmetic 检查类型是否为算术类型。
  * std::is_const 检查类型是否为 const 限定。
  * std::is_polymorphic 检查类型是否为多态类类型
  * std::rank 获取数组类型的维数属性
* **类型关系**：类型关系特征可以用于在编译时查询类型之间的关系。
  * std::is_same :检查两个类型是否相同
  * std::is_base_of :检查一个类型是是另一个类型的基类
  * std::is_virtual_base_of :检查一个类型是否是另一个类型的虚基类
* **类型转换和操作**：用于从一种类型转换到另一种类型，例如:
  * std::remove_const 用于去除类型的const限定
  * std::remove_reference 从给定类型移除引用
  * std::add_lvalue_reference用于给类型添加左值引用。
  * std::common_type用于找出两个类型的公共类型
  * std::aligned_storage用于创建对齐存储。
  * std::decay 进行等价于按值传递函数实参时进行的类型转换。
* **条件选择**：用于根据类型属性选择不同的类型，例如:
  * std::enable_if 用于在条件满足时启用模板特化
  * std::conditional 基于编译时布尔值选择一个类型或另一个
* [**其他**: 逻辑运算、成员关系、符号修饰符、数组、指针](https://zh.cppreference.com/w/cpp/meta)

> 考虑这么一个场景，给定任意类型T，它可能是bool类型、int类型、string类型或者任何自定义的类型，通过type traits技术编译器可以回答一系列问题：它是否为数值类型？是否为函数对象？是不是指针？有没有构造函数？能不能通过拷贝构造？等等。通过这些信息我们就能够提供更具针对性的实现，让编译器在众多选择中决策出最佳的实现。

#### [2.1 类型信息查询](3)
使用例子：
```c++
static_assert(std::is_integral<int>::value); //true
static_assert(std::is_class<std::string>::value); //true
static_assert(std::is_floating_point<float>::value); //true
static_assert(std::is_const_v<const int>); //true
//获取数组维数
static_assert(
        std::rank<int>{} == 0
    &&  std::rank<int[5]>{} == 1
    &&  std::rank<int[5][5]>{} == 2
    &&  std::rank<int[][5][5]>{} == 3 );
```
#### [2.2 类型关系](#)
类型关系特征可以用于在编译时查询类型之间的关系, 类型是否相同、是否是由基础关系。

```c++
static_assert(std::is_same<int, int>::value); //true
static_assert(std::is_same<typename std::decay<int[][6]>::type ,int(*)[6]>::value); //true
static_assert(std::is_base_of_v<Father<Son>, Son>); //true 基础关系
```

#### [2.3 类型转换和操作](#)
类型转换特征用于从一种类型转换到另一种类型，例如:

```c++
#include <iostream>
#include <type_traits>

struct foo
{
    void m() { std::cout << "无 cv\n"; }
    void m() const { std::cout << "const\n"; }
    void m() volatile { std::cout << "volatile\n"; }
    void m() const volatile { std::cout << "const-volatile\n"; }
};
 
int main()
{
    foo{}.m();
    std::add_const<foo>::type{}.m();
    std::add_volatile<foo>::type{}.m();
    std::add_cv<foo>::type{}.m();
    
    typename std::remove_cv<const volatile int>::type age = 0;

    static_assert(std::is_same<int, decltype(age)>::value, "age is not int"); // true
    //从给定类型移除引用
    static_assert(std::is_same<int, std::remove_reference<int&&>::type>::value, "int&& is not int");//true
    static_assert(std::is_same<int, std::remove_reference<int&>::type>::value, "int& is not int");//true
}
```
**类型操作**
```c++
class A {};
struct B
{
    std::int8_t p;
    std::int16_t q;
};
//提供成员常量 value，它等于类型 T 的对齐要求
std::cout << std::alignment_of<A>::value << ' '; 
std::cout << std::alignment_of<B>::value << ' '; 
std::cout << std::alignment_of<int>() << '\n'; // 另一种语法
std::cout << std::alignment_of_v<double> << '\n'; // c++17 另一种语法
//1 2 4 8

template<typename T, T... ints>
void print_sequence(std::integer_sequence<T, ints...> int_seq)
{
    std::cout << "size: " << int_seq.size() << ", sequence: ";
    ((std::cout << ints << ' '), ...);
    std::cout << '\n';
}

print_sequence(std::integer_sequence<unsigned, 9, 2, 5, 1, 9, 1, 6>{});
//size: 7, sequence: 9 2 5 1 9 1 6
print_sequence(std::make_integer_sequence<int, 20>());
//size: 20, sequence: 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19
```

#### [2.4 条件选择](#)
std::enable_if用于在条件满足时启用模板特化，std::conditional基于编译时布尔值选择一个类型或另一个。

是经常使用的两个元模板：
```c++
template<class T>
std::enable_if<std::is_arithmetic<T>::value, long>::type
ADD_M(T&& v, T&& v2) {
    auto t1 = static_cast<long>(v + v2);
    auto t2 = static_cast<long>(v);
    return t1 % t2;
} // #1

::printf("number: %ld\n",ADD_M(15.1, 220.2)); //10

using Type1 = std::conditional<true, int, double>::type;
using Type2 = std::conditional<false, int, double>::type;
using Type3 = std::conditional<sizeof(int) >= sizeof(double), int, double>::type;

std::cout << typeid(Type1).name() << '\n'; //int
std::cout << typeid(Type2).name() << '\n'; //double
std::cout << typeid(Type3).name() << '\n'; //double
```