//
// Created by 14270 on 24-6-19.
//

#ifndef CXX_TEMPLATE_VARI_H
#define CXX_TEMPLATE_VARI_H

namespace ponder::generics::cls{

    //最大值
    template<typename T>
    inline constexpr T max_value = T(0);

    template<>
    inline constexpr int max_value<int> = 2147483647;

    template<>
    inline constexpr double max_value<double> = 1.79769e+308;

    template<typename T = int >
    inline constexpr T min_value = T(0);

    template<>
    inline constexpr int min_value<int> = -2147483648;


    template<typename T, size_t N> T Variable[N];


    template <typename T> class D {
    public:
        template <typename W> static W STP; //静态成员变量模板声明
    };

    template <typename T>
    template <typename W>
    W D<T>::STP = 5;

    template<typename T>
    using Str_Map = std::map<std::string, T>;

    template<typename G, typename E>
    union unit{
        G cell;
        E grid;
    };


    template<typename U>
    class graduate;

    template<typename T>
    class student{
        template<typename> friend class graduate;
    private:
        std::string student_id{};
        double score{0};
        T * data;
    };

    template<typename U>
    class graduate{
    private:
        student<float> _student;
    public:
        double get_score(){
            std::cout << "score: " << _student.score << std::endl;
            return _student.score;
        }
        void set_score(double score){
            _student.score = score;
        }
    };

    template<typename T>
    class special{
        friend T;
    private:
        int data {0};
    };

    class object{
    public:
         void call(){
            special<object> t;
            t.data = 100;
            std::cout << "special data value: " << t.data << std::endl;
        }
    };




}

#endif //CXX_TEMPLATE_VARI_H
