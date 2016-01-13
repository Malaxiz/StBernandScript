//
//  Stack.cpp
//  StBernandScript
//
//  Created by Didrik Munther on 12/01/16.
//  Copyright © 2016 Didrik Munther. All rights reserved.
//

#include <iostream>

#include "Stack.hpp"


Stack::Stack() {
    
}

Stack::~Stack() {
    {
        auto it = objects.begin();
        while(it != objects.end()) {
            delete *it;
            it++;
        }
    }
}

int Stack::size() {
    return (int)stack.size();
}

void Stack::printStack() {
    std::cout << "\n== Printing stack ==\n";
    int index = 1;
    for(auto& i: stack) {
        std::cout << "    ";
        for(int x = 0; x < index; x++) {
            std::cout << ">";
        }
        if(i->value)
            std::cout << *(float*)i->value;
        else
            std::cout << "[t_null]";
        std::cout << "\n";
        index++;
    }
    std::cout << "== Ended printing stack ==\n";
}

void Stack::clear() {
    stack.clear();
}

float Stack::getNumberFromObject(Object* obj) {
    if(obj->type == t_type_number)
        return *(float*)(obj->value);
    else if(obj->type == t_type_bool)
        return (bool)*(int*)(obj->value);
    else
        return INT_MIN; // error
}

Object* Stack::getVariable(std::string identifier) {
    if(variables.find(identifier) == variables.end())
        return nullptr;
    else
        return variables[identifier];
}

void Stack::addVariable(std::string identifier, Object* object) {
    variables[identifier] = object;
}

int* Stack::addInt(int toAdd) {
    ints.push_back(toAdd);
    return &ints.back();
}

float* Stack::addFloat(float toAdd) {
    floats.push_back(toAdd);
    return &floats.back();
}

Object* Stack::addObject(Object* toAdd) {
    objects.push_back(toAdd);
    return objects.back();
}

Object* Stack::getTop() {
    return getObject(0);
}

void Stack::push(Object* obj) {
    stack.push_back(obj);
}

Object* Stack::getObject(int index) {
    index = (int)stack.size() + index - 1;
    if(index < stack.size() && index >= 0)
        return stack[index];
    
    return nullptr;
}

void Stack::pop() {
    stack.pop_back();
}