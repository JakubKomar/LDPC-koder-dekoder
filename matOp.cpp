#include "matOp.hpp"

namespace matOp{
    
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

    cv::Mat shityMatrixMul( cv::Mat X, cv::Mat Y){

        cv::Mat result(1,X.rows, CV_32S, cv::Scalar(0));

        for (auto i = 0; i < X.rows; ++i) {
            for (auto j = 0; j < X.cols; ++j) {
                result.at<int>(0, i) += X.at<int>(i, j) * Y.at<int>(j, 0);
            }
        }

        return result;
    }

    std::pair<cv::Mat, cv::Mat> gaussElimination(cv::Mat A, cv::Mat b){

        cv::Mat A_r = A.clone();
        cv::Mat b_r = b.clone();

        int n = A_r.rows;
        int k = A_r.cols;
                cout<<"A_r\n"<<A_r<<endl;
            cout<<"b_r\n"<<b_r<<endl;
        for (int j = 0; j < std::min(k, n); ++j) {
            std::vector<int> listDepivots;
            for (int i = j; i < n; ++i) {
                if (A_r.at<int>(i, j)) {
                    listDepivots.push_back(i);
                }
            }

            int pivot;
            if (!listDepivots.empty()) 
                pivot = *std::min_element(listDepivots.begin(), listDepivots.end());      
            else
                continue;


            if (pivot != j) {
                auto aux = A_r.row(j).clone();
                A_r.row(pivot).copyTo(A_r.row(j));
                aux.copyTo(A_r.row(pivot));

                int aux1 = b.at<int>(j);
                b.at<int>(j) = b.at<int>(pivot);
                b.at<int>(pivot) = aux1;

            }

            for (int i = j + 1; i < n; ++i) {
                if (A_r.at<int>(i, j)) {
                    A_r.row(i) = cv::abs(A_r.row(i) - A_r.row(j));
                    b_r.at<int>(i) = cv::abs(b_r.at<int>(i) - b_r.at<int>(j));
                }
            }
            cout<<"A_r\n"<<A_r<<endl;
            cout<<"b_r\n"<<b_r<<endl;
        }
        return std::make_pair(A_r, b_r);
    }
}