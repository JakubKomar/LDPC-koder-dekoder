#include <stdexcept>
#include "opencv2/opencv.hpp"
#include <random>
#include <optional>
#include <algorithm>
using std::string, std::vector, std::cout,std::cerr,std::endl;

std::pair<cv::Mat, std::optional<cv::Mat>> gaussJordan(cv::Mat X, bool change );
cv::Mat binaryProduct(cv::Mat X, cv::Mat Y);
cv::Mat shityMatrixMul( cv::Mat X, cv::Mat Y);


std::pair<cv::Mat, cv::Mat> gaussElimination(cv::Mat A, cv::Mat b);