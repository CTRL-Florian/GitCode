#include "Object.h"

bool readObjectContent(std::filesystem::path& path, std::string& out);
bool isExe(std::filesystem::path path);

Blob::Blob(std::filesystem::path path)
{
	mPath = path;

	mType = Object::Type::blob;

	readObjectContent(mPath, mContent);

	mSize = mContent.size();

	mCompleteObject += "blob";
	mCompleteObject += ' ';
	mCompleteObject += std::to_string(mSize);
	mCompleteObject += '\0';
	mCompleteObject += mContent;

	mBinHash = "";
	mHexHash = "";

	mMode = "";
	mName = "";

	mCompressed = "";
}

Tree::Tree(std::filesystem::path path, std::vector<std::unique_ptr<Object>>&& objects) :
	mObjects(std::move(objects))
{
	mPath = std::move(path);

	mType = Object::Type::tree;

	for (const std::unique_ptr<Object>& ob : mObjects) {
		mContent += ob->getMode();
		mContent += ' ';
		mContent += ob->getName();
		mContent += '\0';
		mContent.append(ob->getBinHash().data(), ob->getBinHash().size());
	}

	mSize = mContent.size();

	mCompleteObject += "tree";
	mCompleteObject += ' ';
	mCompleteObject += std::to_string(mSize);
	mCompleteObject += '\0';
	mCompleteObject += mContent;

	mBinHash = "";
	mHexHash = "";

	mMode = "";
	mName = "";

	mCompressed = "";
}

bool Object::binHash()
{
	unsigned char digest[SHA_DIGEST_LENGTH];
	SHA1(reinterpret_cast<const unsigned char*>(mCompleteObject.c_str()), mCompleteObject.size(), digest);
	mBinHash = std::string(reinterpret_cast<char*>(digest), SHA_DIGEST_LENGTH);
	return true;
}

std::string Object::getBinHash()
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
		ss << std::setw(2) << static_cast<int>(c & 0xff);	// Change?
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

	char outBuffer[8192]; // 8kb

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
	if (mHexHash.empty()) hexHash();
	if (mCompressed.empty()) compress();

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

bool Blob::mode()
{
	if (std::filesystem::is_symlink(mPath)) {
		mMode = "120000";
	}
	// #TODO: When is a file executable?
	/*
	else if (isExe(mPath)) {
		mMode = "100755";
	}
	*/
	else {
		mMode = "100644";
	}
	return true;
}

bool Tree::mode()
{
	mMode = "40000";
	return true;
}

std::string Object::getMode()
{
	if (mMode.empty()) mode();
	return mMode;
}

bool Object::name()
{
	mName = mPath.filename().string();
	return true;
}

std::string Object::getName()
{
	if (mName.empty()) name();
	return mName;
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

bool isExe(std::filesystem::path path)
{
	std::error_code ec;
	auto perms = std::filesystem::status(path, ec).permissions();
	if (ec) return false; // fallback bij error

	using fs = std::filesystem::perms;
	return ((perms & fs::owner_exec) == fs::owner_exec) ||
		((perms & fs::group_exec) == fs::group_exec) ||
		((perms & fs::others_exec) == fs::others_exec);
}