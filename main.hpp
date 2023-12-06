/**
 * @file main.hpp
 * @brief main deklarace pro ldpc kódování a dekódování.
 * 
 * @author Bc. Jakub Komárek
 * @year 2023
 */
#pragma once

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

#define HARD_DECITION 0
#define MOST_CONFLICT_BIT 1


void printhelp();
void test();