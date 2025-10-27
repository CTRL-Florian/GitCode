#include "WriteTree.h"

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

	std::string treeFile = "tree ";

	if (objects.empty()) {
		treeFile += "\0";
		return false;
	}

	std::sort(objects.begin(), objects.end());

	return false;
}