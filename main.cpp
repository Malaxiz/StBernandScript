//
//  main.cpp
//  StBernandScript
//
//  Created by Didrik Munther on 12/01/16.
//  Copyright © 2016 Didrik Munther. All rights reserved.
//

#include <iostream>

#include "Lexer.hpp"
#include "Parser.hpp"
#include "Stack.hpp"
#include "Options.hpp"


//int test() {
//    std::cout << "test\n";
//    
//    return 2;
//}

int main(int argc, const char * argv[]) {
    
//    void* thing = (void*)&test;
//    ((int(*)())thing)();
    
    Lexer lexer;
    Parser parser;
    Stack stack;
    
    Options::_Options = Options::Option::PrintErrPosition; // Default options
    
    while(true) {
        std::string input = "";
        std::cout << "> ";
        std::getline(std::cin, input);
        if(input == "exit")
            break;
        
        auto result = lexer.lex(input, Lexer::Options::Lex);
        std::cout << parser.parse(result, &stack) << "\n";
        stack.clear();
    }
    
    return 0;
}
