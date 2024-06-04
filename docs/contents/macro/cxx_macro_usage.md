## [C++ 宏编程技法](#)
> **介绍**: 学习一些C++ 宏编程技巧，更好理解大神写的库代码。

----

### [1. 宏编程注意事项](#)
和模板元编程不一样，宏编程 **没有类型** 的概念，输入和输出都是 符号 —— 不涉及编译时的 C++ 语法，只进行编译前的 **文本替换**：
* 一个 **宏参数** 是一个任意的 **符号序列** (token sequence)，不同宏参数之间 用 **逗号分隔** 。
* 每个参数可以是 空序列，且空白字符会被忽略（例如 a + 1 和 a+1 相同）
* 在一个参数内，不能出现 **逗号** (comma) 或 不配对的 **括号** (parenthesis)（例如 `FOO(bool, std::pair<int, int>`) 被认为是 FOO() 有三个参数：`bool` 、 `std::pair<int` 、 `int>`。

如果需要把 **std::pair<int, int>** 作为一个参数，一种方法是使用 C++ 的 **类型别名 (type alias)** （例如 `using IntPair = std::pair<int, int>;`），避免 参数中出现逗号（即 `FOO(bool, IntPair)` 只有两个参数）。

```cpp
#define PP_REMOVE_PARENS(T) PP_REMOVE_PARENS_IMPL(T)
#define PP_REMOVE_PARENS_IMPL(...) __VA_ARGS__

#define FOO(A, B) int foo(A x, B y)
#define BAR(A, B) FOO(PP_REMOVE_PARENS(A), PP_REMOVE_PARENS(B))

FOO(bool, IntPair)                  // -> int foo(bool x, IntPair y)
BAR((bool), (std::pair<int, int>))  // -> int foo(bool x, std::pair<int, int> y)
```

另外，常用宏函数 代替 特殊符号，用于下文提到的 惰性求值：
```cpp
#define PP_COMMA() ,
#define PP_LPAREN() (
#define PP_RPAREN() )
#define PP_EMPTY()
```

### [2. 符号拼接](#)
在宏编程中，拼接标识符 (identifier concatenation / token pasting) 通过 `##` 将宏函数的参数 拼接成其他符号，再进一步 展开为目标结果。

**忠告**：如果一个 宏参数 用于 **拼接标识符或获取字面量**，那么它不会被展开（例如 BAR() 在拼接前不会展开为 bar）：
```cpp
#define FOO(SYMBOL) foo_ ## SYMBOL
#define BAR() bar

FOO(bar)    // -> foo_bar
FOO(BAR())  // -> foo_BAR()  先解析FOO
```
一种通用的方法是 **延迟拼接操作**（或 延迟 获取字面量 操作）：
```cpp
#define PP_CONCAT(A, B) PP_CONCAT_IMPL(A, B)
#define PP_CONCAT_IMPL(A, B) A##B

#define FOO(N) PP_CONCAT(foo_, N)

FOO(bar)    // -> foo_bar
FOO(BAR())  // -> foo_bar
```
* 在进入宏函数前，所有 **宏参数** 会先进行一次 **预扫描 (prescan)** ，完全展开 未用于 拼接标识符 或 获取字面量 的所有参数。
* 在宏函数展开时，用（预扫描展开后的）参数替换展开目标里的同名符号。
* 在宏函数展开后，替换后的文本会进行 **二次扫描 (scan twice)**，继续展开结果里出现的宏。
* 所以 **PP_CONCAT()** 先展开参数，再传递给 **PP_CONCAT_IMPL()** 进行实际拼接。

```cpp
auto PP_CONCAT(x , y) = 25; // auto xy = 25;
PP_CONCAT(x PP_COMMA() y)  // too few arguments (before prescan) 
PP_CONCAT(x, PP_COMMA())   // too many arguments (after prescan)
```
* 预扫描前，`x PP_COMMA() y` 是一个参数
* 预扫描后，`x, PP_COMMA()` 是三个参数

#### 2.1 自增自减
借助 PP_CONCAT()，我们可以实现 非负整数增减（即 `INC(N) = N + 1 / DEC(N) = N - 1`）：

`PP_INC(N)/PP_DEC(N)` 先展开为 `PP_INC_N/PP_DEC_N`，再经过 二次扫描 展开为对应数值 `N + 1/N - 1` 的符号, **但上述操作有上限，若超出则无法继续展开** 。
```cpp
#define PP_INC(N) PP_CONCAT(PP_INC_, N)
#define PP_INC_0 1
#define PP_INC_1 2
// ...
#define PP_INC_254 255
#define PP_INC_255 256

#define PP_DEC(N) PP_CONCAT(PP_DEC_, N)
#define PP_DEC_256 255
#define PP_DEC_255 254
// ...
#define PP_DEC_2 1
#define PP_DEC_1 0

PP_INC(1)    // -> 2
PP_DEC(2)    // -> 1
PP_INC(256)  // -> PP_INC_256 (overflow)
PP_DEC(0)    // -> PP_DEC_0  (underflow)
```

#### 2.2 逻辑运算
借助 PP_CONCAT()，我们可以实现 布尔类型（0 和 1）的 逻辑运算（与/或/非/异或/同或）, 但上述操作不支持 **非负整数** 的通用逻辑运算（仅支持 0 和 1）。
```cpp
#define PP_NOT(N) PP_CONCAT(PP_NOT_, N)
#define PP_NOT_0 1
#define PP_NOT_1 0

#define PP_AND(A, B) PP_CONCAT(PP_AND_, PP_CONCAT(A, B))
#define PP_AND_00 0
#define PP_AND_01 0
#define PP_AND_10 0
#define PP_AND_11 1

PP_AND(PP_NOT(0), 1)  // -> 1

//仅仅支持 01
PP_AND(PP_NOT(2), 0)  // -> PP_AND_PP_NOT_20
```

#### 2.3 条件选择
借助 **PP_CONCAT()** 和 **PP_BOOL()**，我们可以实现通用的 条件选择 表达式`（PRED ? THEN : ELSE，其中 PRED 可以是 任意非负整数）`




### [3. 变长参数](#)
从C++ 11开始，宏函数支持了变长参数 `...`，接受任意个 **宏参数**（用逗号分隔）：

* 传入的变长参数可以用 `__VA_ARGS__` 获取（也可以通过 `#__VA_ARGS__` 获取 逗号+空格分隔 的参数字面量）
* 另外，允许传递 空参数，即 `__VA_ARGS__` 替换为空

* 对于空参数，展开时需要处理 多余逗号 的问题：

```cpp
#define log(format, ...) printf("LOG: " format, __VA_ARGS__)

log("%d%f", 1, .2);    // -> printf("LOG: %d%f", 1, .2);
log("hello world");    // -> printf("LOG: hello world", ); error 编译错误
log("hello world", );  // -> printf("LOG: hello world", ); error 编译错误
```
下文将借助 长度判空 和 遍历访问，实现 `__VA_OPT__(,)` 的功能。

#### 3.1 下标访问
借助 PP_CONCAT()，我们可以通过 下标访问 变长参数的 特定元素：
```cpp
#define PP_GET_N(N, ...) PP_CONCAT(PP_GET_N_, N)(__VA_ARGS__)
#define PP_GET_N_0(_0, ...) _0
#define PP_GET_N_1(_0, _1, ...) _1
#define PP_GET_N_2(_0, _1, _2, ...) _2
// ...
#define PP_GET_N_8(_0, _1, _2, _3, _4, _5, _6, _7, _8, ...) _8

PP_GET_N(0, foo, bar)  // -> foo
PP_GET_N(1, foo, bar)  // -> bar
```
* `PP_GET_N()` 的参数分为两部分：下标 N 和 变长参数 ...
* 先通过 `PP_CONCAT()` 选择下标 I（从 0 开始）对应的 `PP_GET_N_I` 符号
* `PP_GET_N_I()` 接受至少 I + 1 个参数（其余的参数是变长参数），并返回第 I + 1 个参数（其余的变长参数直接丢弃）


借助 PP_REMOVE_PARENS()，我们还可以通过 下标访问 元组 的特定元素：
```cpp
#define PP_REMOVE_PARENS(T) PP_REMOVE_PARENS_IMPL T
#define PP_REMOVE_PARENS_IMPL(...) __VA_ARGS__

#define PP_GET_TUPLE(N, T) PP_GET_N(N, PP_REMOVE_PARENS(T))

PP_GET_TUPLE(0, (foo, bar))  // -> foo
PP_GET_TUPLE(1, (foo, bar))  // -> bar
```

#### 3.2

### 参考资料
- [C/C++ 宏编程的艺术](https://bot-man-jl.github.io/articles/?post=2020/Macro-Programming-Art)
