#include "File.h"

bool readFileContent(std::filesystem::path& path, std::string& out);

File::File(std::string hexHash) :
	mHexHash{hexHash}
{
	mPath = std::filesystem::path(".gitCode/objects") / mHexHash.substr(0, 2) / mHexHash.substr(2);

	readFileContent(mPath, mCompressed) ? mExists = true : mExists = false;

	mDecompressed = "";

	mType = "";
	mSize = -1;
	mContent = "";
}

bool File::decompress()
{
	z_stream zs = {};
	if (inflateInit(&zs) != Z_OK) {
		std::cerr << "zlib not initialized.\n";
		return false;
	}

	zs.next_in = reinterpret_cast<Bytef*>(mCompressed.data());
	zs.avail_in = mCompressed.size();

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
		mDecompressed.insert(mDecompressed.end(), outBuffer, outBuffer + bytesProduced);

	} while (ret != Z_STREAM_END);

	inflateEnd(&zs);

	return true;
}

std::string File::getDecompressed()
{
	if (mDecompressed.empty()) decompress();
	return mDecompressed;
}

bool File::splitType()
{
	int posSpace = mDecompressed.find(' ');

	mType = std::string(mDecompressed.data(), posSpace);
}

std::string File::getType()
{
	if (mType.empty()) splitType();
	return mType;
}

bool File::splitSize()
{
	int posSpace = mDecompressed.find(' ');
	int posNull = mDecompressed.find('\0');

	const std::string size(mDecompressed.data() + posSpace + 1, posNull - (posSpace + 1));

	mSize = std::stoi(size);

	return true;
}

int File::getSize()
{
	if (mSize == -1) splitSize();
	return mSize;
}

bool File::splitContent()
{
	int posNull = mDecompressed.find('\0');

	mContent = std::string(mDecompressed.data() + posNull + 1, mDecompressed.size() - (posNull + 1));
}

std::string File::getContent()
{
	if (mContent.empty()) splitContent();
	return mContent;
}

bool readFileContent(std::filesystem::path& path, std::string& out)
{
	std::ifstream file(path, std::ios::binary);

	if (!file.is_open()) {
		std::cerr << "Couldn't open file.\n";
		return false;
	}

	std::string compressed((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();

	out = compressed;

	return false;
}
