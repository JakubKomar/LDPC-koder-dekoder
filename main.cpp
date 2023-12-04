#include "main.hpp"


int main(int argc, char *argv[]) {
/*
    decoder d;
    /*cv::Mat dMessage = (cv::Mat_<int>(1, 16) << 1,0,0,1,0,1,0,1,1,1,1,0,0,0,0,1);
    cv::Mat H = (cv::Mat_<int>(16, 8) <<
        1,1,0,0,0,0,1,0,
        1,0,0,0,0,0,0,0,
        0,0,0,0,1,1,1,0,
        0,0,1,0,1,1,0,0,
        0,0,0,0,1,0,0,0,
        1,0,0,1,0,0,0,1,
        1,1,0,1,0,0,1,1,
        0,0,1,0,1,0,1,0,
        0,1,1,1,1,1,1,1,
        0,1,0,0,0,0,0,0,
        0,0,1,0,0,0,0,0,
        0,0,0,1,0,0,0,0,
        0,0,0,0,1,0,0,0,
        0,0,0,0,0,1,0,0,
        0,0,0,0,0,0,1,0,
        0,0,0,0,0,0,0,1
    );
    auto result=gaussElimination(H ,dMessage);
    auto rtG=result.first;
    auto rx=result.second;
    cout<<"rtG\n"<<rtG<<endl;
    cout<<"rx\n"<<rx<<endl;
    */
    coder c;
    matrixMaker a;

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

    const auto cMessage=c.encode(G,bMessage);
    const string codedMassege=c.matToString(cMessage);
    cout<< "codeded:" <<codedMassege<<endl;

    /*decoder d;

    const auto dMessage=d.extractVector(codedMassege);
    */

    
    exit(0);
  /*  
    if(argc != 2) {
        printhelp();
    }
    else if(string(argv[1]) == "-h" || argv[1] == string("--help")) {
        printhelp();
    }
    else if(argv[1] == string("-e") ){
        coder c;
        matrixMaker a;

        auto bMessage=c.stringToBinary("a");

        const int inStrLen=bMessage.cols;
        const int n = inStrLen*2 ;
        const int d_v = inStrLen-1;
        const int d_c = inStrLen;
        
        #define SEED 42
        const auto H =a.makeParityCheckMatrix(n,d_v,d_c,SEED);
        const auto G = a.makeCodingMatrix(H);

        const auto cMessage=c.encode(G,bMessage);
        std::cout<<c.matToString(cMessage)<<std::endl;  
    }
    else if(argv[1] == string("-d")){
        decoder d;
        cout<<d.decodeString(getString())<<endl;
    }
    else{
        printhelp();
    }

    return 0;*/
}

string getString() {
    string input;
    getline(cin, input,'\0');
    return input;
}


void printhelp() {
    cout << 
R""""(Aplikace realizuje kódovaní (encoding) a dekódovaní (decoding) vstupnej správy pomocí Low-density parity-check code (LDPC). Kódový pomer LDPC kódera je 1/2
Autor: Bc. Jakub Komárek (xkomar33)
Použití:
    ./bms [ [ -e | -d ] << "vstupní řetězec"| -h

Argumenty:
    -e << vstup=[ASCII znaky] (slouží pro zakódovaní)
    -d << vstup=[znaky hodnôt jednotlivých bitov] (slouží pro dekódovaní)
    -h – parametr označující výpis nápovědy
)"""";
}
