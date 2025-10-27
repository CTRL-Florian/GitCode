#pragma once

#include "hash.h"
#include "zlibFunctions.h"
#include "FileFunctions.h"

#include <string>
#include <fstream>
#include <filesystem>
#include <iostream>

#include <openssl/sha.h>

class HashObject
{
};

bool hashObject(int argc, char* argv[]);
