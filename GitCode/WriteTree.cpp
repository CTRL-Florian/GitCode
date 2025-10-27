#include "WriteTree.h"

Tree writeTreeHelper(std::filesystem::path& path);
bool isExe(std::filesystem::path path);

bool writeTree(int argc, char* argv[])
{
	if (argc != 3) {
		std::cerr << "Usage: write-tree\n";
		return false;
	}

	std::filesystem::path path = std::filesystem::current_path();
	writeTreeHelper(path);

	return true;
}

Tree writeTreeHelper(std::filesystem::path& path) 
{
	std::vector<std::filesystem::path> paths;

	for (const std::filesystem::directory_entry entry : std::filesystem::directory_iterator(path)) {
		if (entry.path().filename().string() == ".git") {
			continue;
		}

		paths.push_back(entry.path());
	}
	std::sort(paths.begin(), paths.end());

	std::vector<std::unique_ptr<Object>> objects;

	for (std::filesystem::path path : paths) {
		if (std::filesystem::is_directory(path)) {
			
		}
		else {
			std::unique_ptr<Blob> blob = std::make_unique<Blob>(path);
			blob->binHash();
			blob->write();
			objects.push_back(std::move(blob));
		}
	}

	return Tree(path, objects);

	// 

	std::string treeHeader = "tree ";
	std::string treeContent;


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

bool isExe(std::filesystem::path path)
{
	std::filesystem::perms perms = std::filesystem::status(path).permissions();
	return (perms & std::filesystem::perms::owner_exec) != std::filesystem::perms::none ||
		(perms & std::filesystem::perms::group_exec) != std::filesystem::perms::none ||
		(perms & std::filesystem::perms::others_exec) != std::filesystem::perms::none;
}