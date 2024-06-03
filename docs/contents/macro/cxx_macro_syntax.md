## [C++ 宏编程](#)
> **介绍**：在C和C++中，宏（**Macro**）是由预处理器处理的一种文本替换机制，宏可以在编译之前对代码进行简单的替换和扩展。


### [1. 预处理器](#)
预处理器是将源文件的文本作为翻译的第一阶段操作的**文本处理器**，在将 C 和 C++ 文件传递到编译器之前，预处理器将对这些文件执行预先操作。
可以使用预处理器有**条件地编译代码**、**插入文件**、指定编译时错误消息以及将计算机特定规则应用于代码节。

> 宏在编译前的 **预处理(preprocess)** 阶段全部展开 —— 狭义上，编译器看不到且不处理宏代码。

主要通过 **#define** 指令定义,使用 **#undef** 指令来取消定义，分为**对象宏**和**函数宏**两种，还有一系列的判断指令如 **#ifdef** 、**#ifndef**、**#if**、**#elif**、**#else**、**#endif**等。习惯上使用**大写字母** + **下划线:_** 来表示宏定义。

宏可以分为三类内容：
* 预处理指令
* 预处理器运算符
* 预定义宏

一些宏在不同的OS平台下，存在一些差异，如：**__cplusplus** 在MSVC中永恒为199711L。但是在其他平台，值为对应C++版本的值。
在MSVC中，定义了 **_MSC_VER** 表示MSVC的版本号。**_MSVC_LANG** 表示当前使用的C++标准版本。除非特殊情况，一般都推荐使用
ISO C++标准宏。

#### 1.1 使用例子
如下的宏定义，用于判断当前编译器是否支持C++20，如果不支持则报错，并且要求GCC编译器版本为9.1以上，MSVC编译器版本为2017以上。
同时给出的`ponder::di`库的版本号、版本字符串
```cpp
#pragma once

#ifndef DI_CXX_VERSION_LIMIT
    #define DI_CXX_VERSION_LIMIT 202002L
#endif

#if defined(__GNUC__)
    #if (__cplusplus < DI_CXX_VERSION_LIMIT) || (__GNUC__ < 9) || (__GNUC_MINOR__ < 1)
        #error "ponder::di requires C++20 support GCC-9.1+"
    #endif
#elif defined(_MSC_VER)
    #if (_MSC_VER < 1900 || _MSVC_LANG < DI_CXX_VERSION_LIMIT)
        #error "ponder::di requires C++20 support MSVC-2017+"
    #endif
#elif defined(__CLANG__)
    #error "ponder::di not support clang."
#endif

//可以使用CMAKE configure_file 来获取版本信息
#define PONDER_DI_VERSION_MAJOR 1
#define PONDER_DI_VERSION_MINOR 0
#define PONDER_DI_VERSION_PATCH 0

#define PONDER_DI_STRINGIFY(x) #x

#define PONDER_DI_VERSION_STRING_GET(major, minor, patch) \
    PONDER_DI_STRINGIFY(major) "." PONDER_DI_STRINGIFY(minor) "." PONDER_DI_STRINGIFY(patch)

#define PONDER_DI_VERSION_VALUE_GET(major, minor, patch) \
    ((major) * 10000 + (minor) * 100 + (patch))

#define PONDER_DI_VERSION_STRING \
    PONDER_DI_VERSION_STRING_GET(PONDER_DI_VERSION_MAJOR, PONDER_DI_VERSION_MINOR, PONDER_DI_VERSION_PATCH)
#define PONDER_DI_VERSION \
    PONDER_DI_VERSION_VALUE_GET(PONDER_DI_VERSION_MAJOR, PONDER_DI_VERSION_MINOR, PONDER_DI_VERSION_PATCH)
```

### [2. 预处理指令](#)
源文件中的预处理指令告知预处理器应该采取特定操作，可以实现文本替换、条件编译、插入文件、定义编译时错误消息以及将计算机特定规则应用于代码节。

| 指令        |        |         |        |                 |                 | 
|:----------|:-------|:--------|:-------|:----------------|:----------------|
| #if       | #elif  | #else   | #ifdef | #ifndef(C++23)  | #elifdef(C++23) | 
| #elifndef | #endif | #Pragma | #error | #warning(C++23) | #import         |
| #define   | #undef |  #line  | ` `    | ` `             | ` `             |

#### 2.1 条件指令
预处理器支持有条件地编译源文件的某些部分。这一行为由 `#if、#else、#elif、#ifdef、#ifndef、#elifdef、#elifndef (C++23 起) 与 #endif` 指令所控制。

* **#if** 表达式		
* **#ifdef** 标识符		
* **#ifndef** 标识符		
* **#elif** 表达式		
* **#elifdef** 标识符		(C++23 起)
* **#elifndef** 标识符		(C++23 起)
* **#else**		
* **#endif**

```cpp
#if DLEVEL > 5
    #define SIGNAL  1
    #if STACKUSE == 1
        #define STACK   200
    #else
        #define STACK   100
    #endif
#else
    #define SIGNAL  0
    #if STACKUSE == 1
        #define STACK   100
    #else
        #define STACK   50
    #endif
#endif
```

条件表达式支持 `+ - * / % >、<、==、||、&&、()` 等运算符。

* (C++17 起) [**__has_include**](https://zh.cppreference.com/w/cpp/preprocessor/include) 表达式，检测某个头文件或源文件存在。
* (C++20 起) [**__has_cpp_attribute**](https://zh.cppreference.com/w/cpp/feature_test#Attributes) 表达式，检测给定属性记号是否受支持和支持的版本。

**has_include** 使用例子，判断是否支持`<optional>`头文件，如果支持则使用`<optional>`，否则使用`<experimental/optional>`。
如果都没有找到，则定义一个自定义的`optional_t`模板类。
```cpp
#if __has_include(<optional>)
    #include <optional>
    #define has_optional 1
    template<class T> using optional_t = std::optional<T>;
#elif __has_include(<experimental/optional>)
    #include <experimental/optional>
    #define has_optional -1
       template<class T> using optional_t = std::experimental::optional<T>;
#else
    #define has_optional 0
    #include <utility>
    template<class V>
    class optional_t
    {
        V v_{}; bool has_{false};
    public:
        optional_t() = default;
        optional_t(V&& v) : v_(v), has_{true} {}
        V value_or(V&& alt) const& { return has_ ? v_ : alt; }
        /*...*/
    };
#endif

#include <iostream>

int main()
{
    if (has_optional > 0)
        std::cout << "<optional> exist\n";
    else if (has_optional < 0)
        std::cout << "<experimental/optional> exist\n";
    else
        std::cout << "<optional> not exist\n";

    optional_t<int> op;
    std::cout << "op = " << op.value_or(-1) << '\n';
    op = 42;
    std::cout << "op = " << op.value_or(-1) << '\n';
}
```

**has_cpp_attribute**：要检测编译器是否支持 nodiscard 属性，可以这样做：
```cpp
#if defined(__has_cpp_attribute)
    #define HAS_CPP_ATTRIBUTE(attribute) __has_cpp_attribute(attribute)
#else
    #define HAS_CPP_ATTRIBUTE(attribute) 0
#endif

#if HAS_CPP_ATTRIBUTE(nodiscard)
    #define NODISCARD [[nodiscard]]
#else
    #define NODISCARD
#endif
```

#### 2.2 条件指令用于宏存在性判断
检查标识符是否被定义为宏名。 形如 **defined 标识符** 或 **defined (标识符)** 的一元运算符。当此 标识符 已经被定义为宏名时结果为 **1**，否则结果为 **0**。

> defined 是 define的过去式，是运算符，要和 **#define 预定义指令区分开**。

* `#ifdef` 标识符与`#if defined` 标识符实质上等价。
* `#ifndef` 标识符与`#if !defined` 标识符实质上等价。
* `#elifdef` 标识符与`#elif defined` 标识符实质上等价。
* `#elifndef` 标识符与`#elif !defined` 标识符实质上等价。

```cpp
#ifdef _WIN32
    #include <windows.h>
#else
    #error "only support windows"
#endif
//等价于
#if defined(_WIN32)
    #include <windows.h>
#else
    #error "only support windows"
#endif
```
#### 2.3 Pragma 指令与 __pragma 和 _Pragma 关键字
由 **#pragma** 指令控制实现定义行为。指令控制编译器的实现指定行为，如 **禁用编译器警告** 或 **更改对齐要求**, **忽略任何不被识别的语用**, C++99开始支持。

以 **#pragma** 开头行指定 pragma 指令。 **Microsoft 特定 __pragma 关键字** 可以在宏定义内编写 pragma 指令。
* #pragma 语用形参
* _Pragma ( 字符串字面量 )	

| 使用方式                             | 说明                                             | 是否推荐使用                |
|:---------------------------------|:-----------------------------------------------|:----------------------|
| #pragma once                     | 包含守卫，防止引入重复的头文件。                               | 非C++标准，不推荐使用。         |
| #pragma pack(show)               | 显示当前内存对齐的字节数，编辑器默认8字节对齐                        | 并非所有编译器都支持            |
| #pragma pack                     | 用于控制结构体、联合和类的成员的内存对齐方式。                        | 辩证使用                  |
| #pragma region | 这些指令用于代码折叠，方便在 IDE 中管理代码块。                     | 主要用于 Visual Studio。 |
| #pragma endregion | 这些指令用于代码折叠，方便在 IDE 中管理代码块。                     | 主要用于 Visual Studio。 |
| #pragma pack(n)                  | 设置编辑器按照n个字节对齐，n可以取值1,2,4,8,16，n有上限值，超过编译器支持无效。 | YES                   |
| #pragma pack(push)               | 将当前的对齐字节数压入栈顶，不改变对齐字节数 (栈顶里面的值为字节对齐大小)         | YES                   |
| #pragma pack(pop)                | 弹出栈顶对齐字节数，不改变对齐字节数                             | YES                   |
| #pragma pack(push,n)             | 将当前的对齐字节数压入栈顶，并按照n字节对齐                         | YES                   |
| #pragma pack(pop,n)              | 弹出栈顶并直接丢弃，按照n字节对齐                              | YES                   |
| #pragma message                  |用于在编译时向开发人员发送自定义消息，通常用于调试或输出构建信息。| MSVC特定扩展，不推荐使用        |
| #pragma warning                  |控制编译器警告的行为|                       MSVC特定扩展，不推荐使用|


**#pragma once** 用于防止头文件被多次包含。这是一种比传统的包含守卫更简洁的做法，**不推荐使用**。
其不是标准的 C++ 特性，而是编译器特定的扩展。虽然现代大多数编译器（如 GCC、Clang、MSVC 等）都支持它，但并非所有编译器都支持。
```cpp
//传统方法
#ifndef CXX_TEMPLATE_TOOLKITS_HPP
#define CXX_TEMPLATE_TOOLKITS_HPP

class toolkits {
    //...
};

#endif //CXX_TEMPLATE_TOOLKITS_HPP
```
使用 #pragma once
```cpp
#pragma once

class toolkits {
    //...
};
```

**#pragma pack** 用于控制结构体、联合和类的成员的内存对齐方式,可以指定类的对齐，然而它不能使类过对齐。

如下所示，使用 #pragma pack(push, 1) 可以将当前对齐方式压栈并设置新的对齐方式为 1 字节。
如果设置了一个编译器不支持的对齐方式(如n=32)，编译器可能会忽略该设置，并使用默认的对齐方式。
```cpp
#pragma pack(push, 1) // 将当前对齐方式压栈并设置新的对齐方式为 1 字节
struct PackedStruct {
    char c;
    int i;
};
#pragma pack(pop) // 恢复之前的对齐方式

/*
 | 1 | 2  3  4  5|  字节
 | c |     i     | 
 * */

PackedStruct s{'c', 12};
std::cout << sizeof(s) << std::endl; //5
```
设置 **n = 2** 时，结构体大小为 6 字节。
```cpp
#pragma pack(push, 2) // 将当前对齐方式压栈并设置新的对齐方式为 2 字节
struct PackedStruct {
    char c;
    int i;
};
#pragma pack(pop) // 恢复之前的对齐方式

/*
 | 1  2 |  3  4  5  6 |  字节
 |   c  |     i       | 
 * */

PackedStruct s{'c', 12};
std::cout << sizeof(s) << std::endl; //6
```

* PackedStruct1 使用默认对齐方式，通常 int 会对齐到 4 字节边界。结构体总大小为8.
* PackedStruct1 使用 1 字节对齐方式，因此结构体大小为 5 字节（1 字节用于 char a，紧接着 4 字节用于 int b，无填充）。
* PackedStruct2 使用 2 字节对齐方式，因此结构体大小为 6 字节（1 字节用于 char a，1 字节填充，4 字节用于 int b）。

#### 2.4 #line
更改预处理器中的行号，可选地更改当前文件名, 很简单。

* **#line n** 将当前的预处理器行号更改为 行号。宏 \_\_LINE\_\_ 在该点后的展开将产生 行号 加上自此遇到的实际代码行数。
* **#line n "filename"** 还将当前的预处理器文件名更改为 文件名。宏 \_\_FILE\_\_ 在该点后的展开将生成 文件名。

```cpp
int main()
{
    #line 777 "test.cc"
    assert(2+2 == 5);
}
//Assertion failed: 2+2 == 5, file test.cc, line 777
```
报错信息为 Assertion failed: 2+2 == 5, file test.cc, line 777。

#### 2.5 #define 宏定义简介
预处理器支持文本宏替换。也支持函数式文本宏替换。


### 参考资料
* [Visual Studio 2022 C/C++ 预处理器参考](https://learn.microsoft.com/zh-cn/cpp/preprocessor/c-cpp-preprocessor-reference?view=msvc-170)
* [C/C++ 宏编程的艺术](https://bot-man-jl.github.io/articles/?post=2020/Macro-Programming-Art)