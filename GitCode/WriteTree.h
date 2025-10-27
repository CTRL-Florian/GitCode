#pragma once

#include "Hash.h"
#include "zlibFunctions.h"
#include "FileFunctions.h"

#include <string>
#include <fstream>
#include <filesystem>
#include <iostream>

#include "openssl/sha.h"

class WriteTree
{
};

bool isExe(std::filesystem::path path);

bool writeTree(int argc, char* argv[]);
