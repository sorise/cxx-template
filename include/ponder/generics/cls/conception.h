//
// Created by 14270 on 2024/7/16.
//

#ifndef CXX_TEMPLATE_CONCEPTION_H
#define CXX_TEMPLATE_CONCEPTION_H
#include <functional>
#include <algorithm>
#include <iostream>
#include <format>
#include <stack>

namespace ponder::generics::cls{
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
        process(std::forward<T>(t));
    }

    constexpr int max_size = 100; // 编译时确定的最大尺寸

    template<int A, int B>
    struct Add {
        constexpr static const int value = A + B;
    };

    template<size_t A, size_t B>
    constexpr size_t Sub(){
        return A - B;
    }

    template<size_t N> constexpr size_t fibonacci =  fibonacci<N-1> + fibonacci<N-2>;
    template<> constexpr size_t fibonacci<0> = 0;
    template<> constexpr size_t fibonacci<1> = 1;

    template<size_t N> constexpr double golden_ratio = fibonacci<N + 1> * 1.0  / fibonacci<N>;

    template<typename T, T... ints>
    void print_sequence(std::integer_sequence<T, ints...> int_seq)
    {
        std::cout << "size: " << int_seq.size() << ", sequence: ";
        ((std::cout << ints << ' '), ...);
        std::cout << '\n';
    }


    template<class T>
    std::enable_if<std::is_arithmetic<T>::value, long>::type
    ADD_M(T&& v, T&& v2) {
        auto t1 = static_cast<long>(v + v2);
        auto t2 = static_cast<long>(v);
        return t1 % t2;
    } // #1

    template<typename T> struct array_size;
    template<typename E, size_t N>
    struct array_size<E[N]> {
        using element_type = E;
        static constexpr size_t size = N;
    };

    template<typename F> struct function_traits;
    template<typename R, typename... Args>
    struct function_traits<R(Args...)> {
        using args_type = std::tuple<Args...>;
        using return_type = R;
        static constexpr size_t args_size = sizeof...(Args);
        template<size_t Idx> using arg = std::tuple_element_t<Idx, std::tuple<Args...>>;
    };

    template<typename T>
    bool number_equal_impl(T a, T b, std::true_type tp)
    {
        return ::fabs(a - b) < std::numeric_limits<T>::epsilon();
    }

    template<typename T>
    bool number_equal_impl(T a, T b, std::false_type fp)
    {
        return a == b;
    }

    template<typename T>
    bool number_equal(T a, T b)
    {
        return number_equal_impl(a, b, std::is_floating_point<T>{});
    }

    template<typename T>
    auto number_equal_test(T a, T b) -> std::enable_if_t<std::is_arithmetic_v<T>, bool>{
        if constexpr (std::is_integral_v<T>()){
            return a == b;
        }else{
            return ::fabs(a - b) < std::numeric_limits<T>::epsilon();
        }
    }


    template<typename T, typename U>
    concept isChildOf = std::is_base_of<U, T>::value;//类型约束, T必须继承自U

    template <typename T>
    concept integral = std::is_integral<T>::value;

    integral auto inc(integral auto a) { return ++a; }

    template<typename F, typename S>
    requires isChildOf<F, S>
    auto add_one(S* x) -> F* {
        return static_cast<F*>(x);
    }

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

    std::string returnString(){
        return "hello";
    }

    template<typename T>
    void example(T x) {
        std::cout << x << std::endl;
    }

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

    template<typename T>
    void muse_constant(const T& arg){
        std::cout << typeid(arg).name() << std::endl;
    }

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

}
#endif //CXX_TEMPLATE_CONCEPTION_H
