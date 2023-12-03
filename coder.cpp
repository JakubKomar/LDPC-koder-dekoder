#include "coder.hpp"

coder::coder()
{
}

coder::~coder()
{
}


 cv::Mat coder::stringToBinary(string input){
    input= formatString(input);
    
    cv::Mat binaryVector(1, input.length()*8, CV_32S);

    int charCounter = 0;
    for (char c : input) {
        std::bitset<8> binaryRepresentation(c);

        for (int i = 7; i >=0; --i) {
            binaryVector.at<int>(0, charCounter*8 +7-i) = binaryRepresentation[i]?1:0;
        }
        charCounter++;
    }

    return binaryVector;
}


string coder::formatString(string input){
    string output;

    for (auto & c: input){
        if((c>='a' && c<='z') || (c>='A'&&c<='Z')|| (c>='0'&&c<='9')){
            output+=c;
        }
    }

    return output;
}

 cv::Mat coder::encode(cv::Mat G, cv::Mat v){
    cv::Mat d = shityMatrixMul(G, v.t());	

    d.forEach<int>([](int& item, const int* position) -> void {
        item %= 2;
    });

    return d;
 }

 string coder::matToString(cv::Mat X){
    std::stringstream ss;
    
    for (int i = 0; i < X.rows; ++i) {
        for (int j = 0; j < X.cols; ++j) {
            ss << X.at<int>(i, j);
        }
    }

    return ss.str();
 }