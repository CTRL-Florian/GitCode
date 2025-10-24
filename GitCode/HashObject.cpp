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
		else if (arg[0] == '-') {
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

	unsigned char digest[SHA_DIGEST_LENGTH];
	SHA1(reinterpret_cast<const unsigned char*>(content.c_str()), content.size(), digest);


	char hash[SHA_DIGEST_LENGTH * 2 + 1];
	for (int i = 0; i < SHA_DIGEST_LENGTH; i++) {
		sprintf(&hash[i * 2], "%02x", digest[i]);
	}
	hash[SHA_DIGEST_LENGTH * 2] = '\0';
	
	std::cout << hash << std::endl;

	if (!hashWrite) {
		return true;
	}
	
	return false;
}