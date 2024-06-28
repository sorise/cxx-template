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
模板参数Args前面有省略号，代表它是一个可变模板参数，我们把带省略号的参数称为参数包，参数包里面可以包含0到 **N ( N ≥ 0 )** 个模板参数，而args则是一个函数形参参数包。

模板参数包Args和函数形参参数包args的名字可以任意指定，并不是说必须叫做 Args 和 args。

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

#### [1.1 展开参数包](#)
**可变模板参数的展开**，由两种方式
* 通过递归函数来展开参数包
* 是通过逗号表达式来展开参数包。

**递归展开参数包**:
递归展开参数包的方式如下：
* 给函数模板增加一个模板参数，这样就可以从接收到的参数包中分离出一个参数出来。
* 在函数模板中递归调用该函数模板，调用时传入剩下的参数包。
* 如此递归下去，每次分离出参数包中的一个参数，直到参数包中的所有参数都被取出来。

比如我们要打印调用函数时传入的各个参数，那么函数模板可以这样编写：
```c++
template<typename T,typename... Args>    // Args is a template parameter pack
void ShowList(T value, Args... args) // args is a function parameter pack
{
   std::cout << value << " ";
   //std::cout << sizeof...(args) << std::endl; //获取参数包中参数的个数
   printf("\n");
   ShowList(args...); //递归调用
}
```
**这时我们面临的问题就是，如何终止函数的递归调用? 编写无参的递归终止函数**
```c++
void ShowList(){ 
  printf("\n");  
}

template<typename T,typename... Args>    // Args is a template parameter pack
void ShowList(T value, Args... args) // args is a function parameter pack
{
   std::cout << value << " ";
   ShowList(args...);
}

int main(int argc, char *argv[]){
    ShowList("cnm",78, 'A', std::string("hello")); //3
    return 0;
}
```
#### [1.2 逗号表达式来展开参数包](#)
这种方式是通过列表获取参数包中的参数。如果参数包中各个参数的类型都是整型，那么也可以把这个参数包放到列表当中初始化这个整型数组，此时参数包中参数就放到数组中了。比如：

数组可以通过列表进行初始化，比如：
```c++
int a[] = {1,2,3,4}
```
除此之外，如果参数包中各个参数的类型都是整型，那么也可以把这个参数包放到列表当中初始化这个整型数组，此时参数包中参数就放到数组中了。比如：
```c++
//展开函数
template<class ...Args>
void ShowList(Args... args)
{
    int arr[] = { args... }; //列表初始化
    //打印参数包中的各个参数
    for (auto e : arr)
    {
        cout << e << " ";
    }
    cout << endl;
}
```
这时调用ShowList函数时就可以传入多个整型参数了。比如：
```c++
int main()
{
    ShowList(1);
    ShowList(1, 2);
    ShowList(1, 2, 3);
    return 0;
}
```
但C++并不像Python这样的语言，C++规定一个容器中存储的数据类型必须是相同的，因此如果这样写的话，那么调用ShowList函数时传入的参数只能是整型的， 并且还不能传入0个参数，因为数组的大小不能为0，因此我们还需要在此基础上借助逗号表达式来展开参数包。

虽然我们不能用不同类型的参数去初始化一个整型数组，但我们可以借助逗号表达式。
* 逗号表达式会从左到右依次计算各个表达式，并且将最后一个表达式的值作为返回值进行返回。
* 将逗号表达式的最后一个表达式设置为一个整型值，确保逗号表达式返回的是一个整型值。
* 将处理参数包中参数的动作封装成一个函数，将该函数的调用作为逗号表达式的第一个表达式。

```cpp
//处理参数包中的每个参数
template<class T>
void PrintArg(const T& t)
{
    cout << t << " ";
}

//展开函数
template<class ...Args>
void ShowList(Args... args)
{
    int arr[] = { (PrintArg(args), 0)... }; //列表初始化+逗号表达式
    cout << endl;
}
```
这样一来，在执行逗号表达式时就会先调用处理函数处理对应的参数，然后再将逗号表达式中的最后一个整型值作为返回值来初始化整型数组。比如：
```cpp
//处理参数包中的每个参数
template<class T>
void PrintArg(const T& t)
{
    cout << t << " ";
}
//展开函数
template<class ...Args>
void ShowList(Args... args)
{
    int arr[] = { (PrintArg(args), 0)... }; //列表初始化+逗号表达式
    cout << endl;
}
```
我们这里要做的就是打印参数包中的各个参数，因此处理函数当中要做的就是将传入的参数进行打印即可。

可变参数的省略号需要加在逗号表达式外面，表示需要将逗号表达式展开，如果将省略号加在args的后面，那么参数包将会被展开后全
部传入PrintArg函数，代码中的 `{(PrintArg(args), 0)...}` 将会展开成 `{(PrintArg(arg1), 0), (PrintArg(arg2), 0), (PrintArg(arg3), 0), etc...}`。

实际上我们也可以不用逗号表达式，因为这里的问题就是初始化整型数组时必须用整数，那我们可以将处理函数的返回值设置为整型，然后用这个返回值去初始化整型数组也是可以的。比如：
```c++
//支持无参调用
void ShowList()
{
	cout << endl;
}
//处理函数
template<class T>
int PrintArg(const T& t)
{
	cout << t << " ";
	return 0;
}
//展开函数
template<class ...Args>
void ShowList(Args... args)
{
	int arr[] = { PrintArg(args)... }; //列表初始化
	cout << endl;
}
```

#### [1.3 折叠表达式](#)
C++17引入的折叠表达式是一种强大的语言特性，它允许对模板参数包中的元素执行一系列操作， [cppreference 查看详情](https://zh.cppreference.com/w/cpp/language/fold)。

在可变参函数模板中，因为展开参数比较麻烦，如要把传递进来的这些参数加在一起，可能要写很多代码，而通过折叠表达式，只需要一行代码就可以完成。

**支持四种语法**
* ( 形参包 运算符 ... )	(1)
* ( ... 运算符 形参包 )	(2)
* ( 形参包 运算符 ... 运算符 初值 )	(3)
* ( 初值 运算符 ... 运算符 形参包 )	(4)	

> 支持运算符`+ - * / % ^ & | = < > << >> += -= *= /= %= ^= &= |= <<= >>= == != <= >= && || , .* ->*`

折叠表达式的实例化按以下方式展开成表达式 e：

1. 一元右折叠 **(E 运算符 ...) 成为 (E1 运算符 (... 运算符 (EN-1 运算符 EN)))**
2. 一元左折叠 **(... 运算符 E) 成为 (((E1 运算符 E2) 运算符 ...) 运算符 EN)**
3. 二元右折叠 **(E 运算符 ... 运算符 I) 成为 (E1 运算符 (... 运算符 (EN−1 运算符 (EN 运算符 I))))**
4. 二元左折叠 **(I 运算符 ... 运算符 E) 成为 ((((I 运算符 E1) 运算符 E2) 运算符 ...) 运算符 EN)**

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

**模式识别**：编译器识别出折叠表达式的模式。这包括确定表达式是一个二元左折叠（`(args op ...)`）
还是右折叠（`(... op args)`），以及所使用的操作符op。

**递归展开**：编译器将折叠表达式展开为一系列操作。这个过程类似于参数包在递归模板中的展开，但是由编译器直接内建
处理，而不是通过模板实例化。例如，表达式 (a + ... + b) 可能会被展开为 `a + (a1 + (a2 + ... + (aN + b)...))`。

**代码生成**：一旦折叠表达式被完全展开，编译器就会为每个操作生成相应的代码。这个过程与普通的二元操作代码生成相同。

**优化**：在代码生成之后，编译器的优化阶段可能会进一步处理这些操作，如合并相似操作、消除无用代码等。
