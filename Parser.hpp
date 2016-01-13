//
//  Parser.hpp
//  StBernandScript
//
//  Created by Didrik Munther on 12/01/16.
//  Copyright © 2016 Didrik Munther. All rights reserved.
//

#ifndef Parser_hpp
#define Parser_hpp

#include <stdio.h>
#include <vector>

#include "Lexer.hpp"
#include "Stack.hpp"
#include "Options.hpp"


class Parser {
    
    enum ErrorOption {
        PrintNone   = 0,
        PrintHint   = 1 << 1
    };
    
public:
    Parser();
    
    std::string parse(std::vector<std::pair<std::string, Token>> tokens, Stack* stack, bool doShuffle = true);
    std::vector<std::pair<std::string, Token>> shuffle(std::vector<std::pair<std::string, Token>> tokens);
    bool checkValidity(std::vector<std::pair<std::string, Token>> tokens);
    
private:
    template<typename Iter, typename Vec>
    void error(Iter it, Vec* vec, std::string msg, ErrorOption option = PrintHint) {
        if((option & PrintHint) && Options::has(Options::Option::PrintErrPosition)) {
            int position = 0;
            {
                auto i = vec->begin() + 1;
                while(i != vec->end() - 1) {
                    if(i < it)
                        position += i->first.length() + 1;
                    
                    std::cout << i->first << " ";
                    i++;
                }
            }
            std::cout << "\n";
            
            for(int i = 0; i < position; i++) {
                std::cout << "-";
            }
            std::cout << "^";
//                std::cout << "(" << position << ")";
            std::cout << "\n";
        }
        
        std::cout << "[Error] \"" << it->first << "\" (" << it->second << "): " << msg;
    }
    
    int doMathOperation(std::pair<std::string, Token> op, Stack* stack);
    
    template<typename Vec, typename Val>
    bool isInVector(Vec vec, Val val) {
        return std::find(vec.begin(), vec.end(), val) != vec.end();
    }
    
    std::vector<std::pair<std::string, Token>>::iterator getMatchedParenthesis(std::vector<std::pair<std::string, Token>>::iterator* position, std::vector<std::pair<std::string, Token>>* vec);
    
    std::vector<Token> expected;
    std::vector<Token> getOperatorTokens();
    
    /* TEMPLATED HELPER FUNCTIONS */
    
    void setExpected(std::vector<Token> tokens) {
        expected.clear();
        addExpected(tokens);
    }
    
    void addExpected(std::vector<Token> tokens) {
        expected = mergeVectors(expected, tokens);
    }
    
//    template<typename ... Tokens>
//    void addExpected(Tokens ... tokens) {
//        Token args[] {tokens ...};
//        std::vector<Token> v(std::begin(args), std::end(args));
//        expected = mergeVectors(expected, v);
//    }
//    
//    template<typename ... Tokens>
//    void setExpected(Tokens ... tokens) {
//        expected.clear();
//        addExpected(std::forward<Tokens>(tokens)...);
//    }
    
    bool isExpected(Token token) {
        return isInVector(expected, token);
    }
    
    template<typename V>
    V mergeVectors(V a, V b) {
        V ab;
        ab.reserve(a.size() + b.size());
        ab.insert(ab.end(), a.begin(), a.end());
        ab.insert(ab.end(), b.begin(), b.end());
        return ab;
    }
    
    std::string removeTrailingZeros(std::string input);
    std::string removeUntil(std::string input, char until);
    
    int getPriorityLevel(Token token, std::map<int, std::vector<Token>> order);
    
    std::string parse(std::vector<std::pair<std::string, Token>>* tokens, Stack* stack, std::vector<std::pair<std::string, Token>>::iterator* start, std::vector<std::pair<std::string, Token>>::iterator* stop, bool doShuffle = true);
    
};

#endif /* Parser_hpp */
