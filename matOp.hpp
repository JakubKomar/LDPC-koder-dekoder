/**
 * @file matOp.cpp
 * @brief dektlarace specifických maticových operací.
 *
 * Tento soubor poskytuje specifické maticové operace pro potřeby LDPC kódování a dekódování.
 *
 * @author Bc. Jakub Komárek
 * @year 2023
 */
#pragma once

#include <stdexcept>
#include <opencv2/opencv.hpp>
#include <random>
#include <optional>
#include <algorithm>

using std::string, std::vector, std::cout,std::cerr,std::endl;

namespace matOp{

    std::pair<cv::Mat, std::optional<cv::Mat>> gaussJordan(const cv::Mat &H, bool change);

    cv::Mat binaryProduct(cv::Mat X, cv::Mat Y);

    int binaryProductInt(const cv::Mat& a, const cv::Mat& b);

    cv::Mat matrixMulNoRestrictions(const cv::Mat &X, const cv::Mat &Y);
    
    cv::Mat matrixMulInt(cv::Mat X, cv::Mat Y) ;

    std::pair<cv::Mat, cv::Mat> gaussElimination(cv::Mat A, cv::Mat b);
}