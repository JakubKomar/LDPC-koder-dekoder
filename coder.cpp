#include "coder.hpp"

coder::coder()
{
}

coder::~coder()
{
}


string coder::codeString(string input){

    input= formatString(input);

    return input;
}

string coder::formatString(string input){
    string output;
    for (auto & c: input){
        if((c>='a' && c<='z') || (c>='A'&&c<='Z')|| (c>='0'&&c<='9')){
            output+=c;
        }
    }
    return output;
}