//
//  Lexer.hpp
//  StBernandScript
//
//  Created by Didrik Munther on 12/01/16.
//  Copyright © 2016 Didrik Munther. All rights reserved.
//

#ifndef Lexer_hpp
#define Lexer_hpp

#include <stdio.h>
#include <map>
#include <vector>
#include <string>


enum Token {
    /* Errors */
    
    t_error         = -100,
    t_unknown,
    t_null,
    
    /* Things */
    
    t_start = 0,
    t_end,
    t_identifier,
    t_number,
    t_object,
    
    /* Types */
    
    t_type_number   = 100,
    t_type_bool,
    t_type_string,
    
    /* Macros */
    
    t_macro = 200,
    
    /* Operators */
    
    t_forw_slash    = 300,
    t_back_slash,
    
    t_colon,
    t_semicolon,
    
    t_dot,
    t_comma,
    
    t_parenthesis_open,
    t_parenthesis_close,
    t_bracket_open,
    t_bracket_close,
    t_brace_open,
    t_brace_close,
    
    t_equals,
    t_plus,
    t_minus,
    t_asterix,
    t_modulo,
    
    t_add_assign,
    t_sub_assign,
    t_div_assign,
    t_mult_assign,
    t_mod_assign,
    
    t_not,
    t_equal_to,
    t_not_equal_to,
    t_less_than,
    t_greater_than,
    t_less_than_or_equal,
    t_greater_than_or_equal,
    
};

class Lexer {
    
public:
    Lexer();
    
    enum Options {
        Lex             = 0,
        PrintScanned    = 1 << 1,
        PrintTokens     = 1 << 2,
    };

    std::vector<std::pair<std::string, Token>> lex(std::string input, Options options = Lex);
    
    void printTokens(std::map<std::string, Token> tokens, int space = 4);
    void printTokens(std::vector<std::pair<std::string, Token>> tokens, int space = 4);
    
private:
    std::string scan(std::string input);
    std::vector<std::pair<std::string, Token>> sort(std::map<std::string, Token> tokens);
    
    bool isAllowedString(std::string allowed, std::string input);
    bool isIdentifier(std::string input);
    bool isNumber(std::string input);
    Token getKeyword(std::string input);
    Token getOperator(std::string op);
    
    
    template<typename Vec, typename Val>
    void removeFromVector(Vec& vec, Val val) {
        vec.erase(std::remove(vec.begin(), vec.end(), val), vec.end());
    }
    
    std::map<std::string, Token> keywords;
    std::map<std::string, Token> operators;
    
};

#endif /* Lexer_hpp */
