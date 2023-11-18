
#include <iostream>
#include <string>
using  std::cout, std::endl, std::string; // using declaration

class coder
{
private:
public:
    coder();
    ~coder();
    string codeString(std::string input);
    string formatString(std::string input);
};
