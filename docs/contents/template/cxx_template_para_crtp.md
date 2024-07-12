## [C++ 可变参数模板、CRTP](#)
**介绍**：C++ 11 新增可变参数模板、模板继承和CRTP技巧。

-----

* [1. 可变参数模板](#1-可变参数模板)
* [2. 多态在模板中的应用](#2-多态在模板中的应用)
* [3. CRTP](#3-crtp)
* [4. 混入 Mixins](#4-混入-mixins)

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

#### [1.4 通过递归继承方式展开参数包](#)
通过递归继承的方式展开参数包，这是一种非常常见的类模板参数包展开方式。

```c++
//主模板声明
template<typename ...Args>
class Expand{
public:
    virtual constexpr size_t args_count() noexcept{
        return sizeof...(Args);
    }
};
//可变参类模板的偏特化
template<typename T, typename ...Args>
class Expand<T, Args...>: private Expand<Args...>{
public:
    constexpr size_t args_count() noexcept override{
        return sizeof...(Args) + 1;
    }
};
```
**使用**:
```c++
Expand<double, int, bool, float> goer;
std::cout << goer.args_count();
```

**Expand\<\>** `--->` **Expand\<double>** `--->`  **Expand\<double, int>**  `--->`  **Expand\<double, int, bool>**
`--->` **Expand\<double, int, bool, float>**

### [2. 多态在模板中的应用](#)
**多态**: 指的是子类可以替换父类! 对同一个完全相同的消息，所表现出来的动作是各不相同的！  
**重载**: 函数名相同，但是函数的参数不同，调用时根据参数的不同决定调用哪一个函数。

多态在模板中的应用，主要通过虚函数来实现，有父类和子类（有继承关系），父类中必须含有虚函数，子类重写父类中的虚函数。

**传统多态也叫作动态多态（运行时的多态），因为要访问虚函数表指针，所以程序执行性能上多少会有一些影响**

[**静态多态**](https://www.cnblogs.com/lizhenghn/p/3667681.html): 在模板中运用的多态技术在编译期间就确定了具体调用谁，因此并不存在这样的性能问题，这也是泛型编程与面向对象程序设计的不同之处的一个体现。

**优点**：
* 由于静多态是在编译期完成的，因此效率较高，编译器也可以进行优化；
* 有很强的适配性和松耦合性，比如可以通过偏特化、全特化来处理特殊类型；
* 最重要一点是静态多态通过模板编程为C++带来了泛型设计的概念，比如强大的STL库。

**缺点：**
* 由于是模板来实现静态多态，因此模板的不足也就是静多态的劣势，比如调试困难、编译耗时、代码膨胀、编译器支持的兼容性
* 不能够处理异质对象集合

**不同点**：
* 本质不同，静态多态在编译期决定，由模板具现完成，而动态多态在运行期决定，由继承、虚函数实现；
* 动态多态中接口是显式的，以函数签名为中心，多态通过虚函数在运行期实现，静态多台中接口是隐式的，以有效表达式为中心，多态通过模板具现在编译期完成

**相同点**：
* 都能够实现多态性，静态多态/编译期多态、动态多态/运行期多态；
* 都能够使接口和实现相分离，一个是模板定义接口，类型参数定义实现，一个是基类虚函数定义接口，继承类负责实现；

#### [2.1 动态多态](#)
动态多态是在运行时完成的，也可以叫做运行期多态，这造就了动态多态机制在处理异质对象集合时的强大威力（当然，也有了一点点性能损失）。

```c++
//示例
class Geometry
{
public:
      virtual void Draw() const = 0;
};

class Line: public Geometry
{
public:
      virtual void Draw() const{    std::cout << "Line Draw()\n";    }
};

class Circle: public Geometry
{
public:
      virtual void Draw() const{    std::cout << "Circle Draw()\n";    }
};

class Rectangle: public Geometry
{
public:
      virtual void Draw() const{    std::cout << "Rectangle Draw()\n";    }
};

void DrawGeometry(const Geometry *geo)
{
    geo->Draw();
}

//动态多态最吸引人之处在于处理异质对象集合的能力
void DrawGeometry(std::vector<DynamicPoly::Geometry*> vecGeo)
{
    const size_t size = vecGeo.size();
    for(size_t i = 0; i < size; ++i)
    vecGeo[i]->Draw();
}
```

#### [2.2 静态多态](#)
在模板中的多态使用方式，与传统的多态相比，省略一个父类，也省略成员函数前面的virtual修饰符，所以代码看起来更简洁。而且，显然静态多态比动态多态具有更好的执行效率，因为查询虚函数表确定调用哪个虚函数等一系列判断决策只出现在动态多态中。
```c++
class Line
{
public:
    void Draw()const{    std::cout << "Line Draw()\n";    }
};

class Circle
{
public:
    void Draw(const char* name=NULL)const{    std::cout << "Circle Draw()\n";    }
};

class Rectangle
{
public:
    void Draw(int i = 0)const{    std::cout << "Rectangle Draw()\n";    }
};

template<typename Geometry>
void DrawGeometry(const Geometry& geo)
{
    geo.Draw();
}

template<typename Geometry>
void DrawGeometry(std::vector<Geometry> vecGeo)
{
    const size_t size = vecGeo.size();
    for(size_t i = 0; i < size; ++i)
        vecGeo[i].Draw();
}
```

### [3. CRTP](#)
奇异的递归模板模式（Curiously Recurring Template Pattern，CRTP）不是一种新技术，而是一种模板编程时使用的编程手法—**把派生类作为基类的模板参数**。

```c++
// 基类是模板类
template<typename SON>
class Father{
public:
    Father() = default;
    void call_son(){
        SON *s = static_cast<SON*>(this);
        s->do_something();
    }
};
// 派生类Son继承自Father<Son>，并以自身作为模板参数传递给基类
class Son: public Father<Son>{
public:
    void do_something(){
        std::cout << "Son do_something\n";
    }
};

// 在基类中使用派生类对象

Son son;
son.call_son(); //Son do_something
```
可以看到，在基类内部，通过使用static_cast，将this指针转换为模板参数类型T的指针，然后调用类型T的方法。这里有个问题：

当static_cast用于类层次结构中基类（父类）和派生类（子类）之间指针或引用的转换，在进行上行转换（把派生类的指针或引用转换成基类表示）是安全的；而下行转换（把基类指针或引用转换为派生类表示）由于没有动态类型检查，所以不一定安全。

但是，CRTP 的设计原则就是假设 Derived 会继承于 Base。CRTP的要求是，所有的派生类应有如下形式的定义：

```c++
class Derived1 : public Base<Derived1> {};
class Derived2 : public Base<Derived2> {};
```
从基类对象的角度看，派生类对象就是本身（即Derived是一个Base，猫是一种动物）。

而在实际使用时，我们只使用Derived1，Derived2的对象，不会直接使用Base<Derived1>，Base<Derived2>类型定义对象。这就保证了当static_cast被执行的时候，基类Base<DerivedX>的指针一定指向一个子类DerivedX的对象，因此转换是安全的。

* **优点**：省去动态绑定、查询虚函数表带来的开销。通过CRTP，基类可以获得到派生类的类型，提供各种操作，比普通的继承更加灵活。但CRTP基类并不会单独使用，只是作为一个模板的功能。
* **缺点**：模板的通病，即影响代码的可读性。


#### [3.1 CRTP应用之计数器](#)
实现不同子类的计数器：

```c++
template<typename T>
class counter
{
public:
    static int count;
    static std::mutex mtx;
    counter()
    {
        std::lock_guard<std::mutex> lck(counter<T>::mtx);
        ++counter<T>::count;
    }
    ~counter()
    {
        std::lock_guard<std::mutex> lck(counter<T>::mtx);
        --counter<T>::count;
    }
};

template<typename T>
int counter<T>::count = 0;

template<typename T>
std::mutex counter<T>::mtx = std::mutex();

class dog_counter : public counter<dog_counter>
{
public:
    [[nodiscard]] int get_count() const
    {
        return this->count;
    }
};

class cat_counter : public counter<cat_counter>
{
public:
    [[nodiscard]] int get_count() const
    {
        return this->count;
    }
};
/*
DogCount : 1
DogCount : 2
DogCount : 1
CatCount : 7
t1 CatCount : 21
t2 CatCount : 14
* */
```

#### [3.2 enable_shared_from_this](#)
它提供了一种方式，允许一个对象（通常是由 std::shared_ptr 管理的）生成一个指向自身的 std::shared_ptr，即使对象自身是
通过裸指针（raw pointer）传递给 std::shared_ptr 构造函数的。

这种操作通常在对象需要将自身作为参数传递给回调函数或异步操作时非常有用。

某个类想返回智能指针版的this时，需要该类继承enable_shared_from_this,通过shared_from_this()返回对应智能指针。
```c++
_EXPORT_STD template <class _Ty>
class enable_shared_from_this { // provide member functions that create shared_ptr to this
public:
    using _Esft_type = enable_shared_from_this;

    _NODISCARD shared_ptr<_Ty> shared_from_this() {
        return shared_ptr<_Ty>(_Wptr);
    }

    _NODISCARD shared_ptr<const _Ty> shared_from_this() const {
        return shared_ptr<const _Ty>(_Wptr);
    }

    _NODISCARD weak_ptr<_Ty> weak_from_this() noexcept {
        return _Wptr;
    }

    _NODISCARD weak_ptr<const _Ty> weak_from_this() const noexcept {
        return _Wptr;
    }

protected:
    constexpr enable_shared_from_this() noexcept : _Wptr() {}

    enable_shared_from_this(const enable_shared_from_this&) noexcept : _Wptr() {
        // construct (must value-initialize _Wptr)
    }

    enable_shared_from_this& operator=(const enable_shared_from_this&) noexcept { // assign (must not change _Wptr)
        return *this;
    }

    ~enable_shared_from_this() = default;

private:
    template <class _Yty> friend class shared_ptr;

    mutable weak_ptr<_Ty> _Wptr;
};

```
enable_shared_from_this从本质上来说解决了不能直接冲this对象构造智能指针的问题，但是使用时也需要注意，既返回的智能智能必须要通过shared_from_this()获取。

**不使用enable_shared_from_this的问题**
```c++
int main()
{
  int* pt = new int();
  shared_ptr<int> _p1(pt);
  shared_ptr<int> _p2(pt);
  std::cout << "_p1.use_count() = " << _p1.use_count() << std::endl;
  std::cout << " _p2.use_count() = " << _p2.use_count() << std::endl;
  return 0;
} //这会导致多次释放问题
```
如果我们使用对象本身来获取智能指针，**还是有多次释放问题**。
```c++
class car{
public:
    explicit car(std::string _name):name(std::move(_name)){

    }
    std::shared_ptr<car> get_shared(){
        return std::shared_ptr<car>(this);
    }
    ~car(){
        std::cout << "~car() - car free." << std::endl;
    }
    void show_name(){
        std::cout << "car name: " << name << std::endl;
    }
private:
    std::string name;
};

int main()
{
   car* vtd = new car("Xiaomi SU7");
   std::shared_ptr<car> byd1 = vtd->get_shared();
   std::shared_ptr<car> byd2 = vtd->get_shared();

   byd1->show_name();
   byd2->show_name();
}
```
此时需要使用enable_shared_from_this，通过shared_from_this()返回对应智能指针。
```c++
class car: public std::enable_shared_from_this<car>{
public:
    explicit car(std::string _name):name(std::move(_name)){

    }
    std::shared_ptr<car> get_shared(){
        return shared_from_this();
    }
    ~car(){
        std::cout << "~car() - car free." << std::endl;
    }
    void show_name(){
        std::cout << "car name: " << name << std::endl;
    }
private:
    std::string name;
};

int main()
{
   std::shared_ptr<car> byd1 = std::make_shared<car>("xiaomi su7");
   std::shared_ptr<car> byd2 = byd1->get_shared();

   byd1->show_name();
   byd2->show_name();
}
```
通过原始指针的方式实例化shared_ptr很容易产生同一个原始指针实例化多个shared_ptr这样的编码疏忽，从而造成严重后果。 因此**尽量使用std::make_shared或者std::allocate_shared来降低出错的可能性**。

### [4. 混入 Mixins](#)
mixin是一种设计思想，用于将相互独立的类的功能组合在一起，以一种安全的方式来模拟多重继承。而c++中mixin的实现通常
采用 **Template Parameters as Base Classes** 的方法，既实现了多个不交叉的类的功能组合，又避免了多重继承的问题。

**混入的实现手段是把传入的模板参数当作该类模板的父类**。

> 是一种编程手法，用于描述类与类之间的一种关系，这种关系比较类似于多重继承，看起来更像颠倒过来的继承（基类继承自派生类）。

```c++
template<class T>
class Father : public T
{
    //todo
};

template<typename... T>
class Base : public T...
{
public:
    void myfunc()
    {
        cout << "Base::myfunc()执行了!" << endl;
    }
}
```
使用例子：
```c++
template <typename T>
class DerivePrint1 : public T
{
public :
    void print() {
        cout<<"Hello World 1!"<<endl;
        T::print();
    }
};

template <typename T>
class DerivePrint2 : public T
{
public :
    void print() {
        cout<<"Hello World 2!"<<endl;
        T::print();
    }
};

class myClass{
public:
    void print(){
        cout<<"myClass"<<endl;
    }
};


int main()
{
    myClass my1;
    my1.print();
    cout<<"-----------------------"<<endl;
    DerivePrint1<myClass> my2;
    my2.print();
     cout<<"-----------------------"<<endl;
    DerivePrint2<myClass> my3;
    my3.print();
     cout<<"-----------------------"<<endl;
    DerivePrint2<DerivePrint1<myClass>> my4;
    my4.print();
     cout<<"-----------------------"<<endl;
    DerivePrint1<DerivePrint2<myClass>> my5;
    my5.print();
}

其输出结果如下：
myClass
-----------------------
Hello World 1!
myClass
-----------------------
Hello World 2!
myClass
-----------------------
Hello World 2!
Hello World 1!
myClass
-----------------------
Hello World 1!
Hello World 2!
myClass
```
