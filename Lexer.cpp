//
//  Lexer.cpp
//  StBernandScript
//
//  Created by Didrik Munther on 12/01/16.
//  Copyright © 2016 Didrik Munther. All rights reserved.
//

#include <iostream>
#include <sstream>

#include "Lexer.hpp"
#include "Helper.hpp"


Lexer::Lexer() {
    
    keywords = {
        {"function",    t_keyword_function},
        {"if",          t_k_if},
        {"then",        t_k_then},
        {"endf",        t_k_fend},
        
//        {"null",        t_null},
    };
    
    operators = {
        {"=",       t_equals},
        {";",       t_semicolon},
        
        {".",       t_dot},
        {",",       t_comma},
        
        {"+",       t_plus},
        {"-",       t_minus},
        {"/",       t_forw_slash},
        {"\\",      t_back_slash},
        {"*",       t_asterix},
        {"%",       t_modulo},
        {"**",      t_raised},
        
        {"+=",      t_add_assign},
        {"-=",      t_sub_assign},
        {"/=",      t_div_assign},
        {"*=",      t_mult_assign},
        
        {"(",       t_parenthesis_open},
        {")",       t_parenthesis_close},
        {"\"",      t_quote},
        
        {"++",      t_increment},
        {"--",      t_decrement},
    
        {"!",       t_not},
        {"==",      t_equal_to},
        {"!=",      t_not_equal_to},
        {"<",       t_less_than},
        {">",       t_greater_than},
        {"<=",      t_less_than_or_equal},
        {">=",      t_greater_than_or_equal},
    };
    
    types = {
        {"t_identifier",  t_identifier},
        {"t_number",      t_number},
        {"t_object",      t_object}
    };
    
}

std::vector<std::pair<std::string, Token>> Lexer::lex(std::string input, Options options) {
    auto scanned = scan(input);
    
    if(options & Options::PrintScanned)
        std::cout << "\"" << scanned << "\"\n";
    
    std::vector<std::string> splitted = {}; // Delim spaces and put into a vector
    std::string line;
    std::stringstream ss(scanned);
    const char* delim = " ";
    while(std::getline(ss, line, *delim))
        splitted.push_back(line);
    
    std::vector<std::pair<std::string, Token>> toReturn;
    toReturn.push_back({"[t_start]", t_start});
    
    for(auto& i: splitted) {                // Convert input to tokens
        if(i == " " || i == "")
            continue;
        
        auto op = getOperator(i);
        if(op != t_unknown) {
            toReturn.push_back({i, op});
            continue;
        }
        auto keyword = getKeyword(i);
        if(keyword != t_unknown) {
            toReturn.push_back({i, keyword});
            continue;
        }
        if(Helper::isIdentifier(i)) {
            toReturn.push_back({i, t_identifier});
            continue;
        }
        if(Helper::isNumber(i)) {
            toReturn.push_back({i, t_number});
            continue;
        }
        
        toReturn.push_back({i, t_unknown});
    }
    
    toReturn.push_back({"[t_end]", t_end});
    return toReturn;
}

std::string Lexer::scan(std::string input) {
//    auto sorted = sort(operators);
//    printTokens(sorted);
    
    if(input == "")
        return "";
    
    auto sorted = sort(operators);
    int offset = 0;
    while(true) {               // Find and insert spaces between operators
        for(auto& op: sorted) {
            int length = (int)op.first.length();
            if(length + offset > input.length())
                continue;
            
            if(input.substr(offset, length) == op.first) {
                offset += length;
                input = input.substr(0, offset - length) + " " + op.first + " " + input.substr(offset);
                break;
            }
        }
        
        offset++;
        if(offset >= input.length())
            break;
    }
    
//    char lastChar = input[0];   // Remove all double spaces (wierd behaviour)
//    offset = 0;
//    for(auto& c: input) {
//        if(c == ' ' && lastChar == ' ')
//            input = input.substr(0, offset) + input.substr(offset + 1);
//        else
//            lastChar = c;
//        offset++;
//    }
    
    return input;
}

std::vector<std::pair<std::string, Token>> Lexer::sort(std::map<std::string, Token> tokens) {
    std::vector<std::pair<std::string, Token>> toReturn = {};
    
    while(true) {
        if(tokens.size() <= 0)
            break;
        
        std::pair<std::string, Token> longest;
        for(auto& token: tokens) {
            if(token.first.length() > longest.first.length()) {
                longest = token;
            }
        }
        toReturn.push_back(longest);
        tokens.erase(longest.first);
    }
    
    return toReturn;
}

Token Lexer::getKeyword(std::string input) {
    for(auto& i: keywords)
        if(i.first == input)
            return i.second;
    
    return t_unknown;
}

Token Lexer::getOperator(std::string op) {
    for(auto& i: operators)
        if(i.first == op)
            return i.second;
    
    return t_unknown;
}

void Lexer::printTokens(std::map<std::string, Token> tokens, int space /* = 4 */) {
    std::vector<std::pair<std::string, Token>> toPrint = {};
    for(auto& i: tokens)
        toPrint.push_back({i.first, i.second});
    
    printTokens(toPrint);
}

void Lexer::printTokens(std::vector<std::pair<std::string, Token>> tokens, int space /* = 4 */) {
    std::cout << "== Printing tokens ==\n";
    for(auto& i: tokens) {
        int currSpace = space - (int)i.first.length();
        std::cout << "    " << i.first;
        for(int s = 0; s < currSpace; s++)
            std::cout << " ";
        std::cout << "| " << i.second << "\n";
    }
    std::cout << "== Ended printing tokens ==\n\n";
}

std::string Lexer::getStringFromToken(Token token) {
    std::string r = "";
    for(auto& i: keywords)
        if(i.second == token)
            r = i.first;
    for(auto& i: types)
        if(i.second == token)
            r = i.first;
    for(auto& i: operators)
        if(i.second == token)
            r = i.first;
    if(r != "")
        r = "[" + r + "]";
    return r;
}