
#include <iostream>
#include <string>
#include <bitset>
#include <opencv2/opencv.hpp>
#include "matOp.hpp"

using  std::cout, std::endl, std::string; // using declaration

class coder
{
private:
public:
    coder();
    ~coder();
    cv::Mat stringToBinary(std::string input);
    cv::Mat encode(cv::Mat input, cv::Mat G);
    string matToString(cv::Mat X);
    
    
private:
    string formatString(std::string input);
};
