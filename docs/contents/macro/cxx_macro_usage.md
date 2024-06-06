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

#### 2.3 布尔类型
为了支持更通用的 非负整数 的逻辑运算，可以先 将整数 转换成 布尔类型，而不是扩展 布尔类型 的逻辑运算, **不推荐这样做，会存在宏膨胀问题** 。

```cpp
#define PP_BOOL(N) PP_CONCAT(PP_BOOL_, N)
#define PP_BOOL_0 0
#define PP_BOOL_1 1
#define PP_BOOL_2 1
// ...

PP_AND(PP_NOT(PP_BOOL(2)), PP_BOOL(0))  // -> 0
PP_NOT(PP_BOOL(1000))                   // -> PP_NOT_PP_BOOL_1000
```
原理和 `PP_INC()/PP_DEC()` 类似（符号拼接 + 二次展开）
同理，上述操作也有上限，若超出则无法继续展开

#### 2.4 条件选择
借助 **PP_CONCAT()** 和 **PP_BOOL()**，我们可以实现通用的 条件选择 表达式`（PRED ? THEN : ELSE，其中 PRED 可以是 任意非负整数）`

```cpp
#define MACRO_IF(PRED, THEN, ELSE) PP_CONCAT(MACRO_IF_, PRED)(THEN, ELSE)

#define MACRO_IF_true(THEM, ELSE) THEM
#define MACRO_IF_1(THEM, ELSE) THEM
#define MACRO_IF_false(THEM, ELSE) ELSE
#define MACRO_IF_0(THEM, ELSE) ELSE

auto t_value = MACRO_IF(1, 159, 258); //159
auto f_value = MACRO_IF(false, 159, 258); //258
```

#### 2.4 惰性求值
需要注意 PP_IF() 的参数会在 预扫描 阶段被完全展开（例如 PP_COMMA() 会被立即展开为逗号，导致参数个数错误）：
```cpp
#define PP_COMMA_IF(N) PP_IF(N, PP_COMMA(), PP_EMPTY())

PP_COMMA_IF(1)  // -> PP_IF(1, , , ) (too many arguments after prescan)
```
常用的技巧是 **惰性求值 (lazy evaluation)**，即 条件选择先 返回宏函数，再传递参数 延迟调用：

```cpp
#define PP_COMMA_IF(N) PP_IF(N, PP_COMMA, PP_EMPTY)()

PP_COMMA_IF(0)  // (empty)
PP_COMMA_IF(1)  // -> ,
PP_COMMA_IF(2)  // -> ,
```
* P_COMMA_IF() 先借助 PP_IF() 返回 PP_COMMA 或 PP_EMPTY 符号
* PP_COMMA/PP_EMPTY 和后边的括号对 组成 PP_COMMA()/PP_EMPTY()，再继续展开为 逗号 或 空

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
#define PP_CONCAT(A, B) PP_CONCAT_IMPL(A, B)
#define PP_CONCAT_IMPL(A, B) A##B

#define PP_GET_N(N, ...) PP_CONCAT(PP_GET_N_, N)(__VA_ARGS__)

#define PP_GET_N_0(_0, ...) _0
#define PP_GET_N_1(_0, _1, ...) _1
#define PP_GET_N_2(_0, _1, _2, ...) _2
#define PP_GET_N_3(_0, _1, _2, _3, ...) _3
#define PP_GET_N_4(_0, _1, _2, _3,_4 ...) _4
#define PP_GET_N_5(_0, _1, _2, _3,_4,_5 ...) _5
#define PP_GET_N_6(_0, _1, _2, _3,_4,_5,_6 ...) _6
#define PP_GET_N_7(_0, _1, _2, _3,_4,_5,_6,_7 ...) _7
#define PP_GET_N_8(_0, _1, _2, _3,_4,_5,_6,_7,_8 ...) _8
#define PP_GET_N_9(_0, _1, _2, _3,_4,_5,_6,_7,_8,_9 ...) _9
#define PP_GET_N_10(_0, _1, _2, _3,_4,_5,_6,_7,_8,_9,_10 ...) _10
#define PP_GET_N_11(_0, _1, _2, _3,_4,_5,_6,_7,_8,_9,_10,_11 ...) _11
#define PP_GET_N_12(_0, _1, _2, _3,_4,_5,_6,_7,_8,_9,_10,_11,_12 ...) _12
#define PP_GET_N_13(_0, _1, _2, _3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13 ...) _13
#define PP_GET_N_14(_0, _1, _2, _3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14 ...) _14
#define PP_GET_N_15(_0, _1, _2, _3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15 ...) _15
#define PP_GET_N_16(_0, _1, _2, _3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16 ...) _16

PP_GET_N(0, foo, bar)  // -> foo
PP_GET_N(1, foo, bar)  // -> bar


const size_t value_idx_5 = PP_GET_N(5, 1,2,3,4,5,6); //6
const size_t value_idx_0 = PP_GET_N(0, 1,2,3,4,5,6); //1
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

#### 3.2 长度判空与计算
借助 PP_GET_N()，我们可以检查 **变长参数是否为空** 。

```cpp
#define PP_HAS_COMMA(...) PP_GET_N_16(__VA_ARGS__,1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 0, 0 )
#define PP_COMMA_V(...) ,
#define PP_IS_EMPTY(...)                                      \
  PP_AND(PP_AND(PP_NOT(PP_HAS_COMMA(__VA_ARGS__)),            \
                PP_NOT(PP_HAS_COMMA(__VA_ARGS__()))),         \
         PP_AND(PP_NOT(PP_HAS_COMMA(PP_COMMA_V __VA_ARGS__)), \
                PP_HAS_COMMA(PP_COMMA_V __VA_ARGS__())))


auto len =  PP_IS_EMPTY();          // -> 1
auto len1 = PP_IS_EMPTY(foo);      // -> 0
auto len2 = PP_IS_EMPTY(foo());     // -> 0
auto len3 = PP_IS_EMPTY(());        // -> 0
auto len4 = PP_IS_EMPTY(()foo);     // -> 0
auto len5 = PP_IS_EMPTY(PP_EMPTY);  // -> 0
auto len6 = PP_IS_EMPTY(PP_COMMA);  // -> 0
auto len7 = PP_IS_EMPTY(, );        // -> 0
auto len8 = PP_IS_EMPTY(foo, bar);  // -> 0
auto len9 = PP_IS_EMPTY(, , , );    // -> 0
```

长度计算
```cpp
#define PP_COMMA() ,
#define PP_LPAREN() (
#define PP_RPAREN() )
#define PP_EMPTY()

#define PP_BOOL(N) PP_CONCAT(PP_BOOL_, N)
#define PP_BOOL_0 0
#define PP_BOOL_1 1
#define PP_BOOL_2 1

#define PP_IF_1(THEN, ELSE) THEN
#define PP_IF_0(THEN, ELSE) ELSE
#define PP_IF(PRED, THEN, ELSE) \
    PP_CONCAT(PP_IF_, PP_BOOL(PRED))(THEN, ELSE)

#define PP_COMMA_IF(N) PP_CONCAT(PP_IF_, N)(PP_COMMA(), PP_EMPTY())
#define PP_VA_OPT_COMMA(...) PP_COMMA_IF(PP_NOT(PP_IS_EMPTY(__VA_ARGS__)))

#define PP_ARGC_COUNT_NOARGS 0
#define PP_ARGC_COUNT(...) \
    PP_IF(PP_IS_EMPTY(__VA_ARGS__), \
        PP_ARGC_COUNT_NOARGS,       \
        PP_GET_N(16, __VA_ARGS__ PP_VA_OPT_COMMA(__VA_ARGS__) \
        16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0))


auto lenv1 = PP_ARGC_COUNT();          // 存在BUG
/*
  语法提示器提醒错误：
  Too many arguments provided to function-like macro invocation macro 
 'PP_CONCAT_IMPL' defined here
  但是可以运行
 * */

auto lenv2 = PP_ARGC_COUNT(foo);       // -> 1
auto lenv3 = PP_ARGC_COUNT(foo());     // -> 1
auto lenv4 = PP_ARGC_COUNT(());        // -> 1
auto lenv5 = PP_ARGC_COUNT(()foo);     // -> 1
auto lenv6 = PP_ARGC_COUNT(PP_EMPTY);  // -> 1
auto lenv7 = PP_ARGC_COUNT(PP_COMMA);  // -> 1
auto lenv8 = PP_ARGC_COUNT(, );        // -> 2
auto lenv9 = PP_ARGC_COUNT(foo, bar);  // -> 2
auto lenv10 = PP_ARGC_COUNT(, , , );    // -> 4
```

#### 3.3 遍历访问
借助 **PP_ARGC_COUNT** 和 **PP_CONCAT** 可以完成变长参数的遍历访问，访问体就是 **D0_EACH** 。
```cpp
#define D0_EACH(arg) std::cout << arg << std::endl

#define MACRO_FOR_EACH_1(DO, x) DO(x);
#define MACRO_FOR_EACH_2(DO, x, ...) DO(x); MACRO_FOR_EACH_1(DO, __VA_ARGS__)
#define MACRO_FOR_EACH_3(DO, x, ...) DO(x); MACRO_FOR_EACH_2(DO, __VA_ARGS__)
#define MACRO_FOR_EACH_4(DO, x, ...) DO(x); MACRO_FOR_EACH_3(DO, __VA_ARGS__)
#define MACRO_FOR_EACH_5(DO, x, ...) DO(x); MACRO_FOR_EACH_4(DO, __VA_ARGS__)
#define MACRO_FOR_EACH_6(DO, x, ...) DO(x); MACRO_FOR_EACH_5(DO, __VA_ARGS__)
#define MACRO_FOR_EACH_7(DO, x, ...) DO(x); MACRO_FOR_EACH_6(DO, __VA_ARGS__)
#define MACRO_FOR_EACH_8(DO, x, ...) DO(x); MACRO_FOR_EACH_7(DO, __VA_ARGS__)
#define MACRO_FOR_EACH_9(DO, x, ...) DO(x); MACRO_FOR_EACH_8(DO, __VA_ARGS__)
#define MACRO_FOR_EACH_10(DO, x, ...) DO(x); MACRO_FOR_EACH_9(DO, __VA_ARGS__)
#define MACRO_FOR_EACH_11(DO, x, ...) DO(x); MACRO_FOR_EACH_10(DO, __VA_ARGS__)
#define MACRO_FOR_EACH_12(DO, x, ...) DO(x); MACRO_FOR_EACH_11(DO, __VA_ARGS__)
#define MACRO_FOR_EACH_13(DO, x, ...) DO(x); MACRO_FOR_EACH_12(DO, __VA_ARGS__)
#define MACRO_FOR_EACH_14(DO, x, ...) DO(x); MACRO_FOR_EACH_13(DO, __VA_ARGS__)
#define MACRO_FOR_EACH_15(DO, x, ...) DO(x); MACRO_FOR_EACH_14(DO, __VA_ARGS__)
#define MACRO_FOR_EACH_16(DO, x, ...) DO(x); MACRO_FOR_EACH_15(DO, __VA_ARGS__)

#define MACRO_FOR_EACH(DD, ...) \
    PP_CONCAT(MACRO_FOR_EACH_, PP_ARGC_COUNT(__VA_ARGS__))(DD, __VA_ARGS__)


MACRO_FOR_EACH(D0_EACH, 1, 2, 3);
/*
std::cout << 1 << std::endl;
std::cout << 2 << std::endl;
std::cout << 3 << std::endl;
 * */
MACRO_FOR_EACH(D0_EACH, "ASD", "2", 3,5);
/*
std::cout << "ASD" << std::endl;
std::cout << "2" << std::endl;
std::cout << 3 << std::endl;
std::cout << 5 << std::endl;
 * */
MACRO_FOR_EACH(D0_EACH, 1,2,3,4,5,6,7,8,9,10);
```

#### 3.4 宏遍历与元模块结合
可以实现类似于反射的效果。
```cpp
#define CLASS_AUTO_ATTR(NAME)                   \
    auto get_##NAME() const {                   \
        return _##NAME;                         \
    }                                           \
    template<typename TYPE>                     \
    void set_##NAME(const TYPE& NAME) {         \
        _##NAME = NAME;                         \
    }                                           \

class student{
public:
    MACRO_FOR_EACH(CLASS_AUTO_ATTR, age, name);
private:
    int _age;
    std::string _name;
};

//展开
/*
class student{
public:
    auto get_age() const { return _age; }
    template<typename TYPE>
    void set_age(const TYPE &age) { _age = age; };
    auto get_name() const { return _name; }
    template<typename TYPE>
    void set_name(const TYPE &name) { _name = name; };
private:
    int _age;
    std::string _name;
};
 * */
```

### [4. 递归重入](#)
因为 **自参照宏 (self referential macro)** 不会被展开 —— 在展开一个宏时，如果遇到 **当前宏** 的符号，则不会继续展开，
避免 **无限展开 (infinite expansion)** —— 所以宏 不支持 **递归/重入**。

宏展开顺序大致可以归结为：
* 第一步：首先用实参代替形参，将实参代入宏文本中
* 第二步：如果实参也是宏，则展开实参
* 第三步：最后继续处理宏替换后的宏文本，如果仍包含宏，则继续展开
> 注意：如果在第二步，实参代入宏文本后，实参之前或之后遇到#或##，实参不再展开

```cpp
#define cat(a,b) a ## b

//宏调用：cat(cat(1, 2), 3) 的展开顺序为：
//cat(cat(1, 2), 3) -->cat(1, 2) ## 3  -->cat(1, 2)3
//cat(1,2)仍是宏，但后面是##， (cat(1,2) ## 3)不再展开，结果为：cat(1, 2)3
```

```cpp
#define  cat(a,b)   a ## b
#define  xcat(x, y)   cat(x, y)

//宏调用 xcat(xcat(1, 2), 3) 的展开顺序为：
// xcat(xcat(1,2), 3) -->cat(xcat(1, 2), 3) -->cat(cat(1, 2), 3) 
// -->cat(1 ## 2, 3) --> 1 ##2 ## 3 -->123
```




### 参考资料
- [C/C++ 宏编程的艺术](https://bot-man-jl.github.io/articles/?post=2020/Macro-Programming-Art)
- [boost::preprocessor](https://github.com/boostorg/preprocessor/tree/develop) : Boost.Preprocessor 是 Boost 库的一部分，它为 C++ 提供了一组预处理宏，以增强预处理阶段的能力。这些宏可以用于生成代码、简化复杂的宏、处理变长参数列表等。