#include "HashObject.h"

bool hashObject(int argc, char* argv[])
{
	if (argc < 3 || argc > 4) {
		std::cerr << "Usage: hash-object ( -w |  ) <filename>\n";
		return false;
	}

	bool hashWrite = false;
	std::filesystem::path path;

	for (int i = 2; i < argc; i++) {
		std::string arg = argv[i];
		if (arg == "-w") {
			hashWrite = true;
		}
		else if (arg[0] != '-') {
			path = arg;
		}
		else {
			std::cerr << "Invalid option: " << arg << "\n";
			return false;
		}
	}

	if (path.empty()) {
		std::cerr << "Usage: hash-object ( -w |  ) <filename>\n";
		return false;
	}

	if (std::filesystem::is_directory(path)) {
		std::cerr << "Fatal: " << path.string() << ": Is a directory.\n";
	}

	Object ob(Object::Type::blob, path );
	std::cout << ob.getHexHash() << std::endl;

	if (hashWrite) ob.write();

	return true;
}