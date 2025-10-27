#pragma once

#include <string>
#include <filesystem>
#include <sstream>

#include "openssl/sha.h"

class Hash
{
};

void toLower(std::string& s);

std::string firstTwoOfHash(std::string& s);
std::string otherOfHash(std::string& s);

std::string getHashOfObjectShort(std::string object);
std::string getHashOfObjectLong(std::string object);