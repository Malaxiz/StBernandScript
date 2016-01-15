//
//  StBernand.hpp
//  StBernandScript
//
//  Created by Didrik Munther on 14/01/16.
//  Copyright © 2016 Didrik Munther. All rights reserved.
//

#ifndef StBernand_hpp
#define StBernand_hpp

#include <stdio.h>

#include "Stack.hpp"
#include "Lexer.hpp"
#include "Parser.hpp"


class StBernand {
    
public:
    StBernand() : level(0) { };
    
    int start();
    
    
private:
    Lexer lexer;
    Stack stack;
    Parser parser;
    
    int level;
    
};

#endif /* StBernand_hpp */
