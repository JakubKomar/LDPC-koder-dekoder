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

// implementation via https://www.bernh.net/media/download/papers/ldpc.pdf
// useles, 6 hours of my life wasted
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


// good for 1 error, but better that hard decision piece of horse shit
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
        
        //cout<<counters;
        cv::Point maxLoc;
        cv::minMaxLoc(counters, NULL, NULL, NULL, &maxLoc);

        int bitForSwich = maxLoc.x;
        correctedMessage.at<int>(0,bitForSwich)=!correctedMessage.at<int>(0,bitForSwich);    
    }
    return correctedMessage;
}
