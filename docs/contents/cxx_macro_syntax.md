## [C++ 宏编程](#)
> **介绍**：在C和C++中，宏（**Macro**）是由预处理器处理的一种文本替换机制，宏可以在编译之前对代码进行简单的替换和扩展。


### [1. 预处理器](#)
预处理器是将源文件的文本作为翻译的第一阶段操作的**文本处理器**，在将 C 和 C++ 文件传递到编译器之前，预处理器将对这些文件执行预先操作。
可以使用预处理器有**条件地编译代码**、**插入文件**、指定编译时错误消息以及将计算机特定规则应用于代码节。

主要通过 **#define** 指令定义,使用 **#undef** 指令来取消定义，分为**对象宏**和**函数宏**两种，还有一系列的判断指令如 **#ifdef** 、**#ifndef**、**#if**、**#elif**、**#else**、**#endif**等。习惯上使用**大写字母** + **下划线:_** 来表示宏定义。

宏可以分为三类内容：
* 预处理指令
* 预处理器运算符
* 预定义宏

一些宏在不同的OS平台下，存在一些差异，如：**__cplusplus** 在MSVC中永恒为199711L。但是在其他平台，值为对应C++版本的值。
在MSVC中，定义了 **_MSC_VER** 表示MSVC的版本号。**_MSVC_LANG** 表示当前使用的C++标准版本。除非特殊情况，一般都推荐使用
ISO C++标准宏

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

### [2. 宏定义](#)















### 参考资料
* [Visual Studio 2022 C/C++ 预处理器参考](https://learn.microsoft.com/zh-cn/cpp/preprocessor/c-cpp-preprocessor-reference?view=msvc-170)