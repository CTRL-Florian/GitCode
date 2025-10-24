#include "HashObject.h"

bool hashObject(int argc, char* argv[])
{
	if (argc < 3 || argc > 4) {
		std::cerr << "Usage: hash-object ( -w |  ) <filename>\n";
		return false;
	}

	bool hashWrite = false;
	std::string filename;

	for (int i = 2; i < argc; i++) {
		std::string arg = argv[i];
		if (arg == "-w") {
			hashWrite = true;
		}
		else if (arg[0] != '-') {
			filename = arg;
		}
		else {
			std::cerr << "Invalid option: " << arg << "\n";
			return false;
		}
	}

	if (filename.empty()) {
		std::cerr << "Usage: hash-object ( -w |  ) <filename>\n";
		return false;
	}

	std::ifstream file(filename);

	if (!file.is_open()) {
		std::cerr << "Couldn't open file.\n";
		return false;
	}

	std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	std::string completeObject = "blob " + std::to_string(content.size()) + '\0' + content;

	file.close();

	unsigned char digest[SHA_DIGEST_LENGTH];
	SHA1(reinterpret_cast<const unsigned char*>(completeObject.c_str()), completeObject.size(), digest);

	char hash[SHA_DIGEST_LENGTH * 2 + 1];
	const char hex[] = "0123456789abcdef";
	for (int i = 0; i < SHA_DIGEST_LENGTH; i++) {
		hash[i * 2] = hex[(digest[i] >> 4) & 0xF];
		hash[i * 2 + 1] = hex[digest[i] & 0xF];  
	}
	hash[SHA_DIGEST_LENGTH * 2] = '\0';

	std::cout << hash << std::endl;

	if (!hashWrite) {
		return true;
	}

	std::string compressed;
	if (!compress(completeObject, compressed)) {
		std::cerr << "zlib compression failed";
		return false;
	}

	std::string hashString = hash;
	std::string filepath = ".gitCode/objects/" + firstTwoOfHash(hashString);
	std::filesystem::create_directories(filepath);
	filepath += '/' + otherOfHash(hashString);

	std::ofstream hashFile(filepath, std::ios::binary);
	if (!hashFile.is_open()) {
		std::cerr << "Couldn't open file.\n";
		return false;
	}

	hashFile.write(compressed.data(), compressed.size());
	
	return true;
}