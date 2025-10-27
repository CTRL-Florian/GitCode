#include "Hash.h"

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

std::string getHashOfObjectShort(std::string object)
{
    unsigned char digest[SHA_DIGEST_LENGTH];
    SHA1(reinterpret_cast<const unsigned char*>(object.c_str()), object.size(), digest);
    return std::string(reinterpret_cast<char*>(digest), SHA_DIGEST_LENGTH);
}

std::string getHashOfObjectLong(std::string object)
{
    unsigned char digest[SHA_DIGEST_LENGTH];
    SHA1(reinterpret_cast<const unsigned char*>(object.c_str()), object.size(), digest);

    std::stringstream ss;
    ss << std::hex << std::setfill('0');

    for (unsigned char c : digest) {
        ss << std::setw(2) << static_cast<int>(c);
    }
}