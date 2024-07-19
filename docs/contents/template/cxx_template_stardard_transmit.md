### [按值传递、按引用传递](#)
> **介绍**: C++就提供了按值传递（call-by-value）和按引用传递（call-by-reference）两种参
数传递方式，C++11 又引入了移动语义（move semantics）。

----
- [1. 参数传递方式](#1-参数传递方式)
- [2. 按值传递](#2-按值传递)


---
### [1. 参数传递方式](#)
C++ 一共有三种参数传递方式，仅仅对已知的具体类型，决定参数的方式就已经很复杂了。在参数类型未知的模板中，就更难选择合适的传递方式了

* **按值传递:(T /T\*)** : 当按值传递参数时，原则上所有的参数都会被拷贝。
* **const 左值引用:(T const & / const T &)** : 参数引用了被传递的对象，并且参数不能被更改。
* **左值引用:(T &)** : 参数引用了被传递的对象，但是参数可以被更改。
* **右值引用 (X &&)** : 参数通过移动语义引用了被传递的对象，并且参数值可以被更改或者被“窃取”。

> 在C++中，const int& 和 int const & 实际上是等价的，都表示一个指向常量整数的引用

### [2. 按值传递](#)
当按值传递参数时，原则上所有的参数都会被拷贝。因此每一个参数都会是被传递实参的一
份拷贝。对于 class 的对象，参数会通过 class 的拷贝构造函数来做初始化。

> 调用拷贝构造函数的成本可能很高。但是有多种方法可以避免按值传递的高昂成本：事实上
编译器可以通过移动语义（move semantics）来优化掉对象的拷贝，这样即使是对复杂类型
的拷贝，其成本也不会很高

```c++
template<typename T>
void example(T x) {
    //todo
}
```
当将该函数模板用于 int 类型参数时，实例化后的代码是：
```c++
void example(int x) {
    //todo
}
```
**当传递的参数是复合类型时，一般情况下，编译器会调用复合类型的拷贝构造函数来初始化参数**。
#### [2.1 不调用拷贝构造函数](#)
是并不是所有的情况都会调用拷贝构造函数：

**C++ 17 引入了一个规则，要求在满足一定的条件下避免对象的复制**，这在以前是可选的。C++17中一个叫做强制复制省略的技术就是基于这个观点的。
至少包括以下两项内容：
1. **返回值优化(RVO)** : 即通过将返回值所占空间的分配地点从被调用端转移至调用端的手段来避免拷贝操作。返回值优化包括具名返回值优化（NRVO）与无名返回值优化（URVO），两者的区别在于返回值是具名的局部变量还是无名的临时对象。
2. **右值拷贝优化(不同于调用移动构造函数)** :当某一个类类型的临时对象被拷贝赋予同一类型的另一个对象时，通过直接利用该临时对象的方法来避免拷贝操作。

```c++
struct test_class{
    friend std::ostream& operator<<(std::ostream& os, const test_class& v);

    explicit test_class(const int &v):i(v){

    }

    test_class(const test_class& cls): i(cls.i){
        std::cout << "copy" << std::endl;
    }

    test_class(test_class&& cls) noexcept {
        this->i = cls.i;
        cls.i = 0;
        std::cout << "move" << std::endl;
    }
private:
    int i;
};

std::ostream &operator<<(std::ostream &os, const test_class &v) {
    os << v.i;
    return os;
}

test_class return_test_class(){
    return test_class(0);
};

//调用拷贝构造函数
std::string s = "hi";
example(s);

//被传递的参数是纯右值（prvalue，pure right value，临时对象或者某个函数的返回值
//此时编译器会优化参数传递，使得拷贝构造函数不会被调用。
example(return_test_class());
example(test_class(0));

///调用移动构造函数
test_class s1(2);
example(std::move(s1));
```

#### [2.2 按值传递会导致类型退化(decay)](#)
关于按值传递，还有一个必须被讲到的特性：当按值传递参数时，参数类型会退化（decay）

* 裸数组会退化成指针： `int a[10]` 会退化成 `int*`
* const 和 volatile 等限制符会被删除（就像用一个值去初始 化一个用 auto 声明的对象那样) `const int` 会退化成 `int`

```c++
#include <iostream>
#include <concepts>
template<typename T>
void decay_test(T x) {
    if constexpr (std::is_const_v<T>){
        std::cout << "have const." << std::endl;
    }
    if constexpr (std::is_same_v<T, int>){
        std::cout << "int" << std::endl;
    }else if constexpr (std::is_same_v<T, std::string>){
        std::cout << "string" << std::endl;
    } else if constexpr (std::is_same_v<T, int *>){
        std::cout << "int *" << std::endl;
    }
}

int main()
{
    const int v = 0;
    int vs[] = {1,2,3,4,5,6};
    const std::string s = "hello";

    decay_test(v); //int
    decay_test(vs);//int *
    decay_test(s); //string

    return 0;
}
```
这一行为继承自 C 语言，既有优点也有缺点。通常它会简化对被传递字符串常量的处理,
但是缺点是在模板内部无法区分被传递的是一个对象的指针还是一个存储一组对象的数组。

### [3. 按引用传递](#)
按引用传递不会拷贝对象（因为形参将引用被传递的实参）,按引用传递时参数类型也不会退化（decay）。

#### [3.1 按 const 引用传](#)
为了避免（不必要的）拷贝，在传递非临时对象作为参数时，可以使用 const 引用传递:

const 关键字的位置在语法上可以灵活放置，但通常有以下两种常见写法：
* `const int& ref`: 这种写法将 const 放在了类型前面，这种风格有时被称为 `C风格` 或 `前缀风格`。
* `int const &ref`: 这种写法将 const 放在了类型后面，这种风格有时被称为“K&R风格”（源自于Kernighan和Ritchie的C语言书籍），或者`后缀风格`。

尽管这两种写法在语义上是相同的，但在涉及更复杂的声明时，const 放在类型后面可以避免混淆。例如，考虑下面的声明：
```c++
const int *ptr;
```
这实际上声明了一个指向int类型的指针，而指针本身是const的，也就是说，你不能通过ptr来改变它所指向的内容，但是ptr自身是可以重新指向其他地方的。然而，如果使用后缀风格，意图会更加清晰：
```c++
int* const ptr;
```
这里声明的是一个const的指针，它指向int类型，这意味着ptr一旦初始化后就不能再指向其他地方，但是可以通过ptr来改变它所指向的内容。
```c++
template<typename T>
void muse_constant(const T& arg){
    std::cout << typeid(arg).name() << std::endl;
}

int main()
{
    const int v = 0;
    int vs[] = {1,2,3,4,5,6};
    const std::string s = "hello";
    int i = 0;

    muse_constant(v); //int
    muse_constant(vs);//muse_constant(const int const [6]& )
    muse_constant(s); //muse_constant(const std::string& )
    muse_constant(i);//muse_constant(const int& )
    return 0;
}
```
**为在底层实现上，按引用传递还是通过传递参数的地址实现的**，地址会被简单编码，这样可以提高从调用者向被调用者传递地址的效率。

**按引用传递参数时，其类型不会退化**（decay），**也就是说不会把裸数组转换为指针，也不会移除 const 和 volatile 等限制符。而且由于调用参数被声明为 T const &，被推断出来的模
板参数 T 的类型将不包含 const**。

#### [3.2 按非 const 引用传递](#)
如果想通过调用参数来返回变量值（比如修改被传递变量的值），就需要使用非 const 引用 （要么就使用指针）。

同样这时候也不会拷贝被传递的参数。被调用的函数模板可以直接访问被传递的参数。
```c++
template<typename T>
void muse_ref(T& arg){
    std::cout << typeid(arg).name() << std::endl;
}
```
**通常不允许将临时变量（prvalue）或者通过 std::move() 处理过的已存在的变量（xvalue）用作其参数**。
```c++
const int v = 0;
muse_ref(v); //const int

int vs[] = {1,2,3,4,5,6};
muse_ref(vs); //int [6]

const std::string s = "hello";
muse_ref(s); //const std::string

int i = 0;
muse_ref(i);  //int

muse_ref(returnString()); //不允许传递 lvalue
muse_ref(std::move(i));   //不允许传递 lvalue
```
此时如果传递的参数是 const 的，arg 的类型就有可能被推断为 const 引用，也就是说这时可以传递一个右值（rvalue）作为参数

**如果想禁止想非 const 应用传递 const 对象，有如下选择:**
```c++
template<typename T>
requires std::negation<std::bool_constant<std::is_const_v<T>>>::value
void muse_ref(T& arg)
{
    if constexpr(std::is_const_v<T>){
        std::cout << "const " << typeid(arg).name() << std::endl;
    }else{
        std::cout << typeid(arg).name() << std::endl;
    }
}
```
#### [3.3 按转发引用传递参数 Forwarding Reference](#)
使用引用调用（call-by-reference）的一个原因是可以对参数进行完美转发（perfect forward）:

```c++
template<typename T>
void passR (T&& arg) { 
    // arg declared as forwarding reference …
}
```
可以将任意类型的参数传递给转发引用，而且和往常的按引用传递一样，都不会创建被传递参数的备份：
```c++
std::string s = "hi";
passR(s); 
// OK: T deduced as std::string& (also the type of arg)

passR(std::string("hi")); 
// OK: T deduced as std::string, arg is std::string&&

passR(returnString()); 
// OK: T deduced as std::string, arg is std::string&&

passR(std::move(s));
// OK: T deduced as std::string, arg is std::string&&

int arr[4];
passR(arr); 
// OK: T deduced as int(&)[4] (also the type of arg)
```
但是，这种情况下类型推断的特殊规则可能会导致意想不到的结果：
```c++
std::string const c = "hi";
passR(c); 
//OK: T deduced as std::string const&

passR("hi"); 
//OK: T deduced as char const(&)[3] (also the type of arg)

int arr[4];
passR(arr); 
//OK: T deduced as int (&)[4] (also the type of arg
```
都可以在 passR()内部从 arg 的类型得知被传递的参数是一个右值（rvalue）还是一个 const 或者非 const 的左值（lvalue）。

这是唯一一种可以传递一个参数，并用它来区分以上三种情况的方法。

#### [3.4 使用 std::ref()和 std::cre](#)
从 **C++11** 开始，**可以让调用者自行决定向函数模板传递参数的方式**。

如果模板参数被声明成按值传递的，调用者可以使用定义在头文件\<functional\>中的 `std::ref()` 和 `std::cref()` 将参数按引用传递给函数模板。