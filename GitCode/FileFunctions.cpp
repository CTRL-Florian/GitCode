#include "FileFunctions.h"

bool readFile(std::filesystem::path& path, std::string& out)
{
	std::ifstream file(path);

	if (!file.is_open()) {
		std::cerr << "Couldn't open file.\n";
		return false;
	}

	std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();

	out = content;

	return true;
}

bool writeObjectFileBinary(std::string& hashHex, std::string& object)
{
	std::string filepath = ".gitCode/objects/" + firstTwoOfHash(hashHex);
	std::filesystem::create_directories(filepath);
	filepath += '/' + otherOfHash(hashHex);

	std::ofstream hashFile(filepath, std::ios::binary);
	if (!hashFile.is_open()) {
		std::cerr << "Couldn't open file.\n";
		return false;
	}

	hashFile.write(object.data(), object.size());

	return true;
}