//
//  StBernand.cpp
//  StBernandScript
//
//  Created by Didrik Munther on 14/01/16.
//  Copyright © 2016 Didrik Munther. All rights reserved.
//

#include <iostream>

#include "StBernand.hpp"


int StBernand::start() {
    stack.addVariable("true", stack.addObject(new Object(stack.addFloat(1), t_type_bool)));
    stack.addVariable("false", stack.addObject(new Object(stack.addFloat(0), t_type_bool)));
    
    while(true) {
        std::string input = "";
        std::cout << "> ";
        std::getline(std::cin, input);
        if(input == "exit")
            break;
        
        auto result = lexer.lex(input, Lexer::Options::Lex);
        auto ret = parser.parse(result, &stack);
        std::cout << ret.first << "\n";
        stack.clear();
    }
    
    return 0;
}