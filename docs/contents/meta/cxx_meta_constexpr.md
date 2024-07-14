## [constexpr 元编程](#)
> **介绍**: 一种用于在C++中进行常量表达式求值、编译器求值的更好的机制。

----

### [1. constexpr](#)
在C++中，**constexpr** 是一个非常重要的关键字，它主要用于指定一个变量或函数能够在编译时被求值。
这个关键字的引入极大地增强了C++的元编程能力，允许开发者在编译期间执行某些计算，从而提高程序的运行效率并增强类型安全性。

* C++11 引入了 **constexpr** 关键字，但是对 **constexpr** 函数的限制较多，比如**不能包含循环或递归**。
* C++14 放宽了对constexpr函数的限制，**允许使用if语句、switch语句、while循环和递归**，只要它们在编译时可以确定终止。
* C++17 允许 **constexpr** 变量声明静态成员变量，并且给这些变量赋予内联属性。
* C++20 引入了概念约束（ **concepts** ），使得constexpr **函数可以更灵活地使用模板和类型检查**。

**使用constexpr的好处**
* **性能优化**：由于constexpr变量和函数的值在编译时就被计算出来，所以可以避免运行时不必要的计算，提高程序的执行效率。
* **类型安全**：constexpr函数可以用于类型安全的静态断言，确保代码在编译时就满足某些条件。
* **代码清晰**：使用constexpr可以明确指出哪些计算是在编译时完成的，有助于代码的可读性和维护性。

```c++
constexpr int max_size = 100; // 编译时确定的最大尺寸

template<int A, int B>
struct Add {
    constexpr static const int value = A + B;
};

template<size_t A, size_t B>
constexpr size_t Sub(){
    return A - B;
}

int main()
{
    static_assert(Add<2,3>::value == 5); // 在编译时验证add函数的正确性
    static_assert(Sub<8,3>() == 5); // 在编译时验证add函数的正确性
    return 0;
}
```

#### [1.1 编译期间if语句](#)
以 **if constexpr** 打头的 if 语句被称为 Constexpr If。
Constexpr If 是C++17所引入的新的语法特性。它为C++语言提供了在编译期处理条件分歧的功能。

通过语法if constexpr(...)，编译器使用编译时表达式在编译时决定是否使用then部分或者else部分。
如果任一部分被抛弃，那部分代码就不会生成。但是，这不意味着被抛弃的部分完全被忽略了。
它将像未使用模板的代码一样进行检查。 比如：
```c++
#include <string>
template <typename T>
std::string asString(T x)
{
    if constexpr(std::is_same_v<T, std::string>) {
        return x; // statement invalid, if no conversion to string
    }
    else if constexpr(std::is_arithmetic_v<T>) {
        return std::to_string(x); // statement invalid, if x is not numeric
    }
    else {
        return std::string(x); // statement invalid, if no conversion to string
    }
}
```
这里我们用到了编译器if的特性。它在编译时决定我们是只返回一个字符串，还是说要调std::to_string()把数组转称字符串，
又或者把传进来的参数转成std::string。因为无效的调用会被抛弃，下面的代码都可以编译：
```c++
#include "ifcomptime.hpp"
#include <iostream>

int main()
{
    std::cout << asString(42) << '\n';
    std::cout << asString(std::string("hello")) << '\n';
    std::cout << asString("hello") << '\n';
}
```

**运行期 if 语句**
```c++
if (condition) {
  statement;
} else if (condition) {
  statement;
} else {
  statement;
}
```
**预处理 if 语句**
```c++
#if condition
  statement;
#elif condition
  statement;
#else
  statement;
#endif
```
**编译期 if 语句**
```c++
if constexpr (condition) {
  statement;
} else if constexpr (condition) {
  statement;
} else {
  statement;
}
```


* 与运行期 if 语句有所不同，编译期 if 语句中的判断条件必须是**编译期常量**。
* 与预处理期 if 语句有所不同，编译期 if 语句中**被丢弃的分支仍然需要进行语法检查**。
* 与运行期 if 语句一样，编译期 if 语句的两个分支中任何一个分支只包含一句语句时，该分支的大括号可以省略。