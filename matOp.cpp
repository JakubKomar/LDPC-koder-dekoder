#include "matOp.hpp"

std::pair<cv::Mat, std::optional<cv::Mat>> gaussJordan(cv::Mat H, bool change = false)
{
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

    if (change) {
        P_return = P;
    }
    return std::make_pair(A, P_return);
}

cv::Mat binaryProduct(cv::Mat X,  cv::Mat Y) {

    X.convertTo(X, CV_32F);
    Y.convertTo(Y, CV_32F);

    cv::Mat A=X*Y;
    A.convertTo(A, CV_32S);

    // Perform modulo 2 arithmetic
    A.forEach<int>([](int& item, const int* position) -> void {
        item %= 2;
    });
   
    return A;
}