#include "include/file-manager.h"

bool FileManager::readFile(std::string filepath) {
	std::ifstream file;

	file.open(filepath);

	if (!file.is_open()) {
		return false;
	}

	file.close();
	return true;
}
