#include <string>
#include <opencv2/opencv.hpp>

#include "matOp.hpp"

using  std::cout, std::endl, std::string; 

class decoder
{
  
public:
    decoder(){};
    ~decoder(){};
    cv::Mat extractVector(const string &input);
    cv::Mat get_message(const cv::Mat &G,const cv::Mat & r);
    string convertBinaryVectorToString(const cv::Mat &input);
    cv::Mat hardDecitonDecoder(const cv::Mat& H, const cv::Mat& input,int maxIterations);
    cv::Mat mostFuckedBitMethod(const cv::Mat& H, const cv::Mat& m, int iterations);
private:
    string formatString(const string &input);
    cv::Mat stringToMat(const string &input);
    bool checkParity(const cv::Mat& H, const cv::Mat& input);
    void debugMailBox(const std::vector< std::vector<int>> &vNodesState);
};
