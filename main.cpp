/**
 * @file main.cpp
 * @brief main funkce pro ldpc kódování a dekódování.
 * 
 * @author Bc. Jakub Komárek
 * @year 2023
 */
#include "main.hpp"


#define SEED 42
#define CORRECTION_METHOD MOST_CONFLICT_BIT
#define CORRECTION_ITERATIONS 69
int main(int argc, char *argv[]) {
  
    //test();
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
        matrixMaker m;

        cv::Mat bMessage=c.stringToBinary(getString());

        const int inStrLen=bMessage.cols;
        const int n = inStrLen*2 ;
        const int d_v = inStrLen-1;
        const int d_c = inStrLen;
        
        cv::Mat H;
        cv::Mat G;

        if(matFilePath!=""){
            H = m.matrixFromFile(matFilePath);
            G = m.makeCodingMatrix(H);
            assert(H.cols==n);
            assert(H.rows==H.cols-2);
        }
        else{
            H = m.makeParityCheckMatrix(n,d_v,d_c,SEED);
            G = m.makeCodingMatrix(H);
            m.saveMatrixToCSV(H,"matica.csv");
        }
        
        const auto cMessage=c.encode(G,bMessage);
        std::cout<<c.matToString(cMessage)<<std::endl;  
    }
    else if(m==DECODE){

        if(matFilePath=="")
            matFilePath="matica.csv";

        matrixMaker m;

        cv::Mat H = m.matrixFromFile(matFilePath);
        cv::Mat G = m.makeCodingMatrix(H);

        decoder c;

        cv::Mat bMessage=c.extractVector(getString());
        
        assert(H.cols==bMessage.cols);
        assert(H.rows==H.cols-2);

        cv::bitwise_xor(bMessage, 1, bMessage);

        #if CORRECTION_METHOD==HARD_DECITION
            auto repairedMesss= c.hardDecitonDecoder(H,bMessage,CORRECTION_ITERATIONS);
        #elif CORRECTION_METHOD==MOST_CONFLICT_BIT
            auto repairedMesss= c.mostConflictBit(H,bMessage,CORRECTION_ITERATIONS);
        #else
            assert(false&&"Unknown correction method")
        #endif

        auto result =c.get_message(G,repairedMesss);   

        cout<<c.convertBinaryVectorToString(result)<<endl;
        
    }
    else
        printhelp();
    
    return 0;
}
#undef SEED
#undef CORRECTION_METHOD
#undef CORRECTION_ITERATIONS

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

void test(){
    cv::Mat H= (cv::Mat_<int>(4,8)<<
        0, 1, 0, 1, 1, 0, 0, 1,
        1, 1, 1, 0, 0, 1, 0, 0,
        0, 0, 1, 0, 0, 1, 1, 1,
        1, 0, 0, 1, 1, 0, 1, 0);

    cv::Mat  c= (cv::Mat_<int>(1,8)<<
        1, 0, 0, 1, 0, 1, 0, 1);

    decoder d;
    cout<<"c:"<<endl<<c<<endl;
    c.at<int>(0,7)=0;
    //c.at<int>(0,7)=0;
    //c.at<int>(0,6)=1;

    cout<<"c_damaged:"<<endl<<c<<endl;
    auto result =d.hardDecitonDecoder(H,c,69);
    cout<<"result:"<<endl<<result<<endl;
    exit(0);
}