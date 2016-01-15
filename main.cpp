//
//  main.cpp
//  StBernandScript
//
//  Created by Didrik Munther on 12/01/16.
//  Copyright © 2016 Didrik Munther. All rights reserved.
//

#include <iostream>

#include "StBernand.hpp"
#include "Options.hpp"


//int test() {
//    std::cout << "test\n";
//    
//    return 2;
//}
//    void* thing = (void*)&test;
//    ((int(*)())thing)();

int main(int argc, const char * argv[]) {
    Options::_Options = (Options::Option)((int)Options::Option::PrintErrPosition |
                                          (int)Options::Option::PrintExpected); // Default options
    
    StBernand sb;
    return sb.start();
}
