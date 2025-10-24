#include "LsTree.h"

bool lsTree(int argc, char* argv[])
{
	if (argc < 3 || argc > 4) {
		std::cerr << "Usage: hash-object ( -w |  ) <filename>\n";
		return false;
	}

	bool lsName = false;
	std::string hash;

	for (int i = 2; i < argc; i++) {
		std::string arg = argv[i];

		if (arg == "--name-only") {
			lsName = true;
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
		std::cerr << "Usage: ls-tree (--name-only | ) <hash>\n";
		return false;
	}

	std::filesystem::path filename = std::filesystem::path(".git/objects") / firstTwoOfHash(hash) / otherOfHash(hash);
	std::ifstream file(filename, std::ios::binary);

	if (!file.is_open()) {
		std::cerr << "Not a valid object name: " << hash << "\n";
		return false;
	}

	std::string compressed((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

	file.close();

	std::string decompressed;
	if (!decompress(compressed, decompressed)) {
		std::cerr << "zlib decompression failed";
		return false;
	}

	std::cout << decompressed << std::endl;

	return true;
}