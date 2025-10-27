#include "WriteTree.h"

bool isExe(std::filesystem::path path)
{
	std::filesystem::perms perms = std::filesystem::status(path).permissions();
	return (perms & std::filesystem::perms::owner_exec) != std::filesystem::perms::none ||
		(perms & std::filesystem::perms::group_exec) != std::filesystem::perms::none ||
		(perms & std::filesystem::perms::others_exec) != std::filesystem::perms::none;
}

bool writeTree(int argc, char* argv[])
{
	if (argc != 3) {
		std::cerr << "Usage: write-tree\n";
		return false;
	}

	std::vector<std::filesystem::path> objects;

	std::filesystem::path path = std::filesystem::current_path();
	for (const auto& object : std::filesystem::directory_iterator(path)) {
		if (object.path().filename().string() == ".git") {
			continue;
		}

		objects.push_back(object.path());
	}

	std::string treeHeader = "tree ";
	std::string treeContent;

	std::sort(objects.begin(), objects.end());

	for (std::filesystem::path objectP : objects) {
		std::string mode;
		std::string name;
		std::string hash;

		std::filesystem::file_status s = std::filesystem::status(objectP);

		if (std::filesystem::is_symlink(s)) {
			mode = "120000";
			name = objectP.filename().string();
			std::filesystem::path symlinkContent = std::filesystem::read_symlink(objectP);
			std::string completeObject = "blob " + symlinkContent.string().size() + '\0' + symlinkContent.string();
			hash = getSha1Binary(completeObject);

			treeContent += mode + ' ' + name + '\0' + hash;

			std::string compressed;
			compress(completeObject, compressed);

			std::string hashHex = getSha1Hex(completeObject);

			writeObjectFileBinary(hashHex, compressed);
		}
		else if (!std::filesystem::is_directory(s)) {
			if (isExe(objectP)) {
				mode = "100755";
			}
			else {
				mode = "100644";
			}

			name = objectP.filename().string();
			std::string objectContent;
			readFile(objectP, objectContent);
			std::string completeObject = "blob " + std::to_string(objectContent.size()) + '\0' + objectContent;
			hash = getSha1Binary(completeObject);

			treeContent += mode + ' ' + name + '\0' + hash;

			std::string compressed;
			compress(completeObject, compressed);

			std::string hashHex = getSha1Hex(completeObject);

			writeObjectFileBinary(hashHex, compressed);
		}
		else {
			mode = "40000";	// Dir
		}
	}

	std::string treeObject = treeHeader + std::to_string(treeContent.size()) + '\0' + treeContent;
	std::string treehash = getSha1Hex(treeObject);

	std::cout << treehash << std::endl;

	return true;
}