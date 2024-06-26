/**
 * @file coder.cpp
 * @brief Implementace třídy coder pro LDPC kódování a dekódování.
 *
 * Tato třída poskytuje metody pro kódování binárních vektorů
 * pomocí LDPC kódů. Implementuje operace, jako je zakódování, formátování řetězců pro vstup do třídy.
 * a další související funkce pro práci s LDPC kódy.
 *
 * @author Bc. Jakub Komárek
 * @year 2023
 */

#include "coder.hpp"

cv::Mat coder::stringToBinary(const string &input){
    string cleanInput= formatString(input);
    
    cv::Mat binaryVector(1, cleanInput.length()*8, CV_32S);

    int charCounter = 0;
    for (char c : cleanInput) {
        std::bitset<8> binaryRepresentation(c);

        for (int i = 7; i >=0; --i) {
            binaryVector.at<int>(0, charCounter*8 +7-i) = binaryRepresentation[i]?1:0;
        }
        charCounter++;
    }

    return binaryVector;
}

/**
 * Metoda pro formátování vstupního řetězce.
 *
 * Tato metoda provádí formátování vstupního řetězce tak, aby obsahoval pouze
 * alfanumerické znaky (písmena a číslice). Všechny ostatní znaky jsou vynechány.
 *
 * @param input Vstupní řetězec, který chcete formátovat.
 * @return std::string Formátovaný řetězec obsahující pouze alfanumerické znaky.
 */
string coder::formatString(const string &input){
    string output;

    for (auto & c: input){
        if((c>='a' && c<='z') || (c>='A'&&c<='Z')|| (c>='0'&&c<='9')){
            output+=c;
        }
    }

    return output;
}

/**
 * Metoda pro kódování binárního vektoru pomocí generující matice.
 *
 * Tato metoda kóduje binární vektor využitím generující matice G pro LDPC kódování.
 * Kódování probíhá násobením vstupního vektoru maticí G a aplikací operace XOR s hodnotou 1.
 *
 * @param G Generující matice LDPC kódu.
 * @param v Binární vektor, který chcete zakódovat.
 * @return cv::Mat Kódovaný binární vektor.
 */
cv::Mat coder::encode(const cv::Mat &G, const cv::Mat &v){
    cv::Mat d = matOp::matrixMulNoRestrictions(G, v.t());	

    d.forEach<int>([](int& item, const int* position) -> void {
        item %= 2;
    });
    cv::bitwise_xor(d, 1, d);
    return d;
}

/**
 * Konverze binární matice na řetězec.
 *
 * Tato metoda převádí binární matici na řetězec, kde každý prvek matice
 * je reprezentován jako znak v řetězci.
 *
 * @param X Binární matice, kterou chcete převést na řetězec.
 * @return std::string Řetězec reprezentující binární matici.
 */
string coder::matToString(const cv::Mat &X){
    std::stringstream ss;
    
    for (int i = 0; i < X.rows; ++i) {
        for (int j = 0; j < X.cols; ++j) {
            ss << X.at<int>(i, j);
        }
    }

    return ss.str();
}