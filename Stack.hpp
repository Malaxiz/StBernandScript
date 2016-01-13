//
//  Stack.hpp
//  StBernandScript
//
//  Created by Didrik Munther on 12/01/16.
//  Copyright © 2016 Didrik Munther. All rights reserved.
//

#ifndef Stack_hpp
#define Stack_hpp

#include <stdio.h>
#include <vector>
#include <map>
#include <list>

#include "Object.hpp"


class Stack {
    
public:
    Stack();
    ~Stack();
    
    float* addFloat(float toAdd);
    std::string* addString(std::string toAdd);
    Object* addObject(Object* object);
    
    void push(Object* obj);
    
    Object* getTop();
    Object* getObject(int index);
    void pop();
    
    int size();
    void printStack();
    void clear();
    
    float getNumberFromObject(Object* obj);
    
    Object* getVariable(std::string identifier);
    void addVariable(std::string identifier, Object* object);
    
private:
    std::map<std::string, Object*> variables;
    std::vector<Object*> stack;
    
    std::list<float> floats;
    std::list<std::string> strings;
    std::list<Object*> objects;
    
};

#endif /* Stack_hpp */
