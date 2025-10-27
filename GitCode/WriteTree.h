#pragma once

#include <string>
#include <fstream>
#include <filesystem>
#include <iostream>

#include "openssl/sha.h"

class WriteTree
{
};

bool writeTree(int argc, char* argv[]);
