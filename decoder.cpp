#include "decoder.hpp"

decoder::decoder()
{
}

decoder::~decoder()
{
}

string decoder::decodeString(string input){
    string formatedInput= formatString(input);

    return formatedInput;
}

string decoder::formatString(string input){
    string output;
    for (auto & c: input){
        if(c=='0' || c=='1'){
            output+=c;
        }
    }/*
    for(int i=0;i<output.length();i+=8){
        string temp=output.substr(i,8);
        int num=stoi(temp,0,2);
        output[i/8]=num;
    }*/
    return output;
}