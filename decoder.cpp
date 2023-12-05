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

cv::Mat decoder::get_message(cv::Mat G,cv::Mat r){

    int k = G.cols;

    auto result = matOp::gaussElimination(G, r); 
    auto rtG = result.first;
    auto rx = result.second;
    
    cv::Mat message = cv::Mat::zeros(1, k, CV_32S);

    message.at<int>(0, k - 1) = rx.at<int>(0, k - 1);
    for (int i = k - 2; i >= 0; --i) {
        message.at<int>(0, i) = rx.at<int>(0, i);
        message.at<int>(0, i) -= matOp::binaryProductInt(rtG.row(i).colRange(i + 1, k), message.colRange(i + 1, k)); 
    }
    message.forEach<int>([](int& item, const int* position) -> void {item %= 2; });
    return cv::abs(message);
}

string decoder::convertBinaryVectorToString(cv::Mat input){

    std::string result;

    for (int i = 0; i < input.cols; i+=8) {
        char c = 0;
        for (int j = 0; j < 8; ++j) {
            c += input.at<int>(0, i + j) << (7 - j);
        }
        result += c;
    }

    return result;
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


void debugMailBox(std::vector< std::vector<int>> vNodesState){
    cout<<"vNodesState:"<<endl;
    for(auto & vNode: vNodesState){
        for(auto & item: vNode){
            cout<<item<<" ";    
        }
        cout<<endl;
    }
}

bool checkParity(const cv::Mat& H, const cv::Mat& input){
    cv::Mat check= matOp::binaryProduct(H,input.t());
    if(cv::sum(check)[0]==0){
        return true;
    }
    return false;
}

// implementace na základě hard decition algoritmu z https://www.bernh.net/media/download/papers/ldpc.pdf
// úplně k ničemu 
cv::Mat decoder::hardDecitonDecoder(const cv::Mat& H, const cv::Mat& input,int maxIterations){
    
    int cNodesCount= H.cols;
    int vNodesCount= H.rows;

    cv::Mat cNodesState=input.row(0).clone();
    cv::Mat vNodesState=cv::Mat::zeros(vNodesCount,cNodesCount,CV_32S);
    std::vector< std::vector<int>> cNodeMailBox(cNodesCount, std::vector<int> (0));


    for (int i = 0; i < maxIterations; i++){
        
        if(checkParity(H,cNodesState)){
            return cNodesState;
        }

        for (int vNode_i = 0; vNode_i < vNodesCount; vNode_i++){ // step 1
            for(int cNode_i=0; cNode_i<cNodesCount;cNode_i++){ 
                if(H.at<int>(vNode_i,cNode_i)==1){
                    vNodesState.at<int>(vNode_i,cNode_i)=cNodesState.at<int>(0,cNode_i);
                }
            }        
        }
        //cout<<"vNodesState:"<<endl<<vNodesState<<endl;
     

        for(int vNode_i=0; vNode_i<vNodesCount;vNode_i++){ //step 2
            for(int cNode_i=0; cNode_i<cNodesCount;cNode_i++){ 
                if(H.at<int>(vNode_i,cNode_i)==1){
                    int aux = vNodesState.at<int>(vNode_i,cNode_i);
                    vNodesState.at<int>(vNode_i,cNode_i)=0;

                    int sum=0;
                    for(int cNode_j=0; cNode_j<cNodesCount;cNode_j++){ 
                        sum+=vNodesState.at<int>(vNode_i,cNode_j);
                    } 
                    int xorVal = sum%2; 
                    
                    cNodeMailBox.at(cNode_i).push_back(xorVal);
                    vNodesState.at<int>(vNode_i,cNode_i)=aux;
                }
            }
        }
        
        for(int cNode_i=0; cNode_i<cNodesCount;cNode_i++) //step 3         
            cNodeMailBox.at(cNode_i).push_back(cNodesState.at<int>(0,cNode_i)); 

        for(int cNode_i=0; cNode_i<cNodesCount;cNode_i++){ //step 3
            int sumOfOnes=0;
            int sumOfzeros=0;

            for(auto & item: cNodeMailBox.at(cNode_i)){
                if(item==0)
                    sumOfzeros++;
                else if(item==1)
                    sumOfOnes++;
                else    
                    assert(false);
            }
            int decitonBit = sumOfOnes>sumOfzeros?1:0;
            if(cNodesState.at<int>(0,cNode_i)!=decitonBit){
                cNodesState.at<int>(0,cNode_i)=decitonBit;
            }
            cNodeMailBox.at(cNode_i).clear();
        }
    }
    cerr<<"max iterations reached"<<endl;
    return cNodesState;

}


cv::Mat decoder::bitFlipingAlgorithm(const cv::Mat& H, const cv::Mat& input,int maxIterations){
    
    int cNodesCount= H.cols;
    int vNodesCount= H.rows;

    cv::Mat cNodesState=input.row(0).clone();
    cv::Mat vNodesStateP1=cv::Mat::zeros(vNodesCount,cNodesCount,CV_32F);
    cv::Mat vNodesStateP2=cv::Mat::zeros(vNodesCount,cNodesCount,CV_32F);
    std::vector< std::vector<int>> cNodeMailBox(cNodesCount, std::vector<int> (0));


    for (int i = 0; i < maxIterations; i++){
        
        if(checkParity(H,cNodesState)){
            return cNodesState;
        }


    }
    cerr<<"max iterations reached"<<endl;
    return cNodesState;

}

cv::Mat decoder::ldpcCorrection(const cv::Mat& H, const cv::Mat& m, int iterations) {
    cv::Mat correctedMessage = m.clone();

    for (int iter = 0; iter < iterations; ++iter) {
        // Syndrome calculation
        cv::Mat syndrome = matOp::binaryProduct(H,correctedMessage.t());
        cout << "Syndrome: " << syndrome << endl;
        // Check if the syndrome is all zeros (no errors)
        if (countNonZero(syndrome) == 0) {
            cout << "No errors found. Iteration: " << iter + 1 << endl;
            break;
        }

        // Error correction
        for (int i = 0; i < syndrome.rows; ++i) {
            if (countNonZero(H.row(i) == syndrome.row(i)) == H.cols) {
                // Syndrome row matches a row in H, so correct the corresponding bit
                correctedMessage.at<int>(0, i) = 1 - correctedMessage.at<int>(0, i);
            }
        }

        cout << "Iteration: " << iter + 1 << endl;
        cout << "Corrected Message: " << correctedMessage << endl;
    }

    return correctedMessage;
}

// good for 1 error
cv::Mat decoder::mostFuckedBitMethod(const cv::Mat& H, const cv::Mat& m, int iterations) {
    cv::Mat correctedMessage = m.clone();
    
    for (int iter = 0; iter < iterations; ++iter) {
        cv::Mat syndrome = matOp::binaryProduct(H,correctedMessage.t());

        if (countNonZero(syndrome) == 0) {
            break;
        }

        cv::Mat counters = cv::Mat::zeros(1,m.cols,CV_32S);

        for(int i=0;i<H.rows;i++)
            if(syndrome.at<int>(i,0))
                for(int j=0;j<H.cols;j++)
                    if(H.at<int>(i,j))
                        counters.at<int>(0,j)+=1;
        
        cout<<counters;
        cv::Point maxLoc;
        cv::minMaxLoc(counters, NULL, NULL, NULL, &maxLoc);

        int bitForSwich = maxLoc.x;
        cout<<bitForSwich;
        correctedMessage.at<int>(0,bitForSwich)=!correctedMessage.at<int>(0,bitForSwich);    
        break; 
    }
    return correctedMessage;
}
/*
cv::Mat decode(const cv::Mat& H, const cv::Mat& y, int maxiter) {
    int m = H.rows;
    int n = H.cols;

    // Extract bits_hist, bits_values, nodes_hist, nodes_values from utils
    // ...

    cv::Mat Lc = y * 2.5;
    int n_messages = y.cols;

    cv::Mat Lq(m, n, CV_32FC(n_messages), cv::Scalar(0));
    cv::Mat Lr(m, n, CV_32FC(n_messages), cv::Scalar(0));

    for (int n_iter = 0; n_iter < maxiter; ++n_iter) {
        logbp_numba(bits_hist, bits_values, nodes_hist, nodes_values, Lc, Lq, Lr, n_iter);

        cv::Mat x;
        cv::compare(Lr <= 0, cv::Mat::zeros(Lr.size(), Lr.type()), x, cv::CMP_EQ);
        x.convertTo(x, CV_32F);

        // Call utils.incode(H, x) and check the product
        // ...

        if (product) {
            break;
        }
    }

    return x.reshape(0, 1);
}

cv::Mat logbp_numba(const cv::Mat& bits_hist, const cv::Mat& bits_values,
                    const cv::Mat& nodes_hist, const cv::Mat& nodes_values,
                    const cv::Mat& Lc, cv::Mat& Lq, cv::Mat& Lr, int n_iter) {
    int m = Lr.rows;
    int n = Lr.cols;
    int n_messages = Lr.channels();

    int bits_counter = 0;
    int nodes_counter = 0;

    for (int i = 0; i < m; ++i) {
        // Process Horizontal
        int ff = bits_hist.at<int>(i);
        cv::Mat ni = bits_values.rowRange(bits_counter, bits_counter + ff);
        bits_counter += ff;

        for (int j = 0; j < ni.cols; ++j) {
            cv::Mat nij = ni.clone();

            cv::Mat X(n_messages, 1, CV_32F, cv::Scalar(1.0));
            if (n_iter == 0) {
                for (int kk = 0; kk < nij.cols; ++kk) {
                    if (nij.at<int>(0, kk) != j) {
                        X *= tanh(0.5 * Lc.at<float>(nij.at<int>(0, kk), 0));
                    }
                }
            } else {
                for (int kk = 0; kk < nij.cols; ++kk) {
                    if (nij.at<int>(0, kk) != j) {
                        X *= tanh(0.5 * Lq.at<float>(i, nij.at<int>(0, kk), 0));
                    }
                }
            }

            cv::Mat num, denom;
            cv::add(X, 1, num);
            cv::subtract(1, X, denom);

            for (int ll = 0; ll < n_messages; ++ll) {
                if (num.at<float>(ll, 0) == 0) {
                    Lr.at<float>(i, j, ll) = -1;
                } else if (denom.at<float>(ll, 0) == 0) {
                    Lr.at<float>(i, j, ll) = 1;
                } else {
                    Lr.at<float>(i, j, ll) = log(num.at<float>(ll, 0) / denom.at<float>(ll, 0));
                }
            }
        }
    }

    // Process Vertical
    for (int j = 0; j < n; ++j) {
        int ff = nodes_hist.at<int>(j);
        cv::Mat mj = nodes_values.rowRange(nodes_counter, nodes_counter + ff);
        nodes_counter += ff;

        for (int i = 0; i < mj.cols; ++i) {
            cv::Mat mji = mj.clone();
            Lq.at<float>(mji.at<int>(0, i), j, 0) = Lc.at<float>(0, j);

            for (int kk = 0; kk < mji.cols; ++kk) {
                if (mji.at<int>(0, kk) != i) {
                    Lq.at<float>(mji.at<int>(0, kk), j, 0) += Lr.at<float>(mji.at<int>(0, kk), j, 0);
                }
            }
        }
    }

    // LLR a posteriori
    cv::Mat L_posteriori(n, n_messages, CV_32F, cv::Scalar(0));
    nodes_counter = 0;

    for (int j = 0; j < n; ++j) {
        int ff = nodes_hist.at<int>(j);
        cv::Mat mj = nodes_values.rowRange(nodes_counter, nodes_counter + ff);
        nodes_counter += ff;

        for (int ll = 0; ll < n_messages; ++ll) {
            L_posteriori.at<float>(j, ll) = Lc.at<float>(0, j) + cv::sum(Lr.row(mj.at<int>(0, 0))).val[0];
        }
    }

    return Lq;
}
*/