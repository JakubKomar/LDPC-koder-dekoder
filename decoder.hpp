
#include <iostream>
#include <string>
#include <bitset>
#include <opencv2/opencv.hpp>
#include "matOp.hpp"

using  std::cout, std::endl, std::string; 

class decoder
{
  
public:
    decoder();
    ~decoder();
    cv::Mat extractVector(std::string input);
    cv::Mat decodeHardDecision(cv::Mat H, cv::Mat input, int maxIterations) ;
    cv::Mat get_message(cv::Mat G,cv::Mat input);
    string convertBinaryVectorToString(cv::Mat input);
    cv::Mat hardDecitonDecoder(const cv::Mat& H, const cv::Mat& input,int maxIterations);
    cv::Mat bitFlipingAlgorithm(const cv::Mat& H, const cv::Mat& input,int maxIterations);
    cv::Mat ldpcCorrection(const cv::Mat& H, const cv::Mat& m, int iterations);
    cv::Mat mostFuckedBitMethod(const cv::Mat& H, const cv::Mat& m, int iterations);
private:
    string formatString(string input);
    cv::Mat stringToMat(string X);
};
