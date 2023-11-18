
#include <iostream>
#include <string>
using  std::cout, std::endl, std::string; // using declaration

class decoder
{
private:
  
public:
    decoder();
    ~decoder();
    string decodeString(std::string input);
    string formatString(string input);
};
