#include "decoder.hpp"

/**
 * Tato metoda ze vstupního ascii řetězce složeného ze znaků  "0","1", z kterého vytvoří binární vektor v maticové podobě
 *
 * Tato metoda přijímá vstupní řetězec a provede operace na filtraci nepovolených znaků a převodu do matice.
 * Výsledkem je matice, která byla extrahována ze vstupního řetězce.
 *
 * @param input Vstupní řetězec obsahující informace o matici.
 * @return cv::Mat Extrahovaná matice z formátovaného vstupního řetězce.
 */
cv::Mat decoder::extractVector(const string &input){
    string formatedInput= formatString(input);
    auto mat= stringToMat(formatedInput);

    return mat;
}

/**
 * Formátuje vstupní řetězec, zachovává pouze znaky '0' a '1'.
 *
 * Tato metoda přijímá vstupní řetězec a provádí formátování, přičemž zachovává pouze znaky '0' a '1'.
 * Výsledkem je formátovaný řetězec obsahující pouze znaky '0' a '1'.
 *
 * @param input Vstupní řetězec, který má být formátován.
 * @return string Formátovaný řetězec obsahující pouze znaky '0' a '1'.
 */
string decoder::formatString(const string &input){
    string output;
    for (auto & c: input){
        if(c=='0' || c=='1'){
            output+=c;
        }
    }
    return output;
}

/**
 * Převádí řetězec na matici, kde '0' se mapuje na 0 a '1' na 1.
 *
 * Tato metoda přijímá řetězec obsahující znaky '0' a '1' a vytváří z něj matici,
 * kde každý znak '0' je mapován na číslo 0 a každý znak '1' na číslo 1.
 * Výsledkem je vektror v maticové popdobě obsahující tato čísla.
 *
 * @param input Vstupní řetězec obsahující znaky '0' a '1'.
 * @return cv::Mat Vektror v maticové podobě obsahující čísla 0 a 1.
 */
cv::Mat decoder::stringToMat(const string &input){
    cv::Mat output;
    
    for (auto & c: input){
        output.push_back(c=='0'?0:1);
    }

    return output.t();
}

/**
 * Získá dekódovanou zprávu z lineárního blokového kódu. Zpráva musí být korektní - případná oprava musí být provedena před zavoláním. 
 *
 * Tato metoda přijímá generující matici G a přijatý vektor r z lineárního blokového kódu.
 * Provádí Gaussovu eliminaci a následně zpětné substituce k získání dekódované zprávy.
 * Výsledek je binární vektor reprezentující dekódovanou zprávu.
 *
 * @param G Generující matice lineárního blokového kódu.
 * @param r Přijatý vektor z lineárního blokového kódu.
 * @return cv::Mat Binární vektor reprezentující dekódovanou zprávu.
 */
cv::Mat decoder::get_message(const cv::Mat &G,const cv::Mat & r){

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

/**
 * Převádí binární vektor na řetězec znaků.
 *
 * Tato metoda přijímá binární vektor a převádí ho na řetězec znaků.
 * Každých 8 bitů v binárním vektoru je interpretováno jako jeden znak ve výsledném řetězci.
 *
 * @param input Binární vektor reprezentující řetězec znaků.
 * @return string Řetězec znaků vytvořený z binárního vektoru.
 */
string decoder::convertBinaryVectorToString(const cv::Mat & input){

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

/**
 * Vypisuje stav uzlových schránek pro ladění.
 *
 * Tato metoda přijímá vektor vektorů reprezentující stav uzlových schránek
 * a vypisuje tento stav na standardní výstup pro ladění a diagnostické účely.
 *
 * @param vNodesState Vektor vektorů reprezentujících stav uzlových schránek.
 */
void decoder::debugMailBox(const std::vector< std::vector<int>> &vNodesState){
    cout<<"vNodesState:"<<endl;
    for(auto & vNode: vNodesState){
        for(auto & item: vNode){
            cout<<item<<" ";    
        }
        cout<<endl;
    }
}

/**
 * Kontroluje paritu vstupního vektoru vzhledem k paritní kontrolní matici H.
 *
 * Tato metoda přijímá paritní kontrolní matici H a vstupní vektor. Provádí binární
 * násobení H a vstupního vektoru a kontroluje, zda výsledný vektor má nulovou sumu.
 * Nulová suma značí, že vstupní vektor splňuje paritní kontrolu a je v souladu s maticí H.
 *
 * @param H Paritní kontrolní matice.
 * @param input Vstupní binární vektor ke kontrole paritou.
 * @return bool True, pokud vstupní vektor splňuje paritní kontrolu, jinak false.
 */
bool decoder::checkParity(const cv::Mat& H, const cv::Mat& input){
    cv::Mat check= matOp::binaryProduct(H,input.t());
    if(cv::sum(check)[0]==0){
        return true;
    }
    return false;
}

/**
 * HardDeciton dekodér s využitím algoritmu sum-product pro LDPC kódy.
 *
 * Tato metoda provádí tvrdý dekódování vstupního binárního vektoru.
 * Implementace pomocí dokumentu: https://www.bernh.net/media/download/papers/ldpc.pdf
 * Algoritmus velmi rychle konverguje, není třeba používat velký počet iterací.
 * Může se však stát softdeadlock - počítaný vektor konverguje mezi 2 stavy
 * 
 * @param H Paritní kontrolní matice LDPC kódu.
 * @param input Vstupní binární vektor pro dekódování.
 * @param maxIterations Maximální počet iterací dekodéru.
 * @return cv::Mat Dekódovaný binární vektor.
 */
cv::Mat decoder::hardDecitonDecoder(const cv::Mat& H, const cv::Mat& input,int maxIterations){
    
    int cNodesCount= H.cols;
    int vNodesCount= H.rows;

    // Inicializace stavu uzlových schránek, uzlových vrcholů (check vrcholů) a schránek pro výměnu informací
    cv::Mat cNodesState=input.row(0).clone();
    cv::Mat vNodesState=cv::Mat::zeros(vNodesCount,cNodesCount,CV_32S);
    std::vector< std::vector<int>> cNodeMailBox(cNodesCount, std::vector<int> (0));


    for (int i = 0; i < maxIterations; i++){
        
        // Kontrola paritní podmínky, pokud je splněna, vrátí aktuální stav uzlových schránek
        if(checkParity(H,cNodesState)){
            return cNodesState;
        }

        // Krok 1: Přenos informací z uzlových schránek na uzlové vrcholy
        for (int vNode_i = 0; vNode_i < vNodesCount; vNode_i++){ 
            for(int cNode_i=0; cNode_i<cNodesCount;cNode_i++){ 
                if(H.at<int>(vNode_i,cNode_i)==1){
                    vNodesState.at<int>(vNode_i,cNode_i)=cNodesState.at<int>(0,cNode_i);
                }
            }        
        }

        //cout<<"vNodesState:"<<endl<<vNodesState<<endl;
     
        // Krok 2: Výpočet a zasílání informací z uzlových vrcholů (check vrcholů) na uzlové schránky
        for(int vNode_i=0; vNode_i<vNodesCount;vNode_i++){ 
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

        // Step 3.0: Přidání aktuální stovu stavu uzlových schránek do schránek pro výpočet nového stavu uzlových schránek
        for(int cNode_i=0; cNode_i<cNodesCount;cNode_i++)         
            cNodeMailBox.at(cNode_i).push_back(cNodesState.at<int>(0,cNode_i)); 

        // Step 3.1: Výpočet nového stavu uzlových schránek na základě schránek pro výměnu informací
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
            // Výpočet rozhodovacího bitu a aktualizace stavu uzlové schránky
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


/**
 * Metoda pro korekci nejvíce chybových bitů v binárním vektoru.
 *
 * Tato metoda provádí korekci nejvíce chybových bitů v binárním vektoru využitím
 * paritní kontrolní matice H a iteračního procesu.
 *
 * @param H Paritní kontrolní matice LDPC kódu.
 * @param m Vstupní binární vektor s potenciálními chybami.
 * @param iterations Maximální počet iterací pro korekci.
 * @return cv::Mat Korektní binární vektor po provedení korekce.
 */
cv::Mat decoder::mostFuckedBitMethod(const cv::Mat& H, const cv::Mat& m, int iterations) {
    cv::Mat correctedMessage = m.clone();
    
    for (int iter = 0; iter < iterations; ++iter) {
        cv::Mat syndrome = matOp::binaryProduct(H,correctedMessage.t());

        if (countNonZero(syndrome) == 0) 
            break;        

        cv::Mat counters = cv::Mat::zeros(1,m.cols,CV_32S);

        for(int i=0;i<H.rows;i++)
            if(syndrome.at<int>(i,0))
                for(int j=0;j<H.cols;j++)
                    if(H.at<int>(i,j))
                        counters.at<int>(0,j)+=1;

        // Výběr bitu s největším počtem chyb
        cv::Point maxLoc;
        cv::minMaxLoc(counters, NULL, NULL, NULL, &maxLoc);

        int bitForSwich = maxLoc.x;
        correctedMessage.at<int>(0,bitForSwich)=!correctedMessage.at<int>(0,bitForSwich);    
    }
    return correctedMessage;
}
