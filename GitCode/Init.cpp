#include "Init.h"

bool init()
{
	try {
		std::filesystem::create_directory(".gitCode");
		std::filesystem::create_directory(".gitCode/objects");
		std::filesystem::create_directory(".gitCode/refs");

		std::ofstream headFile(".gitCode/HEAD");

		if (!headFile.is_open()) {
			std::cerr << "Failed to create .gitCode/HEAD file.\n";
			return false;
		}

		headFile << "ref: refs/heads/main\n";
		headFile.close();

		std::cout << "Initialized gitCode directory.";
		return true;
	}
	catch (...) {
		std::cerr << "Failed to initialize gitCode..." << std::endl;
		return false;
	}
}