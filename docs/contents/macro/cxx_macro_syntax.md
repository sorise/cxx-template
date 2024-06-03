## [C++ 宏编程](#)
> **介绍**：在C和C++中，宏（**Macro**）是由预处理器处理的一种文本替换机制，宏可以在编译之前对代码进行简单的替换和扩展。

---

- [1. 预处理器](#1-预处理器)
- [2. 预处理指令](#2-预处理指令)
- [3. 预处理器运算符](#3-预处理器运算符)
- [4. 预定义宏](#4-预定义宏)

---
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

| 预处理指令     | 最后更新时间： |2024年6月1日         |                      |                         |                 | 
|:----------|:--------|:--------|:---------------------|:------------------------|:----------------|
| #if       | #elif   | #else   | #ifdef               | #ifndef(C++23)          | #elifdef(C++23) | 
| #elifndef | #endif  | #Pragma | #error               | #warning(C++23)         | #import(MSVC专用) |
| #define   | #undef  |  #line  | import(C++20 尚未完全实现) | module(C++20 尚未完全实现) | #include(不解释)   |

> * 从 Visual Studio 2022 版本 17.1 起，C++20 标准模块（import module）已在 Microsoft C++ 编译器中完全实现。 [使用说明链接](https://learn.microsoft.com/zh-cn/cpp/cpp/modules-cpp?view=msvc-170)
> * GCC 11+ 支持部分功能。
> * Clang 8+ 支持部分功能。
> * Intel C++ 2023.1 支持部分功能。
> 
> * C++ 20 模块指令，将会单开一节详细讲解。

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

条件表达式支持 `+ - * / % >、<、==、||、&&、() !` 等运算符。

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
#### 2.3 #pragma 指令
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
预处理器支持文本宏替换。也支持函数式文本宏替换，如果定义跨行使用 **\** 链接。

* **对象式宏**（object-like macro）以替换列表替换每次出现的被定义标识符。#define 指令的版本 (1) 准确表现如此。
* **函数式宏**（function-like macro）以替换列表替换每次出现的被定义标识符，可选地接受一定量的实参，它们随即替换掉替换列表中出现的任何对应的形参。
    * 额外的实参（是谓可变实参）可用 **\_\_VA\_ARGS\_\_** 标识符访问，它会被与要被替换的标识符一起提供的实参替换。
    * 替换列表可以含有记号序列 **\_\_VA_OPT\_\_(内容(可选))** ，如果 **\_\_VA\_ARGS\_\_** 非空，那么它会被内容 ﻿替换，否则不展开成任何内容。

> `__VA_OPT__` 在C++ 20 引入，许多编译器未必支持。 
> * MSVC 19.25+ 支持，但需要设置 `/Zc:preprocessor` 选项。
> * GCC 12+ 支持

实验 `__VA_OPT__` 设置CMake。
```cmake
#CMake设置
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options("/Zc:preprocessor")
endif()
```  

| 指令                       | 说明              | 可否使用 |
|:-------------------------|:----------------|:-----|
| #define 标识符 替换列表(可选)     | （1）             | YES  |
| #define 标识符(形参) 替换列表     | （2）简单函数式宏。      |     YES |
| #define 标识符(形参,...) 替换列表 | （3）有可变数量实参的函数式宏 |YES|
| #define 标识符(...) 替换列表    | （4） 有可变数量实参的函数式宏，但无常规实参。            |YES|

```cpp
#define PROGRAM_NAME "Ponder"

#define ASSERT_EQ(a, b) assert((a) == (b))

#define PONDER_DI_VERSION_VALUE_GET(major, minor, patch) \
    ((major) * 10000 + (minor) * 100 + (patch))

auto version = PONDER_DI_VERSION_VALUE_GET(7,1,1);
std::cout << "Ponder version: " << version << std::endl;
//Ponder version: 70101

#define F(...) f(0 __VA_OPT__(,) __VA_ARGS__)
F(a, b, c) // 替换为 f(0, a, b, c)
F()        // 替换为 f(0)
 
#define G(X, ...) f(0, X __VA_OPT__(,) __VA_ARGS__)
G(a, b, c) // 替换为 f(0, a, b, c)
G(a, )     // 替换为 f(0, a)
G(a)       // 替换为 f(0, a)
 
#define SDEF(sname, ...) std::vector<int> sname __VA_OPT__(= { __VA_ARGS__ })
SDEF(bar, 1, 2); // 替换为 std::vector<int> bar = { 1, 2 };

define DEBUG_PRINT(fmt, ...) \
    fprintf(stderr, "DEBUG: " fmt __VA_OPT__(,) __VA_ARGS__)

int main() {
    int a = 5;
    DEBUG_PRINT("Value of a is %d\n", a);
    return 0;
}
```
输出打印例子
```cpp
template<typename T, typename ...Args>
void print_all(T t, Args ...args)
{
    std::cout << t << " ";
    print_all(args...);
}

void print_all(){

}

#define PRINT_ALL(...) print_all(__VA_ARGS__)
PRINT_ALL("123",1,2,3,4,5,6); //123 1 2 3 4 5 6
```
**忠告**：对于宏函数的参数，一定要括号括起来，不然很可能发送错误。
```cpp
#define MUSE_ADD_TOOLKIT_ERROE(a,b) (a - b)

#define MUSE_ADD_TOOLKIT(a,b) ((a)-(b))

MUSE_ADD_TOOLKIT_ERROE( 5 + 6, 78-9); // 5 + 6 - 78 - 9
MUSE_ADD_TOOLKIT( 5 + 6, 78-9); // (11) + (69)
```

#### 2.6 #undef 取消定义
#undef 指令取消定义标识符，即取消 #define 指令所作的标识符定义。如果标识符未关联到宏，那么忽略该指令。

```cpp
#define ABDEF 1
#undef ABDEF

#ifdef ABDEF
    #define ABDEF_VALUE 1
#else
    #define ABDEF_VALUE 0
#endif

 std::cout << "ABDEF_VALUE = " << ABDEF_VALUE << std::endl; 
//ABDEF_VALUE = 0
```

#### 2.7 #error
#error 指令在编译时发出用户指定的错误消息，然后终止编译。
```cpp
#if !defined(__cplusplus)
    #error C++ compiler required.
#endif
```

#### 2.8 #import
过去一直合并类型库中的信息。 类型库的内容将转换为 C++ 类，主要描述 COM 接口， MSVC专属。 还有 `#using` 都属于MSVC专属，不推荐使用。


#### 2.9 宏的字符串特性
如何宏定义中有字符串，它会自动连接，不必单向双引号问题，但是两个字符串之间需要存在**空格**。
下面的 PONDER_DI_VERSION_STRING_GET 和 STRING_TEST就是利用这个特性。
```cpp
#define PONDER_DI_STRINGIFY(x) #x

#define PONDER_DI_VERSION_STRING_GET(major, minor, patch) \
    PONDER_DI_STRINGIFY(major) "." PONDER_DI_STRINGIFY(minor) "." PONDER_DI_STRINGIFY(patch)

#define STRING_TEST "name" " " "is" " " "my" " " "love"

#define STRING_APPEND(str, append) str "," append

int main()
{
    auto str1 = STRING_TEST;
    auto str2 = STRING_APPEND("Hello","World");
    
    std::cout << str1 << std::endl; //name is my love
    std::cout << str2 << std::endl; //Hello,World
}
```

### [3. 预处理器运算符](#)
在 #define 指令的上下文中使用了四个预处理器特定运算符, 标准运算符主要为 #、##、defined 运算符

|运算符| 	操作                                |
|:---|:-----------------------------------|
|字符串化运算符 (#)| 	使对应的实参用双引号引起来                     |
|字符化运算符 (#@)| 	使相应的参数用单引号括起来并被视为字符（Microsoft 专用） |
|标记粘贴运算符 (##)| 	允许将用作实参的令牌连接起来形成其他令牌              |
|defined 运算符| 	简化某些宏指令中复合表达式的编写（很简单不必解释）         |

#### 3.1 \# 运算符
字符串化运算符,使对应的实参用双引号引起来。

* `#`出现于 `__VA_ARGS__` 之前时，展开后的 `__VA_ARGS__` 整体被包在引号中：

```cpp
#define TO_STRING(x) #x

TO_STRING(WONIMADE) // "TO_STRING(WONIMADE)"

#define showlist(...) puts(#__VA_ARGS__)
showlist();            // 展开成 puts("")
showlist(1, "x", int); // 展开成 puts("1, \"x\", int")


#define PONDER_DI_STRINGIFY(x) #x

#define PONDER_DI_VERSION_STRING_GET(major, minor, patch) \
    PONDER_DI_STRINGIFY(major) "." PONDER_DI_STRINGIFY(minor) "." PONDER_DI_STRINGIFY(patch)
    
PONDER_DI_VERSION_STRING_GET(1,9,1) 
// "1.9.1"
```

#### 3.2 \#@运算符
非标准，了解一下即可。
```cpp
#define makechar(x)  #@x
```

#### 3.2 \#\#运算符
任何两个相继标识符之间有 `##` 运算符，那么这两个标识符（首先未被宏展开）在运行形参替换的基础上将结果进行拼接。

```cpp
#define paster( n ) printf_s( "token" #n " = %d", token##n )

int main()
{
    int token9 = 9;
    paster(9); //printf_s( "token9 = %d", token9 );
}
```

### [4. 预定义宏](#)
预定义宏主要分为**标准预定义宏**和**MSVC、GCC、CLang、LINUX、Windows专用预定义宏**。

标准中为 C++11 和之后引入的 C++ 语言和程序库的功能特性定义了一组预处理器宏。标准有意使之成为检测这些功能特性是否存在的一种简单且可移植的方式,
[详情可以查看](https://zh.cppreference.com/w/cpp/feature_test#Attributes) 语言功能特性测试、在C++20以后支持配合__has_cpp_attribute使用。

```cpp
//使用例子，检查是否支持lambda表达式
#if !defined(__cpp_lambdas)
    #error not support lambda expression
#endif
```

#### 4.1 标准预定义宏
编译器基本都支持 ISO C99、C11、C17，具体可以查看[cppreference 预定义宏](https://zh.cppreference.com/w/c/language)。

* \_\_FILE\_\_ 展开成当前文件名，作为字符串字面量，可用 #line 指令更改
* \_\_LINE\_\_ 展开成源文件行号，整数常量，可用 #line 指令更改
* \_\_DATE\_\_ 展开成翻译日期，形式为 "Mmm dd yyyy" 的字符串。如果月中日期数小于 10 则 "dd" 的首字符为空格。月份名如同以 std::asctime() 生成
* \_\_TIME\_\_ 展开成翻译时间，形式为 "hh:mm:ss" 的字符串字面量

**__cplusplus**, MSVC中永恒为199711L，代表所用的 C++ 标准版本，展开成以下值之一。
  * 199711L(C++11 前)
  * 201103L(C++11)
  * 201402L(C++14)
  * 201703L(C++17)
  * 202002L(C++20)
  * 202302L(C++23)

**\_\_STDC_HOSTED\_\_** 展开成 1L，表示编译器是主机编译器，展开成 0L，表示编译器是库编译器。

#### 4.2 Windows和MSVC专业预定义宏
详情可以查看 [Microsoft 专用预定义宏](https://learn.microsoft.com/zh-cn/cpp/preprocessor/predefined-macros?view=msvc-170) 。

* _WIN32 展开成 1L，表示编译器是 Windows 编译器，展开成 0L，表示编译器是 UNIX 编译器。
* _WIN64 展开成 1L，表示编译器是 64 位 Windows 编译器，展开成 0L，表示编译器是 32 位 Windows 编译器。
* _MSC_VER 表示MSVC的版本号。
  * Visual C++ 2019：192x
  * Visual C++ 2017：191x
  * Visual C++ 2015：190x
  * Visual C++ 2013：180x
* _MSVC_LANG 表示当前使用的C++标准版本。
  * 202002L  指定了 `/std:c++20` 编译器选项
  * 201703L  指定了 `/std:c++17` 编译器选项
  * 201402L  指定了 `/std:c++14` 编译器选项
  * 201103L  指定了 `/std:c++11` 编译器选项

#### 4.3 Linux和GCC专业预定义宏
GCC 编译器也提供了一些预定义的宏，用于在编译过程中提供有用的信息或执行条件编译。以下是一些常见的 GCC 预定义宏：

* \_\_GNUC\_\_：表示GCC主版本号。
* \_\_GNUC_MINOR\_\_：表示GCC次版本号。
* \_\_GNUC_PATCHLEVEL\_\_：表示 GCC 编译器的修订版本号。例如，GCC 10.2.0 中，的值为 0。
* \_\_unix\_\_：在类 Unix 系统上编译时被定义。
* \_\_linux\_\_：在 Linux 系统上编译时被定义。
* \_\_APPLE\_\_：在 macOS 和 iOS 系统上编译时被定义。

#### 4.4 macOS和CLang专业预定义宏
Clang 编译器也提供了一些常用的预定义宏，用于在编译过程中提供有用的信息或执行条件编译。以下是一些常见的 Clang 预定义宏

* \_\_clang\_\_：表示正在使用 Clang 编译器。
* \_\_clang_major\_\_：表示 Clang 编译器的主版本号。
* \_\_clang_minor\_\_：表示 Clang 编译器的次版本号。
* \_\_clang_patchlevel\_\_：表示 Clang 编译器的修订版本号。

### 参考资料
* [Visual Studio 2022 C/C++ 预处理器参考](https://learn.microsoft.com/zh-cn/cpp/preprocessor/c-cpp-preprocessor-reference?view=msvc-170)
* [C/C++ 宏编程的艺术](https://bot-man-jl.github.io/articles/?post=2020/Macro-Programming-Art)
* [cppreference 预处理器](https://zh.cppreference.com/w/cpp/preprocessor/replace#.E9.A2.84.E5.AE.9A.E4.B9.89.E5.AE.8F)