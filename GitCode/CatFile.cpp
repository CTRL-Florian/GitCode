#include "CatFile.h"

static bool catType();
static bool catPrint();

bool catFile(std::vector<std::string> arguments)
{
	bool catType = false;
	bool catPrint = false;

	for (auto it : arguments) {
		if (it == "-t") {
			catType = true;
		} 
		else if (it == "-p") {
			catPrint = true;
		}
		else {
			catType = true;
			catPrint = true;
		}
	}

	if (catType) {
		
	}

	if (catPrint) {

	}

	return false;
}