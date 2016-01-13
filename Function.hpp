//
//  Function.hpp
//  StBernandScript
//
//  Created by Didrik Munther on 13/01/16.
//  Copyright © 2016 Didrik Munther. All rights reserved.
//

#ifndef Function_hpp
#define Function_hpp

#include <stdio.h>
#include <vector>
#include <string>

#include "Lexer.hpp"
#include "Stack.hpp"


class Function {
    
public:
    void call(Stack* stack);
    
private:
    std::vector<std::pair<std::string, Token>> tokens;
    
};

#endif /* Function_hpp */
