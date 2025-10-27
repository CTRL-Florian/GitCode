#pragma once

#include <string>
#include <fstream>
#include <filesystem>
#include <iostream>

#include <zlib.h>

class File
{
public:
	File(std::string hexHash);

	bool exists() const { return mExists; }
	
	std::string getDecompressed();

	std::string getType();
	int getSize();
	std::string getContent();

private:
	std::string mHexHash;
	std::filesystem::path mPath;
	
	bool mExists;
	std::string mCompressed;

	bool decompress();
	std::string mDecompressed;

	bool splitType();
	std::string mType;

	bool splitSize();
	int mSize;

	bool splitContent();
	std::string mContent;
};

