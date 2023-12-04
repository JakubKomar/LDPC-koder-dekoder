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

 cv::Mat get_message(cv::Mat G,cv::Mat input){
    auto result=gaussElimination(G,input);
    auto rtG=result.first;
    auto rx=result.second;
    cout<<"rtG\n"<<rtG<<endl;
    cout<<"rx\n"<<rx<<endl;
    return G;
 }