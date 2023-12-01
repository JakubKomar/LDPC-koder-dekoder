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
   
    block.convertTo(H.rowRange(0, block_size), CV_32F);
   
    // Vytvoření zbývajících bloků permutací sloupců prvního bloku
    for (int i = 1; i < d_v; ++i) {
        cv::Mat permutedBlock;
        cv::transpose(block, permutedBlock);
        std::shuffle(permutedBlock.begin<int>(), permutedBlock.end<int>(),rng);
        cv::transpose(permutedBlock, permutedBlock);
        permutedBlock.convertTo(H.rowRange(i * block_size, (i + 1) * block_size), CV_32F);
    }

    H.convertTo(H, CV_32S);

    return H;
}

cv::Mat matrixMaker::makeCodingMatrix(cv::Mat H)
{
    cout<<"H:"<<endl<<H<<endl;


    int n_code = H.cols; 

    // DOUBLE GAUSS-JORDAN:

    auto result = gaussJordan(H.t(), true);
    cv::Mat Href_colonnes = result.first;
    cv::Mat tQ = result.second.value();

    cout<<"Href_colonnes:"<<endl<<Href_colonnes<<endl;
    cout<<"Tq:"<<endl<<tQ<<endl;


    cv::Mat Href_diag =gaussJordan(Href_colonnes.t(), false).first;
    cout<<"Href_diag:"<<endl<<Href_diag<<endl;


    cv::Mat Q = tQ.t();
    cout<<"Q:"<<endl<<Q<<endl;


    cout<<"sum"<<endl<<cv::sum(Href_diag)<<endl;


    int n_bits = n_code - cv::sum(Href_diag)[0];

    cv::Mat Y = cv::Mat::zeros(n_code, n_bits, CV_32S);

    Y.rowRange( n_code-n_bits, n_code)=cv::Mat::eye(n_bits, n_bits, CV_32S);
    cout<<"Y:"<<endl<<Y<<endl;
    cv::Mat tG = binaryProduct(Q, Y);

    return tG;
}

std::pair<cv::Mat, std::optional<cv::Mat>> matrixMaker::gaussJordan(cv::Mat H, bool change = false)
{
    //cout<<"H:"<<endl<<H<<endl;
    std::optional<cv::Mat> P_return = std::nullopt;

    cv::Mat A = H.clone(); 

    int m = A.rows;
    int n = A.cols;

    cv::Mat P; // permutation matrix
    if (change) {
        P = cv::Mat::eye(m, m, CV_32S);
    }

    int pivotOld = -1; 
    for (int j = 0; j < n; ++j) { 

        cv::Mat filterDown = A.rowRange(pivotOld + 1, m).col(j).t(); 

        //cout<<"filterDown:"<<endl<<filterDown<<endl;
        cv::Point maxLoc;
        cv::minMaxLoc(filterDown, NULL, NULL, NULL, &maxLoc);

        int pivot = maxLoc.x + pivotOld + 1;
        int value=A.at<int>(pivot, j);
        //cout<<"A.at<int>(pivot, j):"<<A.at<int>(pivot, j)<<endl;

        if (A.at<int>(pivot, j) != 0) {
            pivotOld += 1;

            if (pivotOld != pivot) {
                
                cv::Mat aux = A.row(pivot).clone();
                A.row(pivot).copyTo(A.row(pivotOld));
                aux.copyTo(A.row(pivot));

                if (change) {
                    auto aux = P.row(pivot).clone();
                    P.row(pivot).copyTo(P.row(pivotOld));
                    aux.copyTo(P.row(pivot));
                }
            }

            for (int i = 0; i < m; ++i) {
                if (i != pivotOld && A.at<int>(i, j) != 0) {
                    if (change) {
                        P.row(i) = cv::abs(P.row(i) - P.row(pivotOld));
                    }
                    A.row(i) = cv::abs(A.row(i) - A.row(pivotOld));
                }
            }
        }
        //cout<<"A:"<<endl<<A<<endl;
        if (pivotOld == m - 1) {
            break;
        }
        
    }

    if (change) {
        P_return = P;
    }
    return std::make_pair(A, P_return);

}

cv::Mat matrixMaker::binaryProduct(cv::Mat X,  cv::Mat Y) {
    CV_Assert(X.type() == CV_32S && Y.type() == CV_32S && X.cols == Y.rows);
    CV_Assert(X.cols == Y.rows);

    X.convertTo(X, CV_32F);
    Y.convertTo(Y, CV_32F);
    cout<<"X:"<<endl<<X<<endl;
    cout<<"Y:"<<endl<<Y<<endl;

    cv::Mat A=X*Y;
    cout<<"A:"<<endl<<A<<endl;
    A.convertTo(A, CV_32S);

    // Perform modulo 2 arithmetic
    A.forEach<int>([](int& item, const int* position) -> void {
        item %= 2;
    });
    cout<<"A:"<<endl<<A<<endl;
    

    return A;
}