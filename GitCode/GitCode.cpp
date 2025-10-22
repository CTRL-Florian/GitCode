#include "util.h"
#include "init.h"
#include "CatFile.h"

#include <string>
#include <vector>
#include <iostream>

int main(int argc, char *argv[])
{
    if (argc < 2) { std::cerr << "Not enough arguments..."; return -1; }

    std::vector<std::string> arguments;

    for (int i = 0; i < argc; i++) {
        arguments.push_back(argv[i]);
        toLower(arguments[i]);
    }
    
    std::string command = arguments[1];

    if (command == "init") {
        init();
    } else if (command == "cat-file") {
        // #TODO: Check if .gitCode folder exists.
        catFile(arguments);
    }

    return 0;
}
