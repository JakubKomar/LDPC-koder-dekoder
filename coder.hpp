
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
