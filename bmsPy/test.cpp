#include <iostream>
#include <vector>
#include <string>
#include "opencv2/opencv.hpp"
using std::string, std::vector, std::cout,std::cerr,std::endl;
cv::Mat shityMatrixMul(const cv::Mat X, const cv::Mat Y){

    
    cv::Mat result(1,X.rows, CV_32S, cv::Scalar(0));
    cout<<"X"<<endl<<X<<endl;
    cout<<"Y"<<endl<<Y<<endl;
    cout<<"result"<<endl<<result<<endl;
    for (auto i = 0; i < X.rows; ++i) {
        for (auto j = 0; j < X.cols; ++j) {
            result.at<int>(0, i) += X.at<int>(i, j) * Y.at<int>(j, 0);
        }
    }/*
    result.forEach<int>([](int& item, const int* position) -> void {
        item %= 2;
    });*/
   
    return result;
}

std::vector<int> matrixMultiplication(const std::vector<std::vector<int>>& X, const std::vector<int>& Y) {
    std::vector<int> result(X.size(), 0);

    for (size_t i = 0; i < X.size(); ++i) {
        for (size_t j = 0; j < X[i].size(); ++j) {
            result[i] += X[i][j] * Y[j];
        }
    }

    return result;
}

int main() {
    cv::Mat X = (cv::Mat_<int>(16, 4) <<
        1, 1, 0, 
        1, 0, 0, 
        0, 0, 0, 
        0, 0, 1, 
        0, 0, 0, 
        1, 0, 0, 
        1, 1, 0, 
        0, 0, 1, 
        0, 1, 1, 
        0, 1, 0, 
        0, 0, 1, 
        0, 0, 0, 
        0, 0, 0, 
        0, 0, 0, 
        0, 0, 0,  
        0, 0, 0
    );

    // Definice matice Y
    cv::Mat Y = (cv::Mat_<int>(8, 1) << 0, 1, 1, 0, 0, 0, 0, 1);
    // Maticové násobení
    auto result = shityMatrixMul(X, Y);

    std::cout << "Result: " << result<<std::endl;
    return 0;
}