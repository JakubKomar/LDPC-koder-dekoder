/**
 * @file marixMaker.hpp
 * @brief dektlarace specifických maticových operací.
 *
 * Tato třída poskytuje deklarace metod pro vytváření matic pro LDPC kódování a dekódování. 
 * Dále obsahuje metody pro ukládání těchto matic do souborů a načítání z nich.
 *
 * @author Bc. Jakub Komárek
 * @year 2023
 */
#pragma once

#include <stdexcept>
#include <opencv2/opencv.hpp>
#include <random>
#include <optional>
#include <fstream>

#include "matOp.hpp"

using std::string, std::vector, std::cout,std::cerr,std::endl;

class matrixMaker
{
public:
    matrixMaker(){};
    ~matrixMaker(){};
    cv::Mat makeParityCheckMatrix(const int n_code, const int d_v,const  int d_c, const int seed=0);
    cv::Mat makeCodingMatrix(const cv::Mat &H);
    cv::Mat matrixFromFile(const string &filePath);
    void saveMatrixToCSV(const cv::Mat& matrix, const std::string& filePath) ;
private:
    cv::Mat shuffleBlocks(cv::Mat matrix, std::default_random_engine *rd) ;
};
