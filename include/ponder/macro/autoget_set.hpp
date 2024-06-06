//
// Created by 14270 on 24-6-6.
//

#include <string>
#include <iostream>
#include "toolkits.hpp"

#ifndef CXX_TEMPLATE_AUTOGET_SET_HPP
#define CXX_TEMPLATE_AUTOGET_SET_HPP

#define AUTO_GET_SET(NAME)             \
    auto get_##NAME() const {                   \
        return _##NAME;                         \
    }                                           \
    template<typename TYPE>                     \
    void set_##NAME(const TYPE& NAME) {       \
        _##NAME = NAME;                         \
    }                                           \

class student{
public:
    MACRO_FOR_EACH(AUTO_GET_SET, age, name);
private:
    int _age;
    std::string _name;
};



#endif //CXX_TEMPLATE_AUTOGET_SET_HPP
