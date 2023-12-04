#include "decoder.hpp"

decoder::decoder()
{
}

decoder::~decoder()
{
}

cv::Mat decoder::extractVector(string input){
    string formatedInput= formatString(input);
    auto mat= this->stringToMat(formatedInput);

    return mat;
}

string decoder::formatString(string input){
    string output;
    for (auto & c: input){
        if(c=='0' || c=='1'){
            output+=c;
        }
    }
    return output;
}

cv::Mat decoder::stringToMat(string X){
    cv::Mat output;
    
    for (auto & c: X){
        output.push_back(c=='0'?0:1);
    }

    return output.t();
}

int binaryProduct2(const cv::Mat& a, const cv::Mat& b) {
    int result = 0;

    for (int i = 0; i < a.cols; ++i) {
        result += a.at<int>(0, i) * b.at<int>(0, i);
    }

    return result;
}

cv::Mat decoder::get_message(cv::Mat G,cv::Mat r){
    int n = G.rows;
    int k = G.cols;

    auto result = matOp::gaussElimination(G, r); 
    auto rtG = result.first;
    auto rx = result.second;

    cv::Mat message = cv::Mat::zeros(1, k, CV_32S);

    message.at<int>(0, k - 1) = rx.at<int>(0, k - 1);
    for (int i = k - 2; i >= 0; --i) {
        message.at<int>(0, i) = rx.at<int>(0, i);
        message.at<int>(0, i) -= binaryProduct2(rtG.row(i).colRange(i + 1, k), message.colRange(i + 1, k)); 
    }

    // Convert to absolute values
    cv::Mat absMessage;
    cv::absdiff(message, cv::Scalar::all(0), absMessage);

    return absMessage;
}

cv::Mat decoder::decodeHardDecision(cv::Mat H, cv::Mat input, int maxIterations = 50) {
    cv::Mat L = input.clone();

    for (int iteration = 0; iteration < maxIterations; ++iteration) {
        // Horizontal step (check to variable node messages)
        cv::Mat messagesHV = H * L;

        // Vertical step (variable to check node messages)
        cv::Mat messagesVH = H.t() * messagesHV;

        // Update variable node information
        L = input + messagesVH;

        // Perform hard decision on the current soft information
        cv::threshold(L, L, 0, 1, cv::THRESH_BINARY);
    }

    return L;
}


