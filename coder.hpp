/**
 * @file coder.hpp
 * @brief Deklarace třídy coder pro LDPC kódování a dekódování.
 *
 * Tato třída poskytuje metody pro kódování binárních vektorů
 * pomocí LDPC kódů. Implementuje operace, jako je zakódování, 
 * formátování řetězců pro vstup do třídy a další související funkce pro práci s LDPC kódy.
 *
 * @author Bc. Jakub Komárek
 * @year 2023
 */
#pragma once

#include <string>
#include <bitset>
#include <opencv2/opencv.hpp>

#include "matOp.hpp"

using  std::cout, std::endl, std::string;

class coder
{
public:
    coder(){};
    ~coder(){};
    cv::Mat stringToBinary(const string &input);
    cv::Mat encode(const cv::Mat &G, const cv::Mat &v);
    string matToString(const cv::Mat &X);
    
private:
    string formatString(const string &input);
};
