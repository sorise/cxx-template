## [C++ 元编程概念](#)
> **介绍**: 模板元编程产生的源程序是在编译期间执行的程序，可以生成在运行期间执行的程序，可以在编译期执行两种计算：数值计算和类型计算。

----

- [1. C++ 元编程](#1-c-元编程)
- [3. Type Traits](#3-type-traits)
- [3. Type Traits 应用](#3-type-traits-应用)
- [4. 概念约束](#4-概念约束)

---
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
还有许多工具类，如：[tuple_element](https://zh.cppreference.com/w/cpp/utility/tuple_element) 在其他头文件中定义。

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
  * [std::integral_constant、bool_constant、true_type、false_type](https://zh.cppreference.com/w/cpp/types/integral_constant)

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

//元祖元素类型
static_assert(std::is_same_v<
        std::tuple_element<1, std::tuple<int, double,std::string>>::type,
        double>);
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

### [3. Type Traits 应用](#)
很明显主要作用是达成Java、C#等语言的 **反射** 功能。
#### [3.1 类型内省](#) 
在计算机科学中，内省是指程序在运行时检查对象的类型或属性的一种能力。在C++中类型萃取过程也可以视作内省，这个萃取过程只是在编译时查询与类型相关的特征。

一个类型本身可以由多个部分组成，例如数组类型int[5]由两部分组成：int类型与常量5构成；容器类型vector<int>由模板类vector与类型int构成；函数类型int(int)由两个int类型构成。

通过使用模板类与特化方式，可以解构一个类型的各个组成部分。

通过使用模板类与特化方式，可以解构一个类型的各个组成部分。考虑最基本的一维数组类型，如何获得数组的长度信息？
```c++
template<typename T> struct array_size;
template<typename E, size_t N>
struct array_size<E[N]> {
    using element_type = E;
    static constexpr size_t size = N;
};

static_assert(std::is_same_v<array_size<int[5]>::element_type, int>);
static_assert(array_size<int[5]>::size == 5);
```

考虑稍微复杂的场景，如何获得函数类型的各个组成部分？函数类型由一个返回类型与多个输入类型组成。
```c++
template<typename F> struct function_traits;
template<typename R, typename... Args>
struct function_traits<R(Args...)> {
    using args_type = std::tuple<Args...>;
    using return_type = R;
    static constexpr size_t args_size = sizeof...(Args);
    template<size_t Idx> using arg = std::tuple_element_t<Idx, std::tuple<Args...>>;
};

using F = void(int,float,std::vector<float>);
static_assert(std::is_same_v<function_traits<F>::return_type, void>);
//返回值
static_assert(function_traits<F>::args_size == 3);
//参数个数
static_assert(std::is_same_v<function_traits<F>::arg<2>, std::vector<float>>);
//参数类型
```

#### [3.2 标签分发](#)
标签常常是一个空类，辅助类true_type和false_type类型可视作标签。

关键在于将标签作用于重载函数中，根据不同的标签决议出不同的函数版本。

```c++
template<typename T>
bool number_equal_impl(T a, T b, std::true_type tp)
{
    return ::fabs(a - b) < std::numeric_limits<T>::epsilon();
}

template<typename T>
bool number_equal_impl(T a, T b, std::false_type fp)
{
    return a == b;
}

template<typename T>
bool number_equal(T a, T b)
{
    return number_equal_impl(a, b, std::is_floating_point<T>{});
}
```
还可以使用constexpr来优化为一个函数
```c++
template<typename T>
auto number_equal_test(T a, T b) -> std::enable_if_t<std::is_arithmetic_v<T>, bool>{
    if constexpr (std::is_integral_v<T>()){
        return a == b;
    }else{
        return ::fabs(a - b) < std::numeric_limits<T>::epsilon();
    }
}
```

### [4. 概念约束](#)
有时候我们写的模板可能并不适用于所有的类型，或者需要参数具有某些特性，比如一定要有默认构造函数、一定需要const修饰，这个时候我们需要给模板参数增加一些限制。

#### [4.1 C++ 20之前的约束](#)
使用静态断言 **static_assert** 的来限制模板的类型户或者特性：

```cpp
//确保模板类的入参类型为integer类
template <typename T>
T add_2(T t) {
  static_assert(std::is_integral_v<T>, "Type T should be integer");
  return t+1;
}
```
**std::enable**

`std::enable_if` 顾名思义，满足条件时类型有效。作为选择类型的小工具，其广泛的应用在 C++ 的模板元编程中。它的定义也非常的简单：

```cpp
// STRUCT TEMPLATE enable_if
template <bool _Test, class _Ty = void>
struct enable_if {}; // no member "type" when !_Test

template <class _Ty>
struct enable_if<true, _Ty> { // type is _Ty for _Test
    using type = _Ty;
};
```
由此可知，只有当第一个模板参数`_Test`为 `true` 时，`type` 才有定义（`type`即第二个模板参数`_Ty`）；否则使用 `type` 会产生编译错误，且默认模板参数可以让你不必指定类型。

**enable_if_t** 是一个别名，其定义如下：

```cpp
template <bool _Test, class _Ty = void>
using enable_if_t = typename enable_if<_Test, _Ty>::type;
```

#### [4.2 C++ 20 require、concept](#)
在C++中，概念（Concepts）和requires关键字是C++20引入的重要特性，它们使得模板元编程更加直观和强大，能够更好地表达意图和约束，[官方文档](https://zh.cppreference.com/w/cpp/language/constraints) 。

#### [4.3 requires关键字](#)
requires 是 C++20 中引入的一个新关键字，用于在函数模板或类模板中声明所需的一组语义要求，它可以用来限制模板参数，类似于 typename 和 class 关键字。

requires关键字常与type_traits头文件下类型检查函数匹配使用，当requires后的表达式值为true时满足requires条件，代表由其修饰的函数/类的模板参数合法，可以正常使用

requires 关键字可以用于以下两种情况：
* 在函数模板或成员函数中，使用 requires 关键字限制函数模板或成员函数的参数或返回值必须满足一定的语义要求。例如：
  * 在这个例子中，使用 requires 关键字限制函数模板参数 T 必须是整数类型。
```c++
template <typename T>
    void print(T t) requires std::is_integral_v<T> {
    std::cout << t << std::endl;
}
```
* 在类模板或成员类中，使用 requires 关键字限制类模板或成员类必须满足一定的语义要求。例如：
  * 在这个例子中，使用 requires 关键字限制类模板参数 T 必须是整数类型。
```c++
template <typename T>
requires std::is_integral_v<T>
class IntContainer {
public:
  IntContainer(T t) : value_{t} {}
private:
  T value_;
};
```
**注意**:
* 需要注意的是，requires 关键字仅能用于函数模板和类模板中，不能用于非模板函数和非模板类
* 此外，requires 关键字的语义要求必须在编译时可验证，否则将引发编译时错误。

除此之外，requires关键字也可以用于类型转换前的检查(假如函数内需要)：
```c++
template <typename T>
int64_t unpack(T v) requires std::is_integral<T>::value
{
    return static_cast<int64_t>(v);
}
```

#### [4.4 概念 Concepts](#)
C++20 引入了 concept 以在编译期检查模板实参是否满足指定的约束.

> 概念是一种语法和语义机制，用于在模板中指定类型应该满足的条件或行为。 这使得编译器能够在编译时验证类型是否符合预期的行为模式，从而提高了代码的可读性和健壮性。
> 概念可以被视为类型的一种“协议”或“契约”。

**语法**
```
template < 模板形参列表 >
concept 概念名 = 约束表达式;
```
**定义概念**：
```c++
template <typename T>
concept integral = std::is_integral<T>::value;

```
上面的代码定义了一个名为isChildOf的概念，要求两个类型T和U，T必须继承自U。

**使用概念**：以下展示四种写法
```c++
template <integral T>
T inc(T& a) { 
    return ++a; 
}

integral auto inc(integral auto a) { return ++a; }

//使用这个
template <typename T>
T inc(T a) requires integral<T> { return ++a; }

template <typename T>
requires integral<T> 
T inc(T a) { return ++a; }
```
如果多参数
```c++
template<typename T, typename U>
concept isChildOf = std::is_base_of<U, T>::value;//类型约束, T必须继承自U

template<typename F, typename S>
requires isChildOf<F, S>
auto add_one(S* x) -> F* {
    return static_cast<F*>(x);
}

// 概念 "Hashable" 的声明可以被符合以下条件的任意类型 T 满足：
// 对于 T 类型的值 a，表达式 std::hash<T>{}(a) 可以编译并且它的结果可以转换到 std::size_t
template<typename T>
concept Hashable = requires(T a)
{
    { std::hash<T>{}(a) } -> std::convertible_to<std::size_t>;
};

template<typename T>
concept RevIterator = Decrementable<T> && requires(T t) { *t; };
```
