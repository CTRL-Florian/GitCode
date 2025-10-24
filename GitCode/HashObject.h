#pragma once

#include "Util.h"

#include <string>
#include <fstream>
#include <iostream>

#include <openssl/sha.h>

class HashObject
{
};

bool hashObject(int argc, char* argv[]);
