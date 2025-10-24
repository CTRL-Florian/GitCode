#pragma once

#include "Util.h"
#include "zlibFunctions.h"

#include <string>
#include <fstream>
#include <filesystem>
#include <iostream>

#include <openssl/sha.h>
#include <zlib.h>

class HashObject
{
};

bool hashObject(int argc, char* argv[]);
