#include "Object.h"

bool readObjectContent(std::filesystem::path& path, std::string& out);

Object::Object(Object::Type type, std::filesystem::path path) :
	mType{type}
{
	readObjectContent(path, mContent);

	mSize = mContent.size();
	
	type == Object::Type::blob ? mCompleteObject += "blob" : mCompleteObject += "tree";
	mCompleteObject += ' ';
	mCompleteObject += std::to_string(mSize);
	mCompleteObject += '\0';
	mCompleteObject += mContent;

	mBinHash = "";
	mHexHash = "";

	mCompressed = "";
}

bool Object::binHash()
{
	unsigned char digest[SHA_DIGEST_LENGTH];
	SHA1(reinterpret_cast<const unsigned char*>(mCompleteObject.c_str()), mCompleteObject.size(), digest);
	mBinHash = std::string(reinterpret_cast<char*>(digest), SHA_DIGEST_LENGTH);
	return true;
}

std::string Object::getBinhash()
{
	if (mBinHash.empty()) binHash();
	return mBinHash;
}

bool Object::hexHash()
{
	unsigned char digest[SHA_DIGEST_LENGTH];
	SHA1(reinterpret_cast<const unsigned char*>(mCompleteObject.c_str()), mCompleteObject.size(), digest);

	std::stringstream ss;
	ss << std::hex << std::setfill('0');

	for (unsigned char c : digest) {
		ss << std::setw(2) << static_cast<int>(c);
	}

	mHexHash = ss.str();

	return true;
}

std::string Object::getHexHash()
{
	if (mHexHash.empty()) hexHash();
	return mHexHash;
}

bool Object::compress()
{
	z_stream zs{};
	if (deflateInit(&zs, Z_DEFAULT_COMPRESSION) != Z_OK) {
		std::cerr << "zlib not initialized.\n";
		return false;
	}

	zs.next_in = reinterpret_cast<Bytef*>(mCompleteObject.data());
	zs.avail_in = mCompleteObject.size();

	char outBuffer[4096];

	int ret;
	do {
		zs.next_out = reinterpret_cast<Bytef*>(outBuffer);
		zs.avail_out = sizeof(outBuffer);

		ret = deflate(&zs, Z_FINISH);

		if (ret != Z_OK && ret != Z_STREAM_END) {
			std::cerr << "Compression failed (zlib error: " << ret << ")\n";
			deflateEnd(&zs);
			return false;
		}

		size_t bytesProduced = sizeof(outBuffer) - zs.avail_out;
		mCompressed.insert(mCompressed.end(), outBuffer, outBuffer + bytesProduced);

	} while (ret != Z_STREAM_END);

	deflateEnd(&zs);

	return true;
}

std::string Object::getCompressed()
{
	if (mCompressed.empty()) compress();
	return mCompressed;
}

bool Object::write()
{
	std::string filepath = ".gitCode/objects/" + mHexHash.substr(0, 2);
	std::filesystem::create_directories(filepath);
	filepath += '/' + mHexHash.substr(2);

	std::ofstream hashFile(filepath, std::ios::binary);
	if (!hashFile.is_open()) {
		std::cerr << "Couldn't open file.\n";
		return false;
	}

	hashFile.write(mCompressed.data(), mCompressed.size());

	return true;
}

bool readObjectContent(std::filesystem::path& path, std::string& out)
{
	std::ifstream file(path);

	if (!file.is_open()) {
		std::cerr << "Couldn't open file.\n";
		return false;
	}

	std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();

	out = content;

	return true;
}