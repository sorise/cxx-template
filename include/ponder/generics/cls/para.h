#ifndef CXX_TEMPLATE_PARA_H
#define CXX_TEMPLATE_PARA_H

#include <vector>
#include <mutex>
#include <shared_mutex>

namespace ponder::generics::cls{

    template<typename T, typename ...Args>
    class swagger{

    public:
        constexpr size_t args_count() noexcept{
            return sizeof...(Args);
        }

        constexpr std::array<size_t, sizeof...(Args) + 1> args_size(){
            return {sizeof(T), sizeof(Args)...};
        }
    };


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

    /*
    class Geometry
    {
     public:
         virtual void Draw() const = 0;
    };

    class Line : public Geometry
    {
    public:
         virtual void Draw() const{    std::cout << "Line Draw()\n";    }
    };

    class Circle : public Geometry
    {
    public:
        virtual void Draw() const{    std::cout << "Circle Draw()\n";    }
    };

    class Rectangle : public Geometry
    {
    public:
        virtual void Draw() const{    std::cout << "Rectangle Draw()\n";    }
    };
     */

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

    template<typename SON>
    class Father{
    public:
          Father() = default;
          void call_son(){
              SON *s = static_cast<SON*>(this);
              s->do_something();
          }
    };
    class Son : public Father<Son>{
    public:
        void do_something(){
            std::cout << "Son do_something\n";
        }
    };


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

    class car: public std::enable_shared_from_this<car>{
    public:
        /**
         * car 类的构造函数
         *
         * @param _name 车辆的名称，通过移动语义传递，以提高效率。
         *               移动语义可以避免复制字符串，从而节省资源。
         */
        explicit car(std::string _name):name(std::move(_name)){
            // 构造函数体为空，通过初始化列表初始化成员变量 name
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

}
#endif //CXX_TEMPLATE_PARA_H
