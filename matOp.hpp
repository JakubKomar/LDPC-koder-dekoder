#include <stdexcept>
#include "opencv2/opencv.hpp"
#include <random>
#include <optional>

std::pair<cv::Mat, std::optional<cv::Mat>> gaussJordan(cv::Mat X, bool change );
cv::Mat binaryProduct(cv::Mat X, cv::Mat Y);