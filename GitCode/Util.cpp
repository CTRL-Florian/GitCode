#include "Util.h"

void toLower(std::string& s)
{
    for (char& c : s) {
        c = std::tolower(static_cast<unsigned char>(c));
    }
}

std::string firstTwoOfHash(std::string& s)
{
    return s.substr(0, 2);
}

std::string otherOfHash(std::string& s)
{
    return s.substr(2);
}