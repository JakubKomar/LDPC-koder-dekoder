#include <stdexcept>
#include "opencv2/opencv.hpp"
#include <random>
#include <optional>

using std::string, std::vector, std::cout,std::cerr,std::endl;

std::pair<cv::Mat, std::optional<cv::Mat>> gaussJordan(cv::Mat X, bool change );
cv::Mat binaryProduct(cv::Mat X, cv::Mat Y);
cv::Mat shityMatrixMul( cv::Mat X, cv::Mat Y);
