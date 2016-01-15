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
    
    std::pair<std::string, int> parse(std::vector<std::pair<std::string, Token>> tokens, Stack* stack, bool doShuffle = true);
    std::vector<std::pair<std::string, Token>> shuffle(std::vector<std::pair<std::string, Token>> tokens);
    bool checkValidity(std::vector<std::pair<std::string, Token>> tokens);
    
private:
    template<typename Iter, typename Vec>
    std::pair<std::string, int> error(Iter it, Vec* vec, std::string msg, ErrorOption option = PrintHint) {
        std::string toReturn = "";
        if((option & PrintHint) && Options::has(Options::Option::PrintErrPosition)) {
            int position = 0;
            {
                auto i = vec->begin() + 1;
                while(i != vec->end() - 1) {
                    if(i < it)
                        position += i->first.length() + 1;
                    
                    toReturn += i->first + " ";
                    i++;
                }
            }
            toReturn += "\n";
            
            for(int i = 0; i < position; i++) {
                toReturn += "-";
            }
            toReturn += "^";
//                std::cout << "(" << position << ")";
            toReturn += "\n";
        }
        
        toReturn += "[Error] \"" + it->first + "\" (" + std::to_string(it->second) + "): " + msg;
        return std::make_pair(toReturn, -100);
    }
    
    int doOperationLogic(std::pair<std::string, Token> op, Stack* stack);
    template<typename T, typename T2>
    std::pair<std::string, int> doOperation(std::pair<std::string, Token> op, T first, T2 second) {
        std::string result = "";
        switch(op.second) {
            case t_plus:
                result = second + first;
                break;
                
            case t_minus:
                result = second - first;
                break;
                
            case t_forw_slash:
                result = second / first;
                break;
                
            case t_asterix:
                result = second * first;
                break;
                
            case t_raised:
                result = pow(second, first);
                break;
                
            case t_modulo:
                result = (int)floor(second) % (int)floor(first);
                break;
                
            case t_equal_to:
                result = second == first;
                break;
                
            case t_not_equal_to:
                result = second != first;
                break;
                
            case t_less_than:
                result = second < first;
                break;
                
            case t_greater_than:
                result = second > first;
                break;
                
            case t_less_than_or_equal:
                result = second <= first;
                break;
                
            case t_greater_than_or_equal:
                result = second >= first;
                break;
                
            default:
                return std::make_pair("", -1);
        }
    }
    
    template<typename Vec, typename Val>
    bool isInVector(Vec vec, Val val) {
        return std::find(vec.begin(), vec.end(), val) != vec.end();
    }
    
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
    
    int getPriorityLevel(Token token, std::map<int, std::vector<Token>> order);
    
    std::pair<std::string, int> parse(std::vector<std::pair<std::string, Token>>* tokens, Stack* stack, std::vector<std::pair<std::string, Token>>::iterator* start, std::vector<std::pair<std::string, Token>>::iterator* stop, bool doShuffle = true);
    
};

#endif /* Parser_hpp */
