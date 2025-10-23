#include "CatFile.h"

#include <filesystem>

/*
	Information about the decompression:
		https://chatgpt.com/c/68f9eb48-3238-8328-aabb-118881d7cd2f
*/

bool catFile(int argc, char* argv[])
{
	if (argc < 4 || argc > 4) {
		std::cerr << "Usage: cat-file (-t | -p) <hash>\n";
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

	if (hash.empty()) {
		std::cerr << "Usage: cat-file (-t | -p) <hash>\n";
		return false;
	}

	std::cout << firstTwoOfHash(hash) + "/" + otherOfHash(hash) << std::endl;

	std::cout << "Current: " << std::filesystem::current_path() << std::endl;

	std::string filename = "./git/objects/" + firstTwoOfHash(hash) + "/" + otherOfHash(hash);
	std::ifstream file(filename, std::ios::binary);

	if (!file.is_open()) {
		std::cerr << "Couldn't open file.";
		return false;
	}

	std::vector<char> compressed((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

	std::vector<char> decompressed(1024 * 1024); // 1MB buffer
	z_stream zs = {};
	inflateInit(&zs);
	zs.next_in = reinterpret_cast<Bytef*>(compressed.data());
	zs.avail_in = compressed.size();
	zs.next_out = reinterpret_cast<Bytef*>(decompressed.data());
	zs.avail_out = decompressed.size();
	int ret = inflate(&zs, Z_FINISH);
	if (ret != Z_STREAM_END) {
		std::cerr << "Decompressie mislukt\n";
	}
	inflateEnd(&zs);
	decompressed.resize(zs.total_out);

	file.close();

	std::string objectString(decompressed.begin(), decompressed.end());
	int pos = objectString.find('\0');

	std::string objectContent;

	if (catType) {
		objectContent += objectString.substr(0, pos);
	}

	if (catPrint) {
		objectContent += objectString.substr(pos + 1);
	}

	std::cout << objectContent << std::endl;

	return true;
}
