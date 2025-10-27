#pragma once

#include <string>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <iostream>

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

	std::string getBinhash();
	std::string getHexHash();

	std::string getCompressed();

	bool write();

private:
	Type mType;
	std::string mContent;
	size_t mSize;
	std::string mCompleteObject;

	bool binHash();
	std::string mBinHash;

	bool hexHash();
	std::string mHexHash;

	bool compress();
	std::string mCompressed;
};
