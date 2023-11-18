#include <iostream>
#include "main.hpp"

using std::cout, std::endl, std::string; // using declaration

int main(int argc, char *argv[]) {

    if(argc != 2) {
        printhelp();
    }
    else if(string(argv[1]) == "-h" || argv[1] == string("--help")) {
        printhelp();
    }
    else if(argv[1] == string("-e") ){
        coder c;
        c.codeString("ahoj");
    }
    else if(argv[1] == string("-d")){
        decoder d;
        d.decodeString("ahoj");
    }
    else{
        printhelp();
    }

    return 0;
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
