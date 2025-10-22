#pragma once

#include <string>

class Object
{
public:
	Object(std::string content) :
		mContent{content}
	{ }

	// #TODO: Which setters / getters are necesairy?
	std::string getContent() const { return mContent; }

	void setContent(std::string content) { mContent = content; }

private:
	std::string mContent;

};

