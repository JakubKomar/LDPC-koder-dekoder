#include <stdexcept>
#include <opencv2/opencv.hpp>
#include <random>
#include <optional>
#include <fstream>
#include "matOp.hpp"

class matrixMaker
{
private:

public:
    matrixMaker();
    ~matrixMaker();
    cv::Mat makeParityCheckMatrix(int n_code, int d_v, int d_c, int seed=0);
    cv::Mat makeCodingMatrix(cv::Mat H);
    cv::Mat matrixFromFile(string path);
    void saveMatrixToCSV(const cv::Mat& matrix, const std::string& filePath) ;
private:
    cv::Mat shuffleBlocks(cv::Mat matrix, std::default_random_engine *rd) ;
};
