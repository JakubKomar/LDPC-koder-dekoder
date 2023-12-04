#include "matrixMaker.hpp"

using std::string, std::vector, std::cout,std::cerr,std::endl;

matrixMaker::matrixMaker(/* args */)
{
}

matrixMaker::~matrixMaker()
{
}

cv::Mat matrixMaker::makeParityCheckMatrix(int n_code, int d_v, int d_c, int seed)
{
    std::default_random_engine rng(seed);

    if(d_v <= 1)
        throw std::invalid_argument ("d_v must be at least 2.");

    if(d_c <= d_v)
        throw std::invalid_argument("d_c must be greater than d_v.");

    if (n_code % d_c)
        throw std::invalid_argument("d_c must divide n for a regular LDPC matrix H.");


    int n_equations = (n_code * d_v) / d_c;

    cv::Mat block(n_equations / d_v, n_code, CV_32S, cv::Scalar(0));
   
    cv::Mat H(n_equations, n_code, CV_32F);

    int block_size = n_equations / d_v;

    for (int i = 0; i < block_size; ++i) {
        for (int j = i * d_c; j < (i + 1) * d_c; ++j) {
            block.at<int>(i, j) = 1;
        }
    }
   
    block.convertTo(H.rowRange(0, block_size), CV_32S);
    // Vytvoření zbývajících bloků permutací sloupců prvního bloku
    for (int i = 1; i < d_v; ++i) {
        cv::Mat permutedBlock;
        cv::transpose(block, permutedBlock);
        permutedBlock=shuffleBlocks(permutedBlock,&rng);
        cv::transpose(permutedBlock, permutedBlock);
        permutedBlock.convertTo(H.rowRange(i * block_size, (i + 1) * block_size), CV_32S);
    }

    H.convertTo(H, CV_32S);

    return H;
}

cv::Mat matrixMaker::makeCodingMatrix(cv::Mat H)
{
    int n_code = H.cols; 

    // DOUBLE GAUSS-JORDAN:

    auto result = gaussJordan(H.t(), true);
    cv::Mat Href_colonnes = result.first;
    cv::Mat tQ = result.second.value();

    cv::Mat Href_diag =gaussJordan(Href_colonnes.t(), false).first;


    cv::Mat Q = tQ.t();

    int n_bits = n_code - cv::sum(Href_diag)[0];

    cv::Mat Y = cv::Mat::zeros(n_code, n_bits, CV_32S);

    Y.rowRange( n_code-n_bits, n_code)=cv::Mat::eye(n_bits, n_bits, CV_32S);
    cv::Mat tG = binaryProduct(Q, Y);

    return tG;
}

cv::Mat matrixMaker::shuffleBlocks(cv::Mat matrix, std::default_random_engine * rd) {
    int rows = matrix.rows;
    int cols = matrix.cols;
    int blockSize=cols;
    std::vector<int> indices(rows);
    std::iota(indices.begin(), indices.end(), 0);

    // Náhodné zamíchání indexů

    std::shuffle(indices.begin(), indices.end(), *rd);


    // Naplnění výsledné matice permutovanými bloky
    for (int i=0; i<rows; i++) {
        auto aux = matrix.row(i).clone();
        matrix.row(indices[i]).copyTo(matrix.row(i));
        aux.copyTo(  matrix.row(indices[i]));

    }
    return matrix;
}
