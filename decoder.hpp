
#include <iostream>
#include <string>
#include <bitset>
#include "opencv2/opencv.hpp"
#include "matOp.hpp"

using  std::cout, std::endl, std::string; // using declaration

class decoder
{
private:
  
public:
    decoder();
    ~decoder();
    cv::Mat extractVector(std::string input);
    cv::Mat decode(cv::Mat H,cv::Mat input, int max_iter=1000);
    cv::Mat get_message(cv::Mat G,cv::Mat input);
private:
    string formatString(string input);
    cv::Mat stringToMat(string X);
};
