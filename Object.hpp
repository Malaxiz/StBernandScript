//
//  Object.hpp
//  StBernandScript
//
//  Created by Didrik Munther on 12/01/16.
//  Copyright © 2016 Didrik Munther. All rights reserved.
//

#ifndef Object_hpp
#define Object_hpp

#include <stdio.h>
#include <map>

#include "Lexer.hpp"


class Function;

class Object {
    
public:
    Object(void* value, Token type);
    ~Object();
    
    void* value;
    std::map<std::string, Object*> values;
    
    Object* getValue(std::string identifier) {
        if(values.find(identifier) == values.end())
            return nullptr;
        else
            return values[identifier];
    }
    
    void addValue(std::string identifier, Object* val) {
        values[identifier] = val;
    }
    
    Token type;
    
    float getNumber();
    std::string getString();
    Function* getFunction();
    
    
};

#endif /* Object_hpp */
