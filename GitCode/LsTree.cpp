#include "LsTree.h"

bool lsTree(int argc, char* argv[])
{
	if (argc < 3 || argc > 4) {
		std::cerr << "Usage: ls-tree (--name-only | ) <filename>\n";
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

	int posNull = decompressed.find('\0');
	std::string header = decompressed.substr(0, posNull);
	std::string content = decompressed.substr(posNull + 1);

	size_t pos = 0;
	while (pos < content.size()) {
		size_t spacePos = content.find(' ', pos);
		if (spacePos == std::string::npos) break;

		std::string mode = content.substr(pos, spacePos - pos);
		
		size_t nullPos = content.find('\0', pos);
		if (nullPos == std::string::npos) break;

		std::string name = content.substr(spacePos + 1, nullPos - (spacePos + 1));

		if (nullPos + 20 > content.size()) break;
		std::string binHash = content.substr(nullPos + 1, 20);

		std::stringstream ss;
		ss << std::hex << std::setfill('0');
		for (unsigned char c : binHash) {
			ss << std::setw(2) << static_cast<int>(c);
		}
		std::string hashHex = ss.str();

		if (!lsName) {
			std::cout << mode;
			if (mode == "40000") std::cout << " tree ";
			else std::cout << " blob ";
			std::cout << hashHex << '\t';
		}
		std::cout << name << std::endl;

		pos = nullPos + 21;
	}

	return true;
}