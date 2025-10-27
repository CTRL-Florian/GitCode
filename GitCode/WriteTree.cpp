#include "WriteTree.h"

Tree writeTreeHelper(std::filesystem::path& path);

bool writeTree(int argc, char* argv[])
{
	if (argc != 3) {
		std::cerr << "Usage: write-tree\n";
		return false;
	}

	std::filesystem::path path = std::filesystem::current_path();
	Tree tree = writeTreeHelper(path);

	std::cout << tree.getHexHash() << std::endl;

	tree.write();

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

	for (std::filesystem::path entryPath : paths) {
		if (std::filesystem::is_directory(entryPath)) {
			std::unique_ptr<Tree> tree = std::make_unique<Tree>(writeTreeHelper(entryPath));
			tree->write();
			objects.push_back(std::move(tree));
		}
		else {
			std::unique_ptr<Blob> blob = std::make_unique<Blob>(entryPath);
			blob->write();
			objects.push_back(std::move(blob));
		}
	}

	return Tree(path, objects);
}