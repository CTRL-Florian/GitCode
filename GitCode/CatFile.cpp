#include "CatFile.h"

#include <filesystem>

/*
	Information about the decompression:
		https://chatgpt.com/c/68f9eb48-3238-8328-aabb-118881d7cd2f
*/

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

	std::filesystem::path filename = std::filesystem::path(".gitCode/objects") / firstTwoOfHash(hash) / otherOfHash(hash);
	std::ifstream file(filename, std::ios::binary);

	if (!file.is_open()) {
		if (catExist) {
			std::cerr << "Not a valid object name: " << hash << "\n";
		}
		else {
			std::cerr << "Couldn't open file.\n";
		}
		return false;
	}
	else if (catExist) {
		return true;
	}

	std::string compressed((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

	file.close();

	std::string decompressed;
	if (!decompress(compressed, decompressed)) {
		std::cerr << "zlib compression failed";
		return false;
	}

	int posSpace = decompressed.find(' ');
	int posNull = decompressed.find('\0');

	std::string objectContent;

	if (catType) {
		std::cout.write(decompressed.data(), posSpace);
		std::cout << std::flush;
	}

	if (catSize) {
		std::cout.write(decompressed.data() + posSpace + 1, posNull - (posSpace + 1));
		std::cout << std::flush;
	}

	if (catPrint) {
		std::cout.write(decompressed.data() + posNull + 1, decompressed.size() - (posNull + 1));
		std::cout << std::flush;
	}

	if (catType || catSize || catPrint) {
		std::cout << std::endl;
	}

	return true;
}
