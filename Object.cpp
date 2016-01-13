//
//  Object.cpp
//  StBernandScript
//
//  Created by Didrik Munther on 12/01/16.
//  Copyright © 2016 Didrik Munther. All rights reserved.
//

#include "Object.hpp"


Object::Object(void* value, Token type)
    : value(value)
    , type(type)
{
}

Object::~Object() {
    
}