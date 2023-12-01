#include <stdexcept>
#include "opencv2/opencv.hpp"
#include <random>
#include <optional>

class matrixMaker
{
private:
    /* data */
public:
    matrixMaker(/* args */);
    cv::Mat makeParityCheckMatrix(int n_code, int d_v, int d_c, int seed=0);
    cv::Mat makeCodingMatrix(cv::Mat H);
    std::pair<cv::Mat, std::optional<cv::Mat>> gaussJordan(cv::Mat X, bool change ) ;
    cv::Mat binaryProduct(cv::Mat X, cv::Mat Y);
    ~matrixMaker();
};
