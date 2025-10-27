#pragma once

#include "Hash.h"

#include <string>
#include <fstream>
#include <filesystem>
#include <iostream>

class FileFunctions
{
};

bool readFile(std::filesystem::path& path, std::string& out);
bool writeObjectFileBinary(std::string& hashHex, std::string& object);