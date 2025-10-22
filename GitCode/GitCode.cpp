#include <iostream>

#include "util.h"
#include "CatFile.h"

#include <string>
#include <vector>

int main(int argc, char *argv[])
{
    if (argc < 2) std::cerr << "Not enough arguments...";

    std::vector<std::string> arguments;

    for (int i = 0; i < argc; i++) {
        arguments.push_back(argv[i]);
        toLower(arguments[i]);
    }
    
    std::string command = arguments[1];

    if (command == "init") {
        return -1;
    } else if (command == "cat-file") {
        catFile(arguments);
    }

    return 0;
}
