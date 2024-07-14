## [C++ 元编程概念](#)
> **介绍**: 模板元编程产生的源程序是在编译期间执行的程序，可以生成在运行期间执行的程序，可以在编译期执行两种计算：数值计算和类型计算。

----

### [1. C++ 元编程](#)
C++元编程（C++ Metaprogramming）是一种编程技巧，它允许在编译时解决一些问题和计算，生成或修改代码，而不是在运行时执行这些操作。

元编程的核心在于利用C++编译器的模板机制和类型系统的能力，来编写可以在编译期间执行的**代码生成代码**。

**C++元编程的特征**:
* **类型和模板作为数据**：在C++中，类型和模板可以被视为数据，这意味着你可以使用类型和模板来表示数值、布尔值、甚至是复杂的结构，然后在编译期间对这些“数据”进行操作。
* **静态计算**：元编程可以用于在编译期间执行计算，例如数学运算、条件判断、循环等，这些计算的结果被嵌入到最终的二进制代码中，无需在运行时重复执行。
* **代码生成**：元编程可以用来生成代码，比如根据模板参数的不同生成不同的代码路径，或者根据一些条件自动生成函数的重载版本。
* **类型推导和检查**：元编程可以用来推导类型或检查类型属性，例如检查类型是否具有某个成员函数，或者推导类型是整数还是浮点数。

**C++元编程的用途**:
* **性能优化**：在编译期间计算出结果可以避免运行时的计算开销，例如矩阵乘法的优化。
* **类型安全**：在编译时检查类型兼容性，避免运行时错误，例如使用 **SFINAE(Substitution Failure Is Not An Error)** 来有条件地启用模板特化。
* **代码简化**：自动处理代码的冗余部分，减少手动编码的工作量，例如使用模板元编程自动生成大量的重载函数。
* **自定义容器和算法**：构建具有特殊要求的容器或算法，例如基于元编程的智能指针。

元编程的例子,一个简单的元编程例子是使用模板元编程来实现加法。
```c++
template<int A, int B>
struct Add {
    static const int value = A + B;
};

int main() {
    static_assert(Add<3, 5>::value == 8, "Addition failed.");
    return 0;
}
```
在这个例中，**Add** 模板在编译时计算两个整数的和，并将结果存储在 **value** 静态成员中。
**static_assert** 用于验证编译时计算的结果是否正确。

#### [1.1 编译器能够操作的变量和类型](#)
元编程无法控制运行时的代码，它不能操作普通的变量，不能使用运行时关键字，可以采用的语法元素只能是如下集中：

* enum、static、const 用来定义编译器整数常量
* typedef, using 定义元数据
* `T`、`Args...` 声明元数据类型
* template 定义元函数
* `::` 域运算符，解析作用域。

#### [1.2 编译器 IF 例子](#)
```cpp
template<bool FLAG>
struct IF{};

template<>
struct IF<true>{
    static void Func() { std::cout << "true carry-out!\n"; };
};

template<>
struct IF<false>{
    static void Func() { std::cout << "false carry-out!\n"; };
};


int main() {
    
    IF<true>::Func();  //true carry-out!
    IF<false>::Func(); //false carry-out!

    return 0;
}
```

#### [1.3 编译器 SWITCH 例子](#)
```cpp
template<int Case>
struct SWITCH{
    static void Func() { std::cout << "default value: 0\n"; };
};

template<>
struct SWITCH<10>{
    static void Func() { std::cout << "value: 10\n"; };
};

template<>
struct SWITCH<20>{
    static void Func() { std::cout << "value: 20\n"; };
};

template<>
struct SWITCH<30>{
    static void Func() { std::cout << "value: 30\n"; };
};

int main() {
    SWITCH<10>::Func();  //value: 10
    SWITCH<20>::Func();  //value: 20
    SWITCH<30>::Func();  //value: 30
    SWITCH<110>::Func(); //value: 0

    return 0;
}
```
#### [1.4 编译器 for 循环](#)
for循环本质是利用模板递归：
```cpp
template <typename T,int N>
struct FOR{
    static inline void each(T t){
        std::cout << t[N] << std::endl;
        FOR<T, N -1>::each(t);
    }
};

template<typename T>
struct FOR<T,0>{
    static inline void each(T t){
        std::cout << t[0] << std::endl;
    }
};

//完成对 names的循环
int main() {
    std::vector<std::string> names{"remix", "gege", "kun", "2nb", "tome"};
    FOR<std::vector<std::string>, 4>::each(names);
    return 0;
}
```
