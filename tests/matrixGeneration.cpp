#include <iostream>
#include <opencv2/opencv.hpp>

#include "../matrixMaker.hpp"
#include "../coder.hpp"
#include "../decoder.hpp"
using std::cout, std::endl, std::string, std::cin,std::getline,std::ends; // using declaration

int main(int argc, char const *argv[])
{
    matrixMaker a;
    coder c;

   
    auto bMessage=c.stringToBinary("a");

    const int inStrLen=bMessage.cols;
    const int n = inStrLen*2 ;
    const int d_v = inStrLen-1;
    const int d_c = inStrLen;
    
    #define SEED 4648
    const auto H = a.makeParityCheckMatrix(n,d_v,d_c,SEED);
    const auto G = a.makeCodingMatrix(H);
    cout<<"H\n"<<H<<endl;
    cout<<"G\n"<<G<<endl;
    return 0;
}
