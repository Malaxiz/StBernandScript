//
//  Parser.cpp
//  StBernandScript
//
//  Created by Didrik Munther on 12/01/16.
//  Copyright © 2016 Didrik Munther. All rights reserved.
//

#include <iostream>
#include <algorithm>
#include <math.h>

#include "Parser.hpp"
#include "Options.hpp"
#include "Helper.hpp"


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
    
    auto it = tokens.begin();
    while(it != tokens.end()) {
        if(it+1 != tokens.end())
            next = (it+1)->second;
        else
            next = t_end;
        
        switch(it->second) {
                
            case t_increment:
            case t_decrement:   // DON'T TOUCH THIS
            {
                std::pair<std::string, Token> op = std::make_pair(it->second == t_increment ? "+" : "-", it->second == t_increment ? t_plus : t_minus);
                                                                            // X
                *it = *(it-1);                                              // X
                it = tokens.insert(it + 1, std::make_pair("1", t_number));  // 1
                it = tokens.insert(it + 1, op);                             // -
                it = tokens.insert(it + 1, std::make_pair("=", t_equals));  // =
//                std::cout << "HERE: " << it->first << "   |  " << std::to_string(it == tokens.begin()) << "\n";
            }
                break;
            
            case t_plus:
            case t_minus:
            case t_forw_slash:
            case t_asterix:
            case t_raised:
            case t_modulo:
            case t_equal_to:
            case t_not_equal_to:
            case t_greater_than:
            case t_less_than:
            case t_less_than_or_equal:
            case t_greater_than_or_equal:
            {
                if(next == t_parenthesis_open) {
                    auto endParenthesis = Helper::getClosingToken(&it, &tokens, t_parenthesis_close);
                    std::cout << endParenthesis->first << "\n";
                    auto temp = *it;
                    tokens.erase(it, it + 1);
                    tokens.insert(endParenthesis - 2, temp);
                    break;
                    
                }
                if(next == t_quote) {
                    auto endQuote = Helper::getClosingToken(&it, &tokens, t_quote);
                    auto temp = *it;
                    tokens.erase(it, it + 1);
                    tokens.insert(endQuote - 2, temp);
                    break;
                }
            }
                
            case t_dot:
            {
                if(next == t_number || next == t_identifier) {
                    std::iter_swap(it, it+1);
                }
            }
                break;
                
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
            
            case t_equals:
                std::rotate(it, it + 1, tokens.end() - 1);
                if(it != (tokens.end() - 2)) // when the bugs come look here
                    it--;
                break;
                
            case t_quote:
                it = Helper::getClosingToken(&it, &tokens);
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
    
    std::vector<std::pair<std::string, Token>>::iterator lastParenthesis = tokens.end();
    
    auto it = tokens.begin();
    while(it != tokens.end()) {
        if(it->second == t_parenthesis_close || it->second == t_parenthesis_open)
            lastParenthesis = it;
        
        if(!isExpected(it->second)) {
            std::string err = "";
            if(it->second == t_end)
                err = "Unexpected end of expression.";
            else if(it->second == t_unknown)
                err = "Unknown token.";
            else if(it->second == t_error)
                err = "Simply error.";
            else {
                std::string exp = "";
                Lexer lexer;
                bool first = true; // For printing commas
                for(auto& i: expected) {
                    auto t = lexer.getStringFromToken(i);
                    if(t != "") {
                        if(!first)
                            exp += ", ";
                        exp += t;
                    }
                    first = false;
                }
                err = "Token not expected.";
                if(Options::has(Options::Option::PrintExpected))
                    err += "\nExpected: " + exp;
            }
            std::cout << error(it, &tokens, err).first;
            return false;
        }
        
        setExpected({});
        
        switch(it->second) {
            case t_start:
                addExpected({t_number, t_identifier, t_parenthesis_open, t_quote, t_end});
                break;
            case t_end:
                if(parenthesisLevel > 0) {
                    while(it != tokens.begin()) {
                        if(it->second == t_parenthesis_open)
                            if(Helper::getClosingToken(&it, &tokens, t_parenthesis_close) == tokens.end())
                                break;
                        it--;
                    }
                    std::cout << error(it, &tokens, "Unmatched [(].").first;
                    return false;
                } else if(parenthesisLevel < 0) {
                    it = lastParenthesis;
                    std::cout << error(it, &tokens, "Extraneous [)].").first;
                    return false;
                }
                break;
                
            case t_semicolon:
                addExpected({t_number, t_identifier, t_parenthesis_open, t_end, t_semicolon});
                break;
                
            case t_number:
                addExpected(getOperatorTokens());
                addExpected({t_parenthesis_close, t_end});
                break;
                
            case t_identifier:
                addExpected(getOperatorTokens());
                addExpected({t_equals, t_dot, t_parenthesis_close, t_increment, t_decrement, t_end});
                break;
                
            case t_dot:
                addExpected({t_identifier});
                break;
                
            case t_equals:
                addExpected({t_number, t_identifier, t_quote, t_parenthesis_open});
                break;
                
            case t_parenthesis_open:
                parenthesisLevel += 1;
                addExpected({t_number, t_identifier, t_parenthesis_open, t_parenthesis_close});
                break;
                
            case t_parenthesis_close:
                parenthesisLevel -= 1;
                addExpected(getOperatorTokens());
                addExpected({t_parenthesis_close, t_end});
                break;
                
            case t_quote:
            {
                auto closing = Helper::getClosingToken(&it, &tokens);
                if(closing == tokens.end()) {
                    it = tokens.end(); // Create the unexpected end of expression error
                } else {
                    it = closing;
                    addExpected(getOperatorTokens());
                    addExpected({t_end, t_parenthesis_close});
                }
            }
                break;
                
            case t_increment:
            case t_decrement:
                addExpected(getOperatorTokens());
                addExpected({t_end});
                break;
                
            case t_plus:
            case t_minus:
            case t_forw_slash:
            case t_asterix:
            case t_raised:
            case t_modulo:
            case t_equal_to:
            case t_not_equal_to:
            case t_greater_than:
            case t_less_than:
            case t_less_than_or_equal:
            case t_greater_than_or_equal:
                addExpected({t_number, t_identifier, t_parenthesis_open, t_quote});
                break;
            
            default:
                std::cout << error(it, &tokens, "Token not handled. (validity)").first;
                return false;
                break;
                
        }
        
        it++;
    }
    
    return true;
}

std::pair<std::string, int> Parser::parse(std::vector<std::pair<std::string, Token>> tokens, Stack* stack, bool doShuffle /* = true */) {
    if(!checkValidity(tokens))
        return std::make_pair("", -100);
    
    Lexer lexer;
    if(Options::has(Options::Option::PrintTokensBeforeShuffle))
        lexer.printTokens(tokens);
    
    if(doShuffle)
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
                
                if(toAssign->isConst) {
                    return error(it, &tokens, "Variable is constant.", PrintNone);
                } else {
                    if(!toAssign || !value)
                        return error(it, &tokens, "Unable to assign value to element.", PrintNone);
                    
                    toAssign->type = value->type;
                    toAssign->value = value->value;
                    
                    stack->pop();
                }
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
            case t_parenthesis_close:
                break;
                
            case t_quote:
            {
                std::string stringToAdd = "";
                auto start = it + 1;
                auto end = Helper::getClosingToken(&it, &tokens);
                it = end;
                while(start != end) {
                    stringToAdd += start->first;
                    start++;
                }
                void* str = (void*)stack->addString(stringToAdd);
                Object* obj = stack->addObject(new Object(str, t_type_string));
                stack->push(obj);
            }
                break;
                
            case t_plus:
            case t_minus:
            case t_forw_slash:
            case t_asterix:
            case t_raised:
            case t_modulo:
            case t_equal_to:
            case t_not_equal_to:
            case t_greater_than:
            case t_less_than:
            case t_less_than_or_equal:
            case t_greater_than_or_equal:
            {
                auto result = doOperationLogic(*it, stack);
                switch(result) {
                    case -1:
                    case -2:
                        return error(it, &tokens, "Stack doesn't contain atleast 2 objects.", PrintNone);
                        break;
                    case -3:
                        return error(it, &tokens, "Cannot do math on null.", PrintNone);
                        break;
                    case -4:
                        return error(it, &tokens, "This is not a registred mathematical operator.");
                        break;
                    case -5:
                        return error(it, &tokens, "Cannot do math on these types.");
                        break;
                    default:
                        break;
                }
            }
                break;
                
            case t_add_assign:
            case t_sub_assign:
            case t_div_assign:
            case t_mult_assign:
            case t_mod_assign:
                break;
                
            default:
                return error(it, &tokens, "Token not handled. (exec)");
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
        toReturn = obj->getString();
        if(obj->type == t_type_string)
            toReturn = "\"" + toReturn + "\"";
    }
    
    return std::make_pair(toReturn, 0);
}

std::pair<std::string, int> Parser::parse(std::vector<std::pair<std::string, Token>>* tokens, Stack* stack, std::vector<std::pair<std::string, Token>>::iterator* start, std::vector<std::pair<std::string, Token>>::iterator* stop, bool doShuffle /* = true */ ) {
    
    std::cout << "here\n";
    
    auto copy = *tokens;
    
    copy.erase(*stop, copy.end());
    copy.erase(copy.begin(), *start);
    
    return parse(copy, stack);
}

int Parser::doOperationLogic(std::pair<std::string, Token> op, Stack* stack) {
    if(stack->size() < 2)
        return -1;
    
    /*
     
     Priority level:
        1. string,
        2. number,
        3. bool
     
     Object is rather [1 -> 3].
     
     */
    
    Object* obj1 = stack->getObject(0);
    Object* obj2 = stack->getObject(-1);
    stack->pop();
    stack->pop();
    
    if(!obj1 && !obj2)
        return -2; // error
    if(obj1->type == t_null || obj2->type == t_null)
        return -3;
    
    Token type = t_unknown;
    
    std::string result = std::to_string(INT_MIN);
    
    if(obj1->type == t_type_string || obj2->type == t_type_string) {
        type = t_type_string;
        
        auto first = obj1->getString();
        auto second = obj2->getString();
        
        switch(op.second) {
            case t_plus:
                result = second + first;
                break;
                
            case t_asterix:
                result = "";
                if(obj1->type == t_type_number) {
                    for(int i = 0; i < std::stoi(first); i++) {
                        result += second;
                    }
                } else if(obj2->type == t_type_number) {
                    for(int i = 0; i < std::stoi(second); i++) {
                        result += first;
                    }
                } else // both are string
                    return -5;
                
                break;
                
            case t_minus:
            case t_raised:
            case t_forw_slash:
                return -5;
                break;
                
            case t_equal_to:
                result = std::to_string(second == first);
                break;
                
            case t_not_equal_to:
                result = std::to_string(second != first);
                break;
                
            case t_less_than:
                result = std::to_string(second < first);
                break;
                
            case t_greater_than:
                result = std::to_string(second > first);
                break;
                
            case t_less_than_or_equal:
                result = std::to_string(second <= first);
                break;
                
            case t_greater_than_or_equal:
                result = std::to_string(second >= first);
                break;
                
            default:
                return -4;
        }
        
    } else if((obj1->type == t_type_number || obj1->type == t_type_bool) && (obj2->type == t_type_number || obj2->type == t_type_bool)) {
        type = t_type_number;
        if(obj1->type == t_type_bool || obj2->type == t_type_bool)
            type = t_type_bool;
        
        auto first = obj1->getNumber();
        auto second = obj2->getNumber();
        
        switch(op.second) {
            case t_plus:
                result = std::to_string(second + first);
                break;
                
            case t_minus:
                result = std::to_string(second - first);
                break;
                
            case t_forw_slash:
                result = std::to_string(second / first);
                break;
                
            case t_asterix:
                result = std::to_string(second * first);
                break;
                
            case t_raised:
                result = std::to_string(pow(second, first));
                break;
                
            case t_modulo:
                result = std::to_string((int)floor(second) % (int)floor(first));
                break;
                
            case t_equal_to:
                result = std::to_string(second == first);
                break;
                
            case t_not_equal_to:
                result = std::to_string(second != first);
                break;
                
            case t_less_than:
                result = std::to_string(second < first);
                break;
                
            case t_greater_than:
                result = std::to_string(second > first);
                break;
                
            case t_less_than_or_equal:
                result = std::to_string(second <= first);
                break;
                
            case t_greater_than_or_equal:
                result = std::to_string(second >= first);
                break;
                
            default:
                return -4;
        }
    } else
        return -5;
    
    std::vector<Token> boolean = {t_equal_to, t_less_than, t_less_than_or_equal, t_greater_than, t_greater_than_or_equal, t_not_equal_to};
    if(isInVector(boolean, op.second))
        type = t_type_bool;
    
    void* toPush = nullptr;
    switch(type) {
        case t_type_string:
            toPush = (void*)stack->addString(result);
            break;
            
        case t_type_number:
        case t_type_bool:
            toPush = (void*)stack->addFloat(std::stof(result));
            break;
            
        default:
            return -5;
            break;
    }
    stack->push(stack->addObject(new Object(toPush, type)));
    
    return 0; // Success
}

std::vector<Token> Parser::getOperatorTokens() {
    return {t_plus, t_minus, t_asterix, t_forw_slash, t_raised, t_modulo, t_equal_to, t_less_than, t_greater_than, t_less_than_or_equal, t_greater_than_or_equal, t_not_equal_to};
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