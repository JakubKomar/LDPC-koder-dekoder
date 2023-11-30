#include <stdexcept>
#include "opencv2/opencv.hpp"
#include <random>

class matrixMaker
{
private:
    /* data */
public:
    matrixMaker(/* args */);
    cv::Mat  makeParityCheckMatrix(int n_code, int d_v, int d_c, int seed=0);
    void makeDecoingMatrix();
    ~matrixMaker();
};
