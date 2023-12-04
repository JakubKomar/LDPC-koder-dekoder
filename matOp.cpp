#include "matOp.hpp"

namespace matOp{
    
std::pair<cv::Mat, std::optional<cv::Mat>> gaussJordan(cv::Mat H, bool change = false)
{
    std::optional<cv::Mat> P_return = std::nullopt;

    cv::Mat A = H.clone(); 

    int m = A.rows;
    int n = A.cols;

    cv::Mat P; 
    if (change) 
        P = cv::Mat::eye(m, m, CV_32S);

    int pivotOld = -1; 
    for (int j = 0; j < n; ++j) { 

        cv::Mat filterDown = A.rowRange(pivotOld + 1, m).col(j).t(); 

        cv::Point maxLoc;
        cv::minMaxLoc(filterDown, NULL, NULL, NULL, &maxLoc);

        int pivot = maxLoc.x + pivotOld + 1;

        if (A.at<int>(pivot, j) != 0) {
            pivotOld += 1;

            if (pivotOld != pivot) {
                cv::Mat aux = A.row(pivot).clone();
                A.row(pivotOld).copyTo(A.row(pivot));
                aux.copyTo(A.row(pivotOld));

                if (change) {
                    auto aux = P.row(pivot).clone();
                    P.row(pivotOld).copyTo(P.row(pivot));
                    aux.copyTo(P.row(pivotOld));
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
        if (pivotOld == m - 1) {
            break;
        }
    }

    if (change) 
        P_return = P;
    return std::make_pair(A, P_return);
}

cv::Mat binaryProduct(cv::Mat X,  cv::Mat Y) {

    X.convertTo(X, CV_32F); // protože X*Y; bere pouze floaty z nějakého důvodu
    Y.convertTo(Y, CV_32F);

    cv::Mat A=X*Y;
    A.convertTo(A, CV_32S);

    A.forEach<int>([](int& item, const int* position) -> void {item %= 2; });

    return A;
}

int binaryProductInt(const cv::Mat& a, const cv::Mat& b) {   
    auto result = matOp::matrixMulNoRestrictions(a, b.t());
    
    return result.at<int>(0, 0) %2;
}


cv::Mat matrixMulNoRestrictions( cv::Mat X, cv::Mat Y){

    cv::Mat result(1,X.rows, CV_32S, cv::Scalar(0));

    for (auto i = 0; i < X.rows; ++i) {
        for (auto j = 0; j < X.cols; ++j) {
            result.at<int>(0, i) += X.at<int>(i, j) * Y.at<int>(j, 0);
        }
    }

    return result;
}

std::pair<cv::Mat, cv::Mat> gaussElimination(cv::Mat A, cv::Mat b){
    
    int n = A.rows;
    int k = A.cols;

    for (int j = 0; j < std::min(k, n); ++j) {
        std::vector<int> listDepivots;
        for (int i = j; i < n; ++i) {
            if (A.at<int>(i, j)) {
                listDepivots.push_back(i);
            }
        }

        int pivot;
        if (!listDepivots.empty()) 
            pivot = *std::min_element(listDepivots.begin(), listDepivots.end());      
        else
            continue;


        if (pivot != j) {
            auto aux = A.row(j).clone();
            A.row(pivot).copyTo(A.row(j));
            aux.copyTo(A.row(pivot));

            int aux1 = b.at<int>(j);
            b.at<int>(j) = b.at<int>(pivot);
            b.at<int>(pivot) = aux1;

        }

        for (int i = j + 1; i < n; ++i) {
            if (A.at<int>(i, j)) {
                A.row(i) = cv::abs(A.row(i) - A.row(j));
                b.at<int>(i) = std::abs(b.at<int>(i) - b.at<int>(j));
            }
        }

    }
    return std::make_pair(A, b);
    
}
}