#pragma once

#include <string>

class Hash
{
};

void toLower(std::string& s);

std::string firstTwoOfHash(std::string& s);
std::string otherOfHash(std::string& s);

std::string binToHex(unsigned char digest);