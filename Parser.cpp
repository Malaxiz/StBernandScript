//
//  Parser.cpp
//  StBernandScript
//
//  Created by Didrik Munther on 12/01/16.
//  Copyright © 2016 Didrik Munther. All rights reserved.
//

#include <iostream>
#include <algorithm>

#include "Parser.hpp"
#include "Options.hpp"

Parser::Parser() {
    
}

std::vector<std::pair<std::string, Token>> Parser::shuffle(std::vector<std::pair<std::string, Token>> tokens) {
    
    std::map<int, std::vector<Token>> order = {
        {1, {
            t_forw_slash,
            t_asterix,
            t_modulo
        }},
        {2, {
            t_plus,
            t_minus
        }},
        {3, {
            t_equal_to,
            t_not_equal_to,
            t_greater_than,
            t_less_than,
            t_less_than_or_equal,
            t_greater_than_or_equal
        }},
        {4, {
            t_equals
        }}
    };
    
    Token previous  = t_unknown;
    Token next      = t_unknown;
    
    int offset = 0;
    
    auto it = tokens.begin();       // First loop to place them roughly
    while(it != tokens.end()) {
        if(it+1 != tokens.end())
            next = (it+1)->second;
        else
            next = t_end;
        
        
        
        switch(it->second) {
            
            case t_plus:
            case t_minus:
            case t_forw_slash:
            case t_asterix:
            case t_modulo:
            case t_equal_to:
            case t_not_equal_to:
            case t_greater_than:
            case t_less_than:
            case t_less_than_or_equal:
            case t_greater_than_or_equal:
            case t_dot:
                if(next == t_number || next == t_identifier) {
                    std::iter_swap(it, it+1);
                }
                
//            {
//                int prio = getPriorityLevel(it->second, order);
//                for(int index = 0; index < tokens.size() - offset; index++) {
//                    auto current = (it + index);
//                    if(current->second == t_number || current->second == t_identifier)
//                        continue;
//                    
//                    int indexPrio = getPriorityLevel(current->second, order);
//                    if(prio > indexPrio) {
//                        std::rotate(it, it + 1, (it + index));
//                    }
//                    
//                }
//            }
                break;
            
            case t_equals:
                std::rotate(it, it + 1, tokens.end() - 1);
                break;
                
            default:
                break;
        }
        
        previous = it->second;
        offset++;
        it++;
    }
    
    return tokens;
}

bool Parser::checkValidity(std::vector<std::pair<std::string, Token>> tokens) {
    setExpected({t_start, t_end});
    
    int parenthesisLevel = 0;
    
    auto it = tokens.begin();
    while(it != tokens.end()) {
        if(parenthesisLevel == 0) {
            addExpected({t_end});
        }
        
        if(!isExpected(it->second)) {
            if(it->second == t_end)
                error(it, &tokens, "Unexpected end of expression.");
            else if(it->second == t_unknown)
                error(it, &tokens, "Unknown token.");
            else if(it->second == t_error)
                error(it, &tokens, "Simply error.");
            else
                error(it, &tokens, "Token not expected.");
            return false;
        }
        
        setExpected({});
        
        switch(it->second) {
                
            case t_start:
                addExpected({t_number, t_identifier, t_parenthesis_open});
                break;
            case t_end:
                break;
                
            case t_number:
                addExpected(getOperatorTokens());
                addExpected({t_parenthesis_close});
                break;
                
            case t_identifier:
                addExpected(getOperatorTokens());
                addExpected({t_equals, t_dot, t_parenthesis_close});
                break;
                
            case t_dot:
                addExpected({t_identifier});
                break;
                
            case t_equals:
                addExpected({t_number, t_identifier});
                break;
                
            case t_parenthesis_open:
                parenthesisLevel += 1;
                addExpected({t_number, t_identifier, t_parenthesis_open, t_parenthesis_close});
                break;
                
            case t_parenthesis_close:
                parenthesisLevel -= 1;
                addExpected(getOperatorTokens());
                break;
                
            case t_plus:
            case t_minus:
            case t_forw_slash:
            case t_asterix:
            case t_modulo:
            case t_equal_to:
            case t_not_equal_to:
            case t_greater_than:
            case t_less_than:
            case t_less_than_or_equal:
            case t_greater_than_or_equal:
                addExpected({t_number, t_identifier, t_parenthesis_open});
                break;
            
            default:
                error(it, &tokens, "Token not handled.");
                return false;
                break;
                
        }
        
        it++;
    }
    
    return true;
}

std::string Parser::parse(std::vector<std::pair<std::string, Token>> tokens, Stack* stack) {
    if(!checkValidity(tokens))
        return "";
    
    Lexer lexer;
    if(Options::has(Options::Option::PrintTokensBeforeShuffle))
        lexer.printTokens(tokens);
    
    tokens = shuffle(tokens);
    
    if(Options::has(Options::Option::PrintTokensAfterShuffle))
        lexer.printTokens(tokens);
    
    
    auto it = tokens.begin();
    while(it != tokens.end()) {
        switch(it->second) {
                
            case t_start:
            case t_end:
                break;
                
            case t_equals:
            {
                Object* toAssign = stack->getObject(-1);
                Object* value = stack->getObject(0);
                
                if(!toAssign || !value) {
                    error(it, &tokens, "Unable to assign value to element.");
                    return "";
                }
                
                toAssign->type = value->type;
                toAssign->value = value->value;
                
                stack->pop();
            }
                break;
                
            case t_number:
            {
                void* number = (void*)stack->addFloat(std::stoi(it->first));
                Object* obj = stack->addObject(new Object(number, t_type_number));
                stack->push(obj);
            }
                break;
                
            case t_identifier:
            {
                Object* obj = stack->getVariable(it->first);
                if(!obj) {
                    obj = stack->addObject(new Object(nullptr, t_null));
                    stack->addVariable(it->first, obj);
                }
                stack->push(obj);
            }
                break;
                
            case t_dot:
            {
                Object* parent = stack->getObject(-1);
                Object* child  = parent->getValue((it-1)->first);
                if(!child) {
                    child = stack->addObject(new Object(nullptr, t_null));
                    parent->addValue((it-1)->first, child);
                }
                stack->pop();
                stack->pop();
                stack->push(child);
            }
                break;
                
            case t_parenthesis_open:
                break;
            case t_parenthesis_close:
                break;
                
            case t_plus:
            case t_minus:
            case t_forw_slash:
            case t_asterix:
            case t_modulo:
            case t_equal_to:
            case t_not_equal_to:
            case t_greater_than:
            case t_less_than:
            case t_less_than_or_equal:
            case t_greater_than_or_equal:
            {
                auto result = doMathOperation(*it, stack);
                switch(result) {
                    case -1:
                        error(it, &tokens, "This should not happen.", PrintNone);
                        break;
                    case -2:
                        error(it, &tokens, "Stack doesn't contain atleast 2 objects.", PrintNone);
                        break;
                    case -3:
                        error(it, &tokens, "Unable to perform math on top 2 elements in stack.", PrintNone);
                        break;
                    case -4:
                        error(it, &tokens, "This is not a registred mathematical operator.");
                        break;
                    default:
                        break;
                }
                
                if(result < 0)
                    return "";
            }
                break;
                
            case t_add_assign:
            case t_sub_assign:
            case t_div_assign:
            case t_mult_assign:
            case t_mod_assign:
                break;
                
            default:
                error(it, &tokens, "Token not handled. (exec)");
                return "";
        }
        
        if(Options::has(Options::Option::PrintStackDuringExecution))
            stack->printStack();
        
        it++;
    }
    
    if(Options::has(Options::Option::PrintStackAfterExecution))
        stack->printStack();
    
    std::string toReturn = "";
    if(stack->size() > 0) {
        auto obj = stack->getObject(0);
        if(obj->type == t_null)
            toReturn = "[t_null]";
        else if(obj->type == t_type_string) {
            
        } else {
            toReturn = std::to_string(stack->getNumberFromObject(obj));
            if(obj->type == t_type_number) {
                toReturn = removeTrailingZeros(toReturn);
            } else if(obj->type == t_type_bool) {
                toReturn = stack->getNumberFromObject(obj) > 0 ? "true" : "false";
            }
        }
    }
    
    return toReturn;
}

int Parser::doMathOperation(std::pair<std::string, Token> op, Stack* stack) {
    if(stack->size() < 2)
        return -1;
    
    Object* obj1 = stack->getObject(0);
    Object* obj2 = stack->getObject(-1);
    stack->pop();
    stack->pop();
    
    if(!obj1 && !obj2)
        return -2; // error
    if(!((obj1->type == t_type_number || obj1->type == t_type_bool) && (obj2->type == t_type_number || obj2->type == t_type_bool)))
        return -3;
    
    Token type = t_unknown;
    
    float num1 = stack->getNumberFromObject(obj1);
    float num2 = stack->getNumberFromObject(obj2);
    float result = 0;
    
    switch(op.second) {
        case t_plus:
            type = t_type_number;
            result = num2 + num1;
            break;
            
        case t_minus:
            type = t_type_number;
            result = num2 - num1;
            break;
            
        case t_forw_slash:
            type = t_type_number;
            result = num2 / num1;
            break;
            
        case t_asterix:
            type = t_type_number;
            result = num2 * num1;
            break;
            
        case t_equal_to:
            type = t_type_bool;
            result = num2 == num1;
            break;
            
        case t_not_equal_to:
            type = t_type_bool;
            result = num2 != num1;
            break;
            
        case t_less_than:
            type = t_type_bool;
            result = num2 < num1;
            break;
            
        case t_greater_than:
            type = t_type_bool;
            result = num2 > num1;
            break;
            
        case t_less_than_or_equal:
            type = t_type_bool;
            result = num2 <= num1;
            break;
            
        case t_greater_than_or_equal:
            type = t_type_bool;
            result = num2 >= num1;
            break;
            
        default:
            return -4;
    }
    
    void* number;
    if(type == t_type_bool)
        number = (void*)stack->addInt(result);
    else if(type == t_type_number)
        number = (void*)stack->addFloat(result);
    
    stack->push(stack->addObject(new Object(number, type)));
    
    return 0; // Success
}

std::vector<Token> Parser::getOperatorTokens() {
    return {t_plus, t_minus, t_asterix, t_forw_slash, t_modulo, t_equal_to, t_less_than, t_greater_than, t_less_than_or_equal, t_greater_than_or_equal, t_not_equal_to};
}

std::string Parser::removeTrailingZeros(std::string input) {
    int index = 0;
    
    auto it = input.end() - 1;
    while(it != input.begin()) {
        if(*it != '0' && *it != '.')
            break;
        index++;
        if(*it == '.')
            break;
        it--;
    }
    
    return input.substr(0, input.size() - index);
}

std::string Parser::removeUntil(std::string input, char until) {
    std::string toReturn = "";
    for(auto& i: input) {
        if(i == until)
            break;
        toReturn += i;
    }
    return toReturn;
}

int Parser::getPriorityLevel(Token token, std::map<int, std::vector<Token>> order) {
    for(auto& i: order) {
        int level = i.first;
        for(auto& t: i.second)
            if(t == token)
                return level;
    }
    
    return -1; // not found
}