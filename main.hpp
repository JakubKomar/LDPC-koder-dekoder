#include <iostream>
#include <opencv2/opencv.hpp>

#include "matrixMaker.hpp"
#include "coder.hpp"
#include "decoder.hpp"

using
    std::cout, 
    std::endl, 
    std::string, 
    std::cin,
    std::getline,
    std::ends
; 

enum mode {
    ENCODE,
    DECODE,
    NOTSET
};

void printhelp();
void test();