#pragma once

#include <string>
#include <vector>
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

	std::string getContent() const { return mContent; }
	size_t getSize() const { return mSize; }
	std::string getCompleteObject() const { return mCompleteObject; }

	bool binHash();
	std::string getBinhash();

	bool hexHash();
	std::string getHexHash();

	std::string getCompressed();

	bool write();

	std::string getMode();
	std::string getName();

protected:
	std::filesystem::path mPath;

	Type mType;
	std::string mContent;
	size_t mSize;
	std::string mCompleteObject;

	std::string mBinHash;
	std::string mHexHash;

	bool compress();
	std::string mCompressed;

	virtual bool mode() = 0;
	std::string mMode;

	bool name();
	std::string mName;
};

class Blob : public Object
{
public:
	Blob(std::filesystem::path path);

private: 
	bool mode();
};

class Tree : public Object
{
public:
	Tree(std::filesystem::path path, std::vector<std::unique_ptr<Object>> objects);

private:
	bool mode();
	std::vector<std::unique_ptr<Object>> mObjects;
};