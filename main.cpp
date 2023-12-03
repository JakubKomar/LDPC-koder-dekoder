#include <iostream>
#include "main.hpp"
#include "matrixMaker.hpp"
#include "opencv2/opencv.hpp"


int main(int argc, char *argv[]) {
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

    std::cout<<c.encode(G,bMessage)<<std::endl;
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
        cout<<c.codeString(getString())<<endl;
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
