//
//  Object.cpp
//  StBernandScript
//
//  Created by Didrik Munther on 12/01/16.
//  Copyright © 2016 Didrik Munther. All rights reserved.
//

#include <iostream>

#include "Object.hpp"
#include "Function.hpp"
#include "Lexer.hpp"
#include "Helper.hpp"


Object::Object(void* value, Token type, bool isConst)
    : value(value)
    , type(type)
    , isConst(isConst)
{
}

Object::~Object() { }

float Object::getNumber() {
    if(type == t_null)
        return INT_MIN;
    if(type == t_type_number)
        return *(float*)(value);
    if(type == t_type_bool)
        return *(float*)(value) > 0 ? 1 : 0;
    if(type == t_type_string) {
        std::string v = *(std::string*)(value);
        return Helper::isNumber(v) ? std::stof(v) : INT_MIN;
    }
    if(type == t_type_function)
        return INT_MIN;
    
    
    return 0;
}

std::string Object::getString() {
    if(type == t_null)
        return "[t_null]";
    if(type == t_type_number)
        return Helper::removeTrailingZeros(std::to_string(*(float*)(value)));
    if(type == t_type_bool)
        return *(float*)(value) > 0 ? "true" : "false";
    if(type == t_type_string)
        return *(std::string*)(value);
    if(type == t_type_function)
        return "[t_type_function]";
    
    return "";
}

Function* Object::getFunction() {
    return nullptr;
}