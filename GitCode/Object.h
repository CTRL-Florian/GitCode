#pragma once

#include "FileFunctions.h"

#include <string>
#include <sstream>
#include <filesystem>

#include <zlib.h>
#include <openssl/sha.h>

class Object
{
public:
	enum Type {blob, tree};
	Object(Type type, std::filesystem::path path);

	std::string getContent() const { return mContent; }
	size_t getSize() const { return mSize; }
	std::string getCompleteObject() const { return mCompleteObject; }

	bool binHash();
	std::string getBinhash();

	bool hexHash();
	std::string getHexHash();

	bool compress();
	std::string getCompressed();

	bool write();

private:
	Type mType;
	std::string mContent;
	size_t mSize;
	std::string mCompleteObject;

	std::string mBinHash;
	std::string mHexHash;

	std::string mCompressed;
};
