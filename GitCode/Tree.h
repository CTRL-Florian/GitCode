#pragma once

#include <string>

class Tree
{
public:
	Tree();

	// #TODO: which setters / gettets are necesairy?
	int getMode() const { return mMode; }
	std::string getType() const { return mType; }
	std::string getHash() const { return mHash; }
	std::string getName() const { return mName; }

	void setMode(int mode) { mMode = mode; }
	void setType(std::string type) { mType = type; }
	void setHash(std::string hash) { mHash = hash; }
	void setName(std::string name) { mName = name; }

private:
	int mMode;
	std::string mType;
	std::string mHash;
	std::string mName;

};

