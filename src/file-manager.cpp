#include "include/file-manager.h"
#include <iostream> //TODO: DELETE ME

bool FileManager::readFile(std::string filepath, FileType type,
		char separator, char comment) {
	std::ifstream file;

	file.open(filepath);

	if (!file.is_open()) {
		return false;
	}
	
	switch (type) {
		case FileType::adj_list:
			readAdjList(&file, separator, comment);
			break;

		case FileType::adj_matrix:
			break;
	}

	file.close();
	return true;
}

bool FileManager::readAdjList(std::ifstream* file, char separator, char comment) {
	std::string line;

	// expects to read two values in the header (first line):
	// number of nodes and number of edges.
	bool header_read = false;
	const int expected_header_size = 2;

	// expects three values in the remaining lines:
	// origin, destiny, and weight
	const int expected_size = 3;
	int values [expected_size] = {};
	int size = 0;

	while (std::getline(*file, line)) {
		size = parseLine(&line, values, separator, comment);

		if (size == 0) //Ignores comments
			continue;

		if (header_read && size == 3) {
			//TODO: Add to edges to adj list
			continue;
		}

		if (!header_read && size == expected_header_size) {
			//TODO: Create adj list structure
			header_read = true;
			continue;
		}

		//ERROR: invalid .csv file
		return false;
	}
	return true;
}

bool FileManager::readAdjMatrix(std::ifstream* file, char separator, char comment) {
	return false;
}

int FileManager::parseLine(std::string* line, int* values,
		char separator, char comment) {
	int size = 0;
	std::size_t index;
	
	// removes comment
	if (!line->empty()) {
		index = line->find(comment);
		if (index != std::string::npos) //if valid index
			line->erase(index, line->size() - index);
	}
	// trims string
	if (!line->empty()) { //trims begin
		index = line->find_first_not_of(" \t");
		if (index != std::string::npos)
			line->erase(0, index);
	}
	if (!line->empty()) { //trims end
		index = line->find_last_not_of(" \t");
		if (index != std::string::npos)
			line->erase(index, line->size() - index);
	}

	while (!line->empty()) {
		std::cout << *line << std::endl;
		index = line->find(separator);
		
		if (index != std::string::npos) {
			values[size++] = std::stoi(line->substr(0, index));
			line->erase(0, index + 1);
		}
		else {
			values[size++] = std::stoi(*line);
			line->erase();
		}

	}

	return size;
}
