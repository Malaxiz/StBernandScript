//
//  Helper.cpp
//  StBernandScript
//
//  Created by Didrik Munther on 15/01/16.
//  Copyright © 2016 Didrik Munther. All rights reserved.
//

#include "Helper.hpp"


std::string Helper::removeTrailingZeros(std::string input) {
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

std::string Helper::removeUntil(std::string input, char until) {
    std::string toReturn = "";
    for(auto& i: input) {
        if(i == until)
            break;
        toReturn += i;
    }
    return toReturn;
}

bool Helper::isIdentifier(std::string input) {
    std::string allowed = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM_";
    std::string allowedAfter = "1234567890";
    
    std::string first = "";
    first.push_back(input.at(0));
    
    return isAllowedString(allowed, first) && isAllowedString(allowed + allowedAfter, input);
}

bool Helper::isNumber(std::string input) {
    std::string allowed = "1234567890";
    return isAllowedString(allowed, input);
}

bool Helper::isAllowedString(std::string allowed, std::string input) {
    if((int)input.length() <= 0)
        return false;
    if((int)allowed.length() <= 0)
        return false;
    
    for(auto& c: input)
        if(allowed.find(c) == std::string::npos)
            return false;
    
    return true;
}

// Remember if you don't send closing token the token will be the same as opening token, aswell as the iterator jumps over on step
std::vector<std::pair<std::string, Token>>::iterator Helper::getClosingToken(std::vector<std::pair<std::string, Token>>::iterator* position, std::vector<std::pair<std::string, Token>>* vec, Token close /* = t_null */) {
    
    Token open = (*position)->second;
    
    int level = 1;
    
    auto it = *position;
    if(close == t_null) { // This means closing sign is same as opening sign, jump one step forward
        close = open;
        it++;
    }
    
    while(it != vec->end()) {
        if(it->second == close)
            level--;
        else if(it->second == open)
            level++;
        
        if(level == 0)
            return it;
        
        ++it;
    }
    
    return vec->end();
}