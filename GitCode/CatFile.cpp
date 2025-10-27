#include "CatFile.h"

#include <filesystem>

bool catFile(int argc, char* argv[])
{
	if (argc != 4) {
		std::cerr << "Usage: cat-file (-t | -p | -s | -e) <hash>\n";
		return false;
	}

	bool catType = false;
	bool catSize = false;
	bool catPrint = false;
	bool catExist = false;
	std::string hash;

	for (int i = 2; i < argc; i++) {
		std::string arg = argv[i];

		if (arg == "-t") {
			catType = true;
		}
		else if (arg == "-s") {
			catSize = true;
		}
		else if (arg == "-p") {
			catPrint = true;
		}
		else if (arg == "-e") {
			catExist = true;
		}
		else if (arg[0] != '-') {
			hash = arg;
		}
		else {
			std::cerr << "Invalid option: " << arg << "\n";
			return false;
		}
	}

	if (hash.empty()) {
		std::cerr << "Usage: cat-file (-t | -p | -s | -e) <hash>\n";
		return false;
	}

	File file(hash);

	if (catExist) return file.exists();

	if (catType) {
		std::cout << file.getType() << std::endl;
		std::cout << std::flush;
	}

	if (catSize) {
		std::cout << file.getSize() << std::endl;
		std::cout << std::flush;
	}

	if (catPrint) {
		std::cout << file.getContent() << std::endl;
		std::cout << std::flush;
	}

	return true;
}
