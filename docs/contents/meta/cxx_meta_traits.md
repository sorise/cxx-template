## [C++ 萃取技术与策略技术](#)
> **介绍**:萃取（trait）技术和策略（policy）技术在模板与泛型编程中有着非常广泛的应用,在编译期间根据类型进行各种判断和运算
> 从而提取出一些有用信息的技术。

----

### [1. 萃取的基本概念](#)
“萃取”这个词的英文是trait，当然也有人翻译成“特征”，**萃取技术用于对模板中的各种模板参数进行管理**。
这种技术往往是通过书写一个类模板来体现，而这种类模板也称为“**trait类模板**”。

> “类型萃取”（type traits）这个概念，可以理解为一套函数库（类型萃取函数库），该函数库原本是在Boost库中，后来引入C++ 11标准中，这个库就实现了很多类型信息的提取、类型的变换等功能。

通过萃取技术编写的 **trait** 类模板，也广泛存在于C++标准库中。学习萃取技术的目的有以下两点。
* 了解标准库中许多萃取技术的实现方法。
* 灵活运用并组合这些实现方法，写出功能更强大、更优雅和实用的代码。

#### [1.1 类型萃取的基本作用](#)
**作用**：在C++中，“类型萃取”（Type Extraction）**是指从模板参数中提取、利用类型信息的过程**。类型萃取是一个非常有用的技术，尤其是在泛型编程中。通过类型萃取，我们可以编写高度灵活且重用性强的代码。
* 给定任意类型T，它可能是bool类型、int类型、string类型或者任何自定义的类型，通过type traits技术编译器可以回答一系列问题：它是否为数值类型？是否为函数对象？是不是指针？有没有构造函数？能不能通过拷贝构造？等等
* type traits技术还能对类型进行变换，比如给定任意类型T，能为这个类型添加const修饰符、添加引用或者指针等。而这一切都发生在编译时，过程没有任何运行时开销。
* **类型安全**：
  1. 在编译时确保使用正确的类型进行操作。
  2. 避免运行时错误，比如类型不匹配引起的错误。
* **元编程**：
  1. 使用类型萃取技术可以实现更高级的元编程功能，如条件类型、类型列表处理等。
* **类型灵活性**：
  1. 允许用户指定特定的类型或选择最佳类型以适应不同的场景。
  2. 例如，根据输入类型自动选择最合适的算法实现。

类型萃取的常用方法：
* std::remove_cv_t/std::remove_cv： 移除类型上的const和volatile限定符。
* std::remove_reference_t/std::remove_reference： 移除类型上的引用限定符。
* std::remove_pointer_t/std::remove_pointer： 移除类型上的指针限定符。
* std::common_type_t/std::common_type： 提供两个或多个类型之间的公共类型。
* std::enable_if_t/std::enable_if： 根据条件启用或禁用类型。
* std::conditional_t/std::conditional： 根据条件选择类型。
* std::is_same： 检查两个类型是否相同。
* std::type_identity_t/std::type_identity： 不做任何转换地返回原类型。
* std::decay_t/std::decay： 将类型衰减为非引用非数组类型，并移除cv限定符。
* std::underlying_type_t/std::underlying_type： 获取枚举类型的底层类型。
#### [1.2 std::enable_if](#)
std::enable_if是C++标准库中的一个类型特质，用于在模板元编程中实现条件编译，它允许你根据类型特征的存在与否来启用或禁用特定的模板特化。

**std::enable_if** 有两个模板参数：
* 第一个参数是一个布尔表达式，通常是 std::is_XXX 之类的类型特质。
* 第二个参数是当第一个参数为真时返回的类型，通常为void，但在某些情况下也可以是其他类型。

实际实现：主要利用类型特质和类型别名来实现。
```c++
template<bool B, class T = void>
struct enable_if {};
 
template<class T>
struct enable_if<true, T> { typedef T type; };

template< bool B, class T = void >
using enable_if_t = typename enable_if<B,T>::type;
```
std::enable_if 有多种用法，包括：
* 用作额外的函数实参（不适用于大多数运算符重载）
* 用作返回类型（不适用于构造函数与析构函数）
* 用作类模板或函数模板形参。

```c++
template<typename T>
typename std::enable_if<std::is_integral<T>::value, T>::type
safe_divide(T a, T b) {
    if (b == 0) throw std::runtime_error("Division by zero");
    return a / b;
}
```

#### [1.3 自定义trait类模板](#)
accum 函数进行求和计算，为了防止溢出，需要使用 AccumulationTraits 特质类模板。
```c++
template<typename T>
struct AccumulationTraits;

template<>
struct AccumulationTraits<char> {
    using AccT = int;
    static AccT const zero = 0;
};

template<>
struct AccumulationTraits<signed short> {
    using AccT = int;
    static AccT const zero = 0;
};

template<>
struct AccumulationTraits<unsigned short> {
    using AccT = unsigned int;
    static AccT const zero = 0;
};

template<>
struct AccumulationTraits<int> {
    using AccT = long;
    static AccT const zero = 0;
};

template<>
struct AccumulationTraits<unsigned int> {
    using AccT = unsigned long;
    static AccT const zero = 0;
};

template<>
struct AccumulationTraits<float> {
    using AccT = double;
    static constexpr double const zero = 0.0;
};

template<>
struct AccumulationTraits<double> {
    using AccT = double;
    static constexpr double const zero = 0.0;
};

template<typename T>
requires std::is_arithmetic_v<T>
T accum (T const* beg, T const* end)
{
    using ResultType = typename AccumulationTraits<T>::AccT ; // assume this actually creates a zero value
    ResultType total = AccumulationTraits<T>::zero;
    while (beg != end) {
        total += *beg;
        ++beg;
    }
    return total;
}
```
