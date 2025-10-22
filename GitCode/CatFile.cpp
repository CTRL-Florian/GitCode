#include "CatFile.h"

#include <filesystem>

bool catFile(int argc, char* argv[])
{
	if (argc < 3) {
		std::cerr << "Invalid arguments, required: '<blob-sha>'\nOptional: '-t', '-p'\n";
		return false;
	}

	bool catType = false;
	bool catPrint = false;
	std::string hash;

	for (int i = 2; i < argc; i++) {
		std::string arg = argv[i];

		if (arg == "-t") {
			catType = true;
		}
		else if (arg == "-p") {
			catPrint = true;
		}
		else if (arg[0] != '-') {
			hash = arg;
		}
		else {
			std::cerr << "Invalid option: " << arg << "\n";
		}
	}

	std::cout << firstTwoOfHash(hash) + "/" + otherOfHash(hash) << std::endl;

	std::cout << "Current: " << std::filesystem::current_path() << std::endl;

	std::string filename = "./git/objects/" + firstTwoOfHash(hash) + "/" + otherOfHash(hash);
	std::ifstream file(filename, std::ios::binary);

	if (!file.is_open()) {
		std::cerr << "Couldn't open file.";
		return false;
	}

	std::string objectString{ std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() };

	file.close();

	std::string objectContent;
	size_t pos = objectString.find('\0');

	if (catType) {
		objectContent += objectString.substr(0, pos);
	}

	if (catPrint) {
		objectContent += objectString.substr(pos + 1);
	}

	std::cout << objectContent;

	return true;
}
