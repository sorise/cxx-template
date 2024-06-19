//
// Created by 14270 on 24-6-12.
//

#ifndef CXX_TEMPLATE_CLS_HPP
#define CXX_TEMPLATE_CLS_HPP
#include <functional>
#include <algorithm>
#include <iostream>
#include <format>
#include <stack>

template<typename T1, typename T2 = int>
struct kv_pair{
    T1 key;
    T2 value;
};

template<typename T1, typename T2,const size_t _SIZE = 8>
struct kv_array{
    kv_pair<T1,T2> arr[_SIZE];
};

namespace ponder::generics::cls{

    template<typename T>
    class Stack final //不让继承
    {
    private:
        static const int SIZE = 8;
        T *container; //容器指针
        int len;
        int capacity;
    public:
        Stack(/* args */);
        Stack(const Stack<T> & other);
        Stack(const int& capty);
        void push(T const&);  // 入栈
        bool pop();           // 出栈
        T top() const;        // 返回栈顶元素
        bool empty() const;   // 如果为空则返回真。
        Stack<T> &operator=(const Stack<T>& other);
        ~Stack();
    };

    template<typename T>
    Stack<T>::Stack(/* args */)
    {
        capacity = SIZE;
        container = new T[SIZE];
        len = 0;
    }

    template<typename T>
    Stack<T>::Stack(const Stack<T> & other){
        if (other.len = 0)
        {
            capacity = SIZE;
        }else{
            capacity = other.capacity;
        }
        len = 0;
        container = new T[capacity];
        for(len = 0; len < other.len; len++)
        {
            container[len] = other.container[len];
        }
    }
    template<typename T>
    Stack<T> & Stack<T>::operator=(const Stack<T>& other){
        if (&other == this); return (*this);
        capacity = other.capacity;
        len = 0;
        delete [] container;//把自己的内容进行释放
        container = new T[capacity]; //重写申请
        for(len = 0; len < other.len; len++)
        {
            //赋值内容
            container[len] = other.container[len];
        }
        return *this;
    }



    template<typename T>
    Stack<T>::Stack(const int& capty){
        capacity = capty;
        container = new T[capacity];
        len = 0;
    }

/* 入栈 */
    template<typename T>
    void Stack<T>::push(T const& val){
        if (len == capacity)
        {
            /* code  增加一倍容量 */
            capacity *= 2;
            //完成复制
            T* container_tem_pointer = new T[capacity];
            for (int i = 0; i < len; i++)
            {
                container_tem_pointer[i] = container[i];
            }
            //释放掉
            delete [] container;
            container = container_tem_pointer;
        }
        //加入栈顶
        container[len++] = val;
    }

// 返回栈顶元素
    template<typename T>
    T Stack<T>::top() const{
        if (len == 0)
        {
            throw "stack is empty!";
        }else{

        }
        return container[len - 1];
    }


// 出栈
    template<typename T>
    bool Stack<T>::pop(){
        if(len > 0) {
            len--;
            return true;
        };
        return false;

    }

    template<typename T>
    bool Stack<T>::empty() const{
        return len == 0;
    }

    template<typename T>
    Stack<T>::~Stack()
    {
        delete [] container;
    }
    template<typename T>
    class tuber{
    public:
        //添加到树上
        template<typename F, typename ...Args >
        void setTimeout(long long milliseconds, F && f, Args&&... args){

        }

        template<typename K, typename V>
        kv_pair<K,V> get(K key, V default_value){
            return kv_pair<K,V>(key, default_value);
        }

        virtual void start(int *obj){
            std::cout << *obj << std::endl;
        }
    };


    template<typename T>
    class MyClass {
        int i = 0;
    public:
        template<class T1>
        requires std::is_arithmetic_v<T1>
        explicit MyClass(T1 v1){
            this->i = static_cast<int>(v1);
            std::cout << std::format("call copy constructor template func {}\n", i);
        }

        template<class T1>
        requires std::is_arithmetic_v<T1>
        explicit MyClass(const T1& v1){
            this->i = static_cast<int>(v1);
            std::cout << std::format("call copy constructor template func {}\n", i);
        }

        template<class U>
        requires std::is_arithmetic_v<U>
        MyClass<T> & operator =(const MyClass<U>& other){
            this->i = other.get_i();
            std::cout << std::format("call operator = template func {}\n", i);
            return *this;
        }

        [[nodiscard]] int get_i() const {
            return i;
        }
    };

    template<typename T, unsigned int N>
    class Array final
    {
    private:
        T *container;
        std::stack<T> _t_stack;
    public:
        Array(std::initializer_list<T> list);
        Array(const Array<T,N>& other);
        Array(const Array<T,N>&& other);
        Array<T,N>& operator=(Array<T,N>& other);
        template<typename U>
        std::string getTypeName(U typeVariable);
        int size() const;
        T &operator[](int idx);
        ~Array();
    };

    template<typename T, unsigned int N>
    template<typename U>
    std::string Array<T,N>::getTypeName(U typeVariable){
        std::string typeName(typeid(typeVariable).name());
        return typeName;
    }
}

#endif //CXX_TEMPLATE_CLS_HPP
