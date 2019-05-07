#include "include/file-manager.h"

GraphRepresentation<size_t>* FileManager::readFile(std::string filepath,
		FileType type, char separator, char comment) {
	std::ifstream file;

	file.open(filepath);

	if (!file.is_open()) {
		return NULL;
	}
	
	GraphRepresentation<size_t>* gr = NULL;

	switch (type) {
		case FileType::adj_list:
			gr = readAdjList(&file, separator, comment);
			break;

		case FileType::adj_matrix:
			gr = readAdjMatrix(&file, separator, comment);
			break;
	}

	file.close();
	return gr;
}

AdjacencyList<size_t>* FileManager::readAdjList(std::ifstream* file,
		char separator, char comment) {
	std::string line;

	// expects to read two values in the header (first line):
	// number of nodes and number of edges.
	bool header_read = false;
	const size_t expected_header_size = 2;

	// expects three values in the remaining lines:
	// origin, destiny, and weight
	const size_t expected_size = 3;
	size_t values[expected_size] = {};
	size_t size = 0;

	AdjacencyList<size_t>* adjl = NULL;
	size_t num_edges = 0;

	while (std::getline(*file, line)) {
		size = parseLine(&line, values, separator, comment);

		if (size == 0) //Ignores comments
			continue;

		if (header_read && size == 3) {
			adjl->addEdge(
					typename GraphRepresentation<size_t>::Edge{
						.orig = values[0],
						.dest = values[1],
						.value = values[2]
					});
			continue;
		}

		if (!header_read && size == expected_header_size) {
			adjl = new AdjacencyList<size_t>(values[0], 0);
			num_edges = values[1];
			header_read = true;

			//TODO: check if this condition verifies if
			//memory space was allocated
			if (adjl == NULL)
				break;

			continue;
		}

		//ERROR: invalid .csv file
		break;
	}

	//assert num_edges matches number of inserted edges
	if (adjl != NULL && num_edges != adjl->getNumEdges()) {
		delete adjl;
		return NULL;
	}

	adjl->shrinkToFit(); //saves memory space

	return adjl;
}

AdjacencyMatrix<size_t>* FileManager::readAdjMatrix(std::ifstream* file,
		char separator, char comment) {
	return NULL;
}

size_t FileManager::parseLine(std::string* line, size_t* values,
		char separator, char comment) {
	size_t size = 0;
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
		if (index != std::string::npos && index != line->size() - 1)
			line->erase(index, line->size() - index);
	}


	while (!line->empty()) {
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

void FileManager::writeLine(std::ofstream* file, std::string line) {
	*file << line << '\n';
}

void FileManager::writeLine(std::string filepath, std::string line) {
	std::ofstream file;

	file.open(filepath);

	if (!file.is_open())
		return;
	
	FileManager::writeLine(&file, line);

	file.close();
}

void FileManager::writeFile(std::string filepath,
		const GraphRepresentation<size_t>* graph) {
	std::ofstream file;

	file.open(filepath);

	if (!file.is_open())
		return;
	
	FileManager::writeLine(&file, graph->toStr());

	file.close();
}
