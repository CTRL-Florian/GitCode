#include "util.h"
#include "Init.h"
#include "CatFile.h"
#include "HashObject.h"
#include "LsTree.h"

#include <string>
#include <vector>
#include <iostream>

int main(int argc, char *argv[])
{
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    if (argc < 2) { std::cerr << "Not enough arguments..." << std::endl; return EXIT_FAILURE; }
    
    std::string command = argv[1];

    if (command == "init") {
        return init() ? EXIT_SUCCESS : EXIT_FAILURE;
    } else if (command == "cat-file") {
        return catFile(argc, argv) ? EXIT_SUCCESS : EXIT_FAILURE;
    }
    else if (command == "hash-object") {
        return hashObject(argc, argv) ? EXIT_SUCCESS : EXIT_FAILURE;
    }
    else if (command == "ls-tree") {
        return lsTree(argc, argv) ? EXIT_SUCCESS : EXIT_FAILURE;
    }
    else {
        std::cerr << "Not a vallid command..." << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_FAILURE;
}
