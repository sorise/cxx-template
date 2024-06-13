//
// Created by 14270 on 24-6-12.
//

#ifndef CXX_TEMPLATE_CLS_HPP
#define CXX_TEMPLATE_CLS_HPP
#include <functional>
#include <algorithm>
#include <iostream>

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


}

#endif //CXX_TEMPLATE_CLS_HPP
