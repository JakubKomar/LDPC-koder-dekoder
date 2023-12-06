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
