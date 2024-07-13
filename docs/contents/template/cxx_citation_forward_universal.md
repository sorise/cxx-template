### [万能引用、完美转发](#)
> **介绍**：万能引用、完美转发，C++的进阶内容，是每一个需要开发自定义库的Cpper必背的技能。
----

* [1. 万能引用](#1-万能引用)
* [2. remove_reference](#2-remove_reference)
* [3. 完美转发](#3-完美转发)
* [4. std::move](#4-stdmove)

---
### [1. 万能引用](#)
C++只有两种引用，左值引用和右值引用。右值引用只能绑定到右值上，不能绑定到左值上。

万能引用(universal reference)，也称**为未定义引用**，是指既可以接收左值，又可以接收右值。

* 实参是左值，他就是左值引用（引用折叠）
* 实参是右值，他就是右值引用
> 万能引用还有另一种叫法：**引用折叠** 

万能引用既可以是左值引用，用来绑定左值；也可以是右值引用，用来绑定右值。
* 模板中的 && 不代表右值引用，而是 万能引用 ，其既能接收左值又能接收右值。
* 模板的万能引用只是提供了能够接收同时接收左值引用和右值引用的 **能力**
* 但是引用类型的唯一作用就是—— 限制了接收的类型 ，后续使用中都退化成了 **左值**
```c++
void myfunc(int&& val)    // 右值引用
{
	cout << val << endl;
	return;
}

void myfunc(int& val)    // 左值引用
{
	cout << val << endl;
	return;
}

template <typename T>    // 函数模板
void myfunc(T&& val)     // 万能引用
{
	cout << val << endl;
	return;
}
```
**注意**：**const修饰词会剥夺一个引用成为万能引用的资格**，被打回原形成为右值引用。
```c++
template <typename T>         // 函数模板
void myfunc(const T&& val)    // 右值引用
{
	cout << val << endl;
	return;
}

int i = 100; 
myfunc(i); //不可以，只能传递右值进去
// 必须是myfunc(std::move(i));
myfunc(std::move(i)); //可以
```
请注意T&&前后都不要加修饰符，不然很可能就不是万能引用，直接退化为右值引用
#### 1.1 引用折叠
声明引用的引用是非法的，但编译器却可以在模板实例化过程产生引用的引用。
```c++
int&&&& j = 1; // 不允许使用对引用的引用
```
模板实例化过程中出现这种情况就会发生引用折叠。如果任一引用为左值引用，则结果为左值引用, 若两个皆为右值引用结果为右值引用。

| |引用折叠前|	引用折叠后|
|:---|:---|:---|
|左值引用-左值引用|	T& &|	T&|
|左值引用-右值引用|	T& &&|	T&|
|右值引用-左值引用|	T&& &|	T&|
|右值引用-右值引用|	T&& &&|	T&&|
规则就是：如果任一引用为左值引用，则结果为左值引用；否则，结果为右值引用。

#### 1.2 深化理解万能引用
如下并非万能引用, 因为const会剥夺万能引用的资格，**但是又可以接受右值**。
```c++
template <typename T> void func(const T &abc) {
    //todo
}

func(25);
//（1）T的类型是int；
//（2）abc的类型是const int &
```
尝试理解：
```c++
void process(int&& i){
    std::cout << "&&i = " << i << std::endl;
}

void process(int& i){
    std::cout << "&i = " << i << std::endl;
}

void process(const int& i){
    std::cout << "ci = " << i << std::endl;
}

template<typename T>
void universal_process(T&& t){
    process(t);
}

int main()
{
    universal_process(10);
    int i = 100;
    universal_process(i);
    const int v = 1000;
    universal_process(v);
}
```
**结果如下**，原因是但是引用类型的唯一作用就是—— 限制了接收的类型 ，后续使用中都退化成了 **左值**。
所以除了v，其他都调用 **void process(int& i)** 函数。
```
&i = 10
&i = 100
ci = 1000
```
为了让左值调用**void process(int& i)** 函数，右值调用**void process(int&& i)** 函数，需要用到**完美转发**。
```c++
template<typename T>
void universal_process(T&& t){
    process(std::forward<T>(t));
}
//等价于如下操作
template<typename T>
void universal_process(T&& t){
    process(static_cast<T&&>(t));
}
```

### [3. remove_reference](#)
**std::remove_reference**, 是C++标准库中的一个类型特质（type trait），用于去除类型上的引用修饰。
```c++
namespace std {

_EXPORT_STD template <class _Ty>
struct remove_reference {
    using type                 = _Ty;
    using _Const_thru_ref_type = const _Ty;
};

template <class _Ty>
struct remove_reference<_Ty&> {
    using type                 = _Ty;
    using _Const_thru_ref_type = const _Ty&;
};

template <class _Ty>
struct remove_reference<_Ty&&> {
    using type                 = _Ty;
    using _Const_thru_ref_type = const _Ty&&;
};


template <class T>
using remove_reference_t = typename remove_reference<T>::type;
}  // namespace std
```
特化（Specialization）：std::remove_reference有三个特化版本：
* 对于非引用类型T，remove_reference<T>直接返回T。
* 对于左值引用T&，remove_reference<T&>返回T。
* 对于右值引用T&&，remove_reference<T&&>同样返回T。

假设有一个引用类型int& ref，我们可以通过std::remove_reference去除引用：
```c++
int x = 10;
int& ref = x;

// 去除ref的引用
using NonRefType = std::remove_reference<decltype(ref)>::type;
// 或者使用类型别名
using NonRefTypeAlias = std::remove_reference_t<decltype(ref)>;

// NonRefType 和 NonRefTypeAlias 都是 int
```

### [3. 完美转发](#)
**由于右值引用变量的属性会被编译器识别成左值**，如果希望能够在传递过程中 保持它的左值或者右值的属性, 就需要用到 **std::forward 完美转发** 。
否则在移动构造的场景下无法完成 资源转移（移动构造）。

std::forward 完美转发 在传参的过程中保留 对象原生类型属性，即保持它的左值或者右值的属性

```c++
void Fun(int& x) { cout << "左值引用" << endl; }
void Fun(const int& x) { cout << "const 左值引用" << endl; }

void Fun(int&& x) { cout << "右值引用" << endl; }
void Fun(const int&& x) { cout << "const 右值引用" << endl; }

template<typename T>
void PerfectForward(T&& t)
{
    // std::forward<T>(t)在传参的过程中保持了t的原生类型属性
	// 完美转发，t是左值引用，保持左值属性
	// 完美转发，t是右值引用，保持右值属性
    Fun(std::forward<T>(t));
}
```
#### [3.1 源码](#)
它允许函数模板将参数原封不动地转发给另一个函数，同时保持参数的值类别（左值、右值）不变,本质上就是很简单的static_cast转换。
```c++
_EXPORT_STD template <class _Ty>
_NODISCARD _MSVC_INTRINSIC constexpr _Ty&& forward(remove_reference_t<_Ty>& _Arg) noexcept {
    return static_cast<_Ty&&>(_Arg);
}

_EXPORT_STD template <class _Ty>
_NODISCARD _MSVC_INTRINSIC constexpr _Ty&& forward(remove_reference_t<_Ty>&& _Arg) noexcept {
    static_assert(!is_lvalue_reference_v<_Ty>, "bad forward call");
    return static_cast<_Ty&&>(_Arg);
}
```
假设有一个模板函数template \<typename T\> void callFunction(T&& arg);，并且我们想要将参数转发给另一个函数void someOtherFunction(int&);，那么我们可以这样做：
```c++
template <typename T>
void callFunction(T&& arg)
{
    someOtherFunction(std::forward<T>(arg));
}
```
### [4. std::move](#)
std::move是标准库中的一个模板函数，它的作用是将一个表达式转换为右值引用，以便触发移动语义。
当一个表达式被std::move包裹时，编译器会将其视为一个即将消失的右值，即使这个表达式实际上是一个左值。
```c++
int main()
{
    int a;
	int& r = a;
	int&& rr = move(a);
    //std::move()函数位于头文件中，该函数名字具有迷惑性，它并不搬移任何东西
	//唯一的功能就是将一个左值强制转化为右值引用，然后实现移动语义
	cout << &r << endl;
	cout << &rr << endl;  
    //我们知道右值不能取地址，不能被修改，而这里都能正常打印
	//证明结论：右值引用变量的属性会被编译器识别成左值
    return 0;
}
/*
000000232B9EFA64
000000232B9EFA64
*/
```
假设有一个MyClass类型，其中包含一个成员函数operator=(MyClass&&)来实现移动赋值操作：
```c++
class MyClass {
public:
    MyClass() : data(new int(0)) {}
    MyClass(MyClass&& other) noexcept : data(other.data) {
        other.data = nullptr; // 将其他对象的数据指针置空
    }
    MyClass& operator=(MyClass&& other) noexcept {
        std::swap(data, other.data);
        return *this;
    }
private:
    int* data;
};
```
现在，我们可以使用std::move来触发移动语义：
```c++
MyClass obj1;
MyClass obj2 = std::move(obj1); // 调用移动构造函数
obj2 = std::move(obj1);          // 调用移动赋值运算符
```
虽然std::move可以提高效率，但在使用时也需要注意以下几点：
* 不要过度使用std::move，**尤其是不要对非临时对象使用std::move**，这可能会导致意外的结果，如对象的有效性问题。
* std::move本身不会执行移动操作，它只是改变了编译器如何处理表达式。实际的移动行为是由类的移动构造函数和移动赋值运算符决定的。
* 当使用std::move时，应确保目标对象的类型支持移动语义，否则可能引发未定义的行为。

总之，std::move是一个强大的工具，可以显著提升性能，特别是在处理大对象或资源密集型操作时。然而，正确和审慎地使用它是关键。
#### [4.1 源码](#)
它的源代码相当简洁，主要作用是将左值表达式转换为右值引用，以便触发移动语义。
```c++
namespace std {
    _EXPORT_STD template <class _Ty>
    _NODISCARD _MSVC_INTRINSIC constexpr remove_reference_t<_Ty>&& move(_Ty&& _Arg) noexcept {
        return static_cast<remove_reference_t<_Ty>&&>(_Arg);
    }
}
```

