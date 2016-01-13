//
//  Options.hpp
//  StBernandScript
//
//  Created by Didrik Munther on 13/01/16.
//  Copyright © 2016 Didrik Munther. All rights reserved.
//

#ifndef Options_hpp
#define Options_hpp

#include <stdio.h>


struct Options {
    
    enum Option {
        None                        = 0,
        PrintErrPosition            = 1 << 1,
        PrintExpected               = 1 << 2,
        PrintStackAfterExecution    = 1 << 3,
        PrintStackDuringExecution   = 1 << 4,
        PrintTokensBeforeShuffle    = 1 << 5,
        PrintTokensAfterShuffle     = 1 << 6,
        
    };
    
    static Option _Options;
    
    static void add(Options::Option option) {
        int temp = (int)Options::_Options | (int)option;
        Options::_Options = (Options::Option)temp;
    }
    
    static void remove(Options::Option option) {
        int temp = (int)Options::_Options & ~(int)option;
        Options::_Options = (Options::Option)temp;
    }
    
    static void toggle(Options::Option option) {
        int temp = (int)Options::_Options ^ (int)option;
        Options::_Options = (Options::Option)temp;
    }
    
    static bool has(Options::Option option) {
        return (int)Options::_Options & (int)option;
    }
    
};

#endif /* Options_hpp */
