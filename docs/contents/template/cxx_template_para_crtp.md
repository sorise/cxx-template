## [C++ 可变参数模板、CRTP](#)
**介绍**：C++ 11 新增可变参数模板、模板继承和CRTP技巧。

-----

* [1. 可变参数模板](#1-可变参数模板)

--- 
### [1. 可变参数模板](#)
**可变参数模板（variadic template）** 为了**解决强类型语言的严格性与灵活性的冲突而产生**，让我们能够创建这样的模板函数和模板类：即可接受可变数量的参数。
它对参数进行了高度的泛化，可表示0到任意个数、任意类型的参数。 提供了一个用 **省略号(...)** 表示的元运算符（meta-operator）

* 在C++11之前，类模板和函数模板中只能包含固定数量的模板参数，可变模板参数无疑是一个巨大的改进，但由于可变参数模板比较抽象，因此使用起来需要一定的技巧。
* 在C++11之前其实也有可变参数的概念，比如printf函数就能够接收任意多个参数，但这是函数参数的可变参数，并不是模板的可变参数。


省略号的作用，**左打右解**
* 位于参数左边： **打包** T... args
* 位于参数右边：**解包** args...

要创建可变参数模板，需要理解几个要点：
* 模板参数包（parameter pack）；
* 函数参数包
* 展开（unpack）参数包；
* 递归。
* 折叠表达式

**可变参数模板的定义方式** , 可变参数模板定义方式如下：
```c++
template<typename …Args>
返回类型 函数名(Args… args)
{
    //函数体
}
//模板类
template<typename T, typename ...Args>
class swagger{
    constexpr size_t args_count() noexcept{
        return sizeof...(Args);
    }
public:
    constexpr std::array<size_t, sizeof...(Args) + 1> args_size() noexcept{
        return {sizeof(T), sizeof(Args)...};//折叠表达式
    }
};
```

```cpp
//按照值传递
template<typename... Args>    // Args is a template parameter pack
void ShowList(Args... args) // args is a function parameter pack
{
   std::cout << sizeof...(args) << std::endl; //获取参数包中参数的个数
}

//按照引用传递
template<typename... Args>    // Args is a template parameter pack
void ShowList(const Args&... args) // args is a function parameter pack
{
   std::cout << sizeof...(args) << std::endl; //获取参数包中参数的个数
}

```

* 模板参数Args前面有省略号，代表它是一个可变模板参数，我们把带省略号的参数称为参数包，参数包里面可以包含0到 **N ( N ≥ 0 )** 个模板参数，而args则是一个函数形参参数包。
* 模板参数包Args和函数形参参数包args的名字可以任意指定，并不是说必须叫做 Args 和 args。

**现在调用ShowList函数时就可以传入任意多个参数了，并且这些参数可以是不同类型的。比如：**

```cpp
int main(int argc, char *argv[]){
    ShowList(); //0
	ShowList(1); //1
	ShowList(1, 'A'); //2
	ShowList(1, 'A', std::string("hello")); //3
    return 0;
}
```

#### 1.1 展开参数包
**可变模板参数的展开**，由两种方式
* 通过递归函数来展开参数包
* 是通过逗号表达式来展开参数包。

#### 1.2 折叠表达式
C++17引入的折叠表达式是一种强大的语言特性，它允许对模板参数包中的元素执行一系列操作， [cppreference 查看详情](https://zh.cppreference.com/w/cpp/language/fold)。

在可变参函数模板中，因为展开参数比较麻烦，如要把传递进来的这些参数加在一起，可能要写很多代码，而通过折叠表达式，只需要一行代码就可以完成。

**支持四种语法**
* ( 形参包 运算符 ... )	(1)
* ( ... 运算符 形参包 )	(2)
* ( 形参包 运算符 ... 运算符 初值 )	(3)
* ( 初值 运算符 ... 运算符 形参包 )	(4)	

> 支持运算符`+ - * / % ^ & | = < > << >> += -= *= /= %= ^= &= |= <<= >>= == != <= >= && || , .* ->*`

**使用例子**
```c++
template<typename... Args>
auto sum(Args... args) {
    return (... + args); // 将所有参数相加
}

template<typename... Args>
auto multiply(Args... args) {
    return (... * args); // 将所有参数相乘
}

template<typename... Args>
bool all_true(Args... args) {
    return (... && args); // 如果所有参数都为真，则返回true
}

template<typename... Args>
bool any_true(Args... args) {
    return (... || args); // 如果任一参数为真，则返回true
}

template<typename... Args>
void call_all(Args&&... args) {
    (... , args()); // 调用每个参数的函数
}

template<typename... Mutexes>
void lock_all(Mutexes&... mutexes) {
    std::lock(mutexes...); // 同时锁定所有互斥量
}
```

C++17的折叠表达式为处理参数包提供了一种更简洁的方式。从编译器的角度看，折叠表达式的处理可以分为以下几个步骤：

* 模式识别：编译器识别出折叠表达式的模式。这包括确定表达式是一个二元左折叠（`(args op ...)`）还是右折叠（`(... op args)`），以及所使用的操作符op。
* 递归展开：编译器将折叠表达式展开为一系列操作。这个过程类似于参数包在递归模板中的展开，但是由编译器直接内建处理，而不是通过模板实例化。例如，表达式 (a + ... + b) 可能会被展开为 a + (a1 + (a2 + ... + (aN + b)...))。
* 代码生成：一旦折叠表达式被完全展开，编译器就会为每个操作生成相应的代码。这个过程与普通的二元操作代码生成相同。
* 优化：在代码生成之后，编译器的优化阶段可能会进一步处理这些操作，如合并相似操作、消除无用代码等。
