#pragma once

#include <string>

class Commit
{
public:
	Commit(std::string treeHash, std::string commitMessage) : 
		mTreeHash{treeHash}, mCommitMessage{commitMessage}
	{ }

	// #TODO: Which setters / getters are necesairy?
	std::string getTreeHash() const { return mTreeHash; }
	std::string getCommitMessage() const { return mCommitMessage; }

	void setTreeHash(std::string treeHash) { mTreeHash = treeHash; }
	void setCommitMessage(std::string commitMessage) { mCommitMessage = commitMessage; }

private:
	std::string mTreeHash;
	std::string mCommitMessage;

};

