#include "matrixMaker.hpp"


matrixMaker::matrixMaker(/* args */)
{
}

matrixMaker::~matrixMaker()
{
}

cv::Mat matrixMaker::makeCodingMatrix(int n_code, int d_v, int d_c, int seed)
{
    if(d_v <= 1)
        throw std::invalid_argument ("d_v must be at least 2.");

    if(d_c <= d_v)
        throw std::invalid_argument("d_c must be greater than d_v.");

    if (n_code % d_c)
        throw std::invalid_argument("d_c must divide n for a regular LDPC matrix H.");

    int n_equations = (n_code * d_v) / d_c;
    int block_size = n_equations / d_v;

    cv::Mat block(n_equations / d_v, n_code, CV_32S, cv::Scalar(0));
    cv::Mat H(n_equations, n_code, CV_32F);

    for (int i = 0; i < block_size; ++i) {
        for (int j = i * d_c; j < (i + 1) * d_c; ++j) {
            block.at<int>(i, j) = 1;
        }
    }
    block.convertTo(H.rowRange(0, block_size), CV_32F);

    // Vytvoření zbývajících bloků permutací sloupců prvního bloku
    for (int i = 1; i < d_v; ++i) {
        cv::Mat permutedBlock;
        cv::transpose(block, permutedBlock);
        std::random_shuffle(permutedBlock.begin<int>(), permutedBlock.end<int>());
        cv::transpose(permutedBlock, permutedBlock);
        permutedBlock.convertTo(H.rowRange(i * block_size, (i + 1) * block_size), CV_32F);
    }

    H.convertTo(H, CV_32S); // Převod na celočíselný formát
    return H;
}

void matrixMaker::makeDecoingMatrix()
{
    
}