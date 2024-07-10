## [constexpr元编程](#)
> **介绍**: 一种用于在C++中进行常量表达式求值、编译器求值的更好的机制。

----

### [1. constexpr](#)



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