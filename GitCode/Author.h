#pragma once

#include <string>

class Author
{
public:
	Author(std::string name, std::string email, long timestamp) :
		mName{name}, mEmail{email}, mTimestamp{timestamp}
	{ }

	// #TODO: Which setters / getters are needed?
	std::string getName() { return mName; }
	std::string getEmail() { return mEmail; }
	long getTimestamp() { return mTimestamp; }

	void setName(std::string name) { mName = name; }
	void setEmail(std::string email) { mEmail = email; }
	void setTimestamp(long timestamp) { mTimestamp = timestamp; }

private:
	std::string mName;
	std::string mEmail;
	long mTimestamp;
	// #TODO: UTC offset.

};

