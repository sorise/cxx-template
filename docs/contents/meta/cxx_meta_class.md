## [C++ 类模板](#)
> **介绍**: C++ 类模板，也是产生类的模具，通过给定的模板参数，生成具体的类，也就是实例化一个特定的类，这个概念听起来跟函数模板差不多。

----

### [1. 类模板概念](#)
模板是创建泛型类或函数的蓝图或公式。库容器，比如迭代器和算法，都是泛型编程的例子，它们都使用了模板的概念。 定义一个类模板要使用到 **template** 关键字！

定义一个类模板，如下所示：
```cpp
#include <iostream>

template<typename T>
class MyClass {
public:
    MyClass(T value) : data(value) {}
    void display() const {
        std::cout << data << std::endl;
    }
private:
    T data;
};

int main() {
    MyClass<int> intObj(42);        // 使用 int 类型实例化模板
    intObj.display();               // 输出：42

    MyClass<std::string> strObj("Hello, World!"); // 使用 std::string 类型实例化模板
    strObj.display();                             // 输出：Hello, World!

    return 0;
}
```

#### 1.1 typename和class
在 C++ Template 中很多地方都用到了 typename 与 class 这两个关键字，而且好像可以替换, class 用于定义类，在模板引入 c++ 后，最初定义模板的方法为：

```cpp
template<class T> //......
```
这里 class 关键字表明T是一个类型，后来为了避免 class 在这两个地方的使用可能给人带来混淆，所以引入了 typename 这个关键字，它的作用同 class 一样表明后面的符号为一个类型，这样在定义模板的时候就可以使用下面的方式了：

```cpp
template<typename T>......
```
在模板定义语法中关键字 class 与 typename 的作用完全一样。typename 难道仅仅在模板定义中起作用吗？其实不是这样，typename 另外一个作用为：使用嵌套依赖类型(nested depended name)，如下所示：

```cpp
class TArray
{
public:
typedef int LengthType;
//.....
}

template<class T>
void MyMethod( T myarr )
{
typedef typename T::LengthType LengthType;
LengthType length = myarr.GetLength;
}
```
这个时候 typename 的作用就是告诉 c++ 编译器，typename 后面的字符串为一个类型名称，而不是
成员函数或者成员变量，这个时候如果前面没有 typename，编译器没有任何办法知道 
T::LengthType 是一个类型还是一个成员名称(静态数据成员或者静态函数)，所以编译不能够通过。

#### 1.2 typename
typename 关键字在 C++ 模板编程中起着重要作用，理解和正确使用 typename 关键字，可以帮助你写出更清晰和更强大的模板代码，它的主要用途包括：

* 声明模板参数类型（与 class 等价）。
* 指示嵌套依赖类型。
* 在类型别名中使用。
* 在模板模板参数中使用。

typename 关键字也可以在类型别名中使用，这在模板编程中非常有用：
```cpp
template<typename T>
class MyClass {
public:
    using Type = typename T::InnerType; // 定义类型别名 Type 为 T::InnerType
};
```

**在模板模板参数中使用** 当定义模板的模板参数时，typename 也可以使用：
```cpp
template<template<typename> class Container, typename T>
class MyClass {
    Container<T> cont; // 使用模板模板参数 Container
};
```

#### 1.3 成员函数
模板类的方法定义部分和模板函数一样，也需要模板类型说明，同时类名限定需要改成 class_Name<...>, 成员函数的定义最好
在模板类中实现，在外部实现需要再次声明模板参数，如下所示：

```cpp
template<typename T>
class Stack
{
private:
    T *container; //容器指针
    int len;
    int capacity;
public:
    Stack(/* args */);
    Stack(const int& capty);
    void push(T const&);  // 入栈
    void pop();           // 出栈
    T top() const;        // 返回栈顶元素
    bool empty() const;   // 如果为空则返回真。
    ~Stack();
};

//外部声明
template<typename T>
Stack<T>::Stack(const int& capty){
    capacity = capty;
    container = new T[capacity];
    len = 0;
}

//成员方法
template<typename T>
bool Stack<T>::empty() const{
    return len == 0;
}
```

#### 1.4 模板类的位置
模板类不是具体的类的定义，他是一组告诉编译器如何生成类的指令模板！ 所以对于模板类，应该整体放到 .h文件中！而不需要 分开防止到 .h和cpp文件中！

编译器何时利用模板来生成独立的类声明和独立的类方法呢？ 如下所示的时候！
```cpp
stack<int> istack;
stack<double> dstack;
```

#### 1.5 模板的多功能性
可以将用于常规类的技术用于模板类。模板类 可用作基类，也可以用作组件类，还可用作其他模板的类型参数
```cpp
template<typename T>
class LinkedStack: public Stack<T>{
/* ... */
}

//模板为类型
Array<Stack<double>, 10> myArray;
```

#### 1.6 模板默认参数
默认参数也可以是模板。

```cpp
template <typename T, typename F = less<T>>
    int compare(const T &before, const T &after, F f = F()){
    if(f(before, after)) return -1;
    if(f(after, before)) return 1;
    return 0;
}
```

#### 1.7 模板类型是指针
如果模板类型是指针，这个问题，值得思考,推荐创建另一个专门重载模板类来解决， 如果提供的T是指针那么会使用下面的第二个模板来，如果不是指针会直接使用第一个模板类！

```cpp
template<typename T> class FeedBack { /* ... */ };

template<typename T *> class FeedBack { /* ... */ };
```

### [2. 类模板实例化](#)
类模板和函数模板类似，也有隐式实例化，显式实例化，当然还支持模板特化、部分特化。

#### [2.1 隐式实例化](#)
最为推荐，也是最常用的，在使用的时候生成一个实例化类。
```cpp
Stack<double> theshys;

Array<double, 10> *ary_ptr; //不会生成具体类
ary_ptr = new Array<double, 10>({95.5,85.0,96.25,94.45,90.5}); //会生成具体类 实例化
```
#### [2.2 显式实例化](#)
使用关键字 template并指出所需要的类型来声明类，在使用前就生成实例化类。
```cpp
template class Array<animal, 10>; // 生成一个 Array<animal, 10> 类
```

#### [2.3 类模板特化](#)
等同于函数的具体化(特化)，为特定类型定义一个类, 属于特殊要求。
一般称之为全特化，所谓全特化，就是把TC这个泛化版本中的所有模板参数都用具体的类型代替，构成一个特殊的版本（全特化版本）。

显式具体化例子如下，需要借助关键字 `template <>`
```cpp
template <> 
class Array<int, 10>
{
private:
    int *container;
public:
    Array(std::initializer_list<int> list);
    Array(const Array& other);
    Array(const Array&& other);
    Array& operator=(const Array& other);
    int size() const;
    int &operator[](int idx);
    ~Array();
};

Array<int, 10>::Array(std::initializer_list<int> list){
    container = new int[10];
    for (int i = 0; i < list.size() && i < 10; i++)
    {
        container[i] = *(list.begin() + i);
    }
}

int Array<int, 10>::size() const{
    return 10;
}
/* ... */
```

#### [2.4 部分特化](#)
顾名思义，如果模板类用到了两个以上的类型参数，可以选择部分具体化。

```cpp
template<class T1, class T2> 
class keyValue{
    /* ... */
}

//部分显式具体化
template <class T1> class keyValue<T1, int>{
    /* 你懂的！ */
}
```

