#include <iostream>
#include "include/file-manager.h"
#include "include/graph-representation.h"

int main(int argc, char *argv[]) {
	std::cout << "Hello World" << std::endl;

	GraphRepresentation* gr = FileManager::readFile(
			"task-graphs/adj-lists/descriptions_seed-73_multi-start-node_weight-range-1-100_dimension-4x4_in-out-degrees-2-2.adjl",
			FileManager::FileType::adj_list);

	if (gr != NULL)
		delete gr;

	return 0;
}
