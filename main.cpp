#include "main.hpp"

enum mode {ENCODE,DECODE,NOTSET};

int main(int argc, char *argv[]) {

    mode m = NOTSET;
    string matFilePath="";

    for(int i = 1; i < argc; i++) {
        string arg=argv[i];

        if(string(arg) == "-h" || arg == string("--help")) 
            printhelp();      
        else if(arg == string("-e") && m == NOTSET) {
            m = ENCODE;
        }
        else if(arg == string("-d")&& m == NOTSET){
            m = DECODE;
        }
        else if(arg == string("-M")){
            if(i+1>argc)
                throw std::invalid_argument("Missing argument after -M");
            matFilePath=argv[i+1];
            i++;
        }
        else
            printhelp();
    }

    if(m==NOTSET)
        printhelp();

    auto getString = []() -> string {
        string input;
        getline(cin, input,'\n');
        return input;
    };
    
    if (m==ENCODE){
        coder c;
        matrixMaker a;

        cv::Mat bMessage=c.stringToBinary(getString());

        const int inStrLen=bMessage.cols;
        const int n = inStrLen*2 ;
        const int d_v = inStrLen-1;
        const int d_c = inStrLen;
        
        #define SEED 42
        cv::Mat H =a.makeParityCheckMatrix(n,d_v,d_c,SEED);
        cv::Mat G = a.makeCodingMatrix(H);


        if(matFilePath!="")
            a.saveMatrixToCSV(H,matFilePath);
        
        const auto cMessage=c.encode(G,bMessage);
        std::cout<<c.matToString(cMessage)<<std::endl;  
    }
    else if(m==DECODE){
        if(matFilePath=="")
            throw std::invalid_argument("Missing argument -M");

        matrixMaker m;

        cv::Mat H = m.matrixFromFile(matFilePath);
        cv::Mat G = m.makeCodingMatrix(H);

        decoder c;

        cv::Mat bMessage=c.extractVector(getString());
        //cv::Mat  bMessage = (  cv::Mat_ <int>(1, 16) << 1,1,1,1,0,0,1,1,0,0,0,1,1,1,1,0);
    
        cout<<"bMessage:"<<endl<<bMessage<<endl;    
        cv::bitwise_xor(bMessage, 1, bMessage);
        cout<<"bMessageXor:"<<endl<<bMessage<<endl;

        auto result =c.get_message(G,bMessage);   
        cout<<"result:"<<endl<<result<<endl;
        cout<<c.convertBinaryVectorToString(result)<<endl;
        
    }
    else
        printhelp();
    
    return 0;
}

void printhelp() {
    cout << 
R""""(Aplikace realizuje kódovaní (encoding) a dekódovaní (decoding) vstupnej správy pomocí Low-density parity-check code (LDPC). Kódový pomer LDPC kódera je 1/2
Autor: Bc. Jakub Komárek (xkomar33)
Použití:
    ./bms [ -M "matice.csv" [ -e | -d ] << "vstupní řetězec"| -h

Argumenty:
    -e << vstup=[ASCII znaky] (slouží pro zakódovaní)
    -d << vstup=[znaky hodnôt jednotlivých bitov] (slouží pro dekódovaní)
    -h – parametr označující výpis nápovědy
    -M "matice.csv" - matice pro kodovani/dekodovani, pokud není je vygenerována
)"""";
    exit(0);    
}
