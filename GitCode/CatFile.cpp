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

	std::filesystem::path filename = std::filesystem::path(".git/objects") / firstTwoOfHash(hash) / otherOfHash(hash);
	std::ifstream file(filename, std::ios::binary);

	if (!file.is_open()) {
		std::cerr << "Couldn't open file.\n";
		return false;
	}

	std::vector<char> compressed((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

	file.close();

	z_stream zs = {};
	if (inflateInit(&zs) != Z_OK) {
		std::cerr << "zlib not initialized.\n";
		return false;
	}

	zs.next_in = reinterpret_cast<Bytef*>(compressed.data());
	zs.avail_in = compressed.size();

	std::vector<char> decompressed;
	char outBuffer[4096]; // 4kb;
	
	int ret;
	do {
		zs.next_out = reinterpret_cast<Bytef*>(outBuffer);
		zs.avail_out = sizeof(outBuffer);

		ret = inflate(&zs, 0);

		if (ret != Z_OK && ret != Z_STREAM_END) {
			std::cerr << "Decompression failed (zlib error: " << ret << ")\n";
			inflateEnd(&zs);
			return false;
		}

		size_t bytesProduced = sizeof(outBuffer) - zs.avail_out;
		decompressed.insert(decompressed.end(), outBuffer, outBuffer + bytesProduced);

	} while (ret != Z_STREAM_END);

	inflateEnd(&zs);

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
