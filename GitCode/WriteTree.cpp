#include "WriteTree.h"

std::unique_ptr<Tree> writeTreeHelper(std::filesystem::path& path);

bool writeTree(int argc, char* argv[])
{
	if (argc != 2) {
		std::cerr << "Usage: write-tree\n";
		return false;
	}

	std::filesystem::path path = std::filesystem::current_path();
	std::unique_ptr<Tree> tree = writeTreeHelper(path);

	std::cout << tree->getHexHash() << std::endl;

	tree->write();

	return true;
}

std::unique_ptr<Tree> writeTreeHelper(std::filesystem::path& path)
{
	std::vector<std::filesystem::path> paths;

	for (const std::filesystem::directory_entry entry : std::filesystem::directory_iterator(path)) {
		if (entry.path().filename().string() == ".git") {
			continue;
		}

		if (entry.path().filename().string() == ".gitCode") {
			continue;
		}

		paths.push_back(entry.path());
	}
	std::sort(paths.begin(), paths.end());

	std::vector<std::unique_ptr<Object>> objects;

	for (std::filesystem::path entryPath : paths) {
		if (std::filesystem::is_directory(entryPath)) {
			std::unique_ptr<Tree> subTree = writeTreeHelper(const_cast<std::filesystem::path&>(entryPath));
			subTree->write();
			objects.push_back(std::move(subTree));
		}
		else {
			std::unique_ptr<Blob> blob = std::make_unique<Blob>(entryPath);
			blob->write();
			objects.push_back(std::move(blob));
		}
	}

	return std::make_unique<Tree>(path, std::move(objects));
}