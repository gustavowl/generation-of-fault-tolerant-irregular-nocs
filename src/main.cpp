#include <iostream>
#include "include/file-manager.h"
#include "include/graph-representation.h"
#include "include/adjacency-matrix.h"
#include "include/graph-converter.h"

int main(int argc, char *argv[]) {
	std::cout << "Hello World" << std::endl;

	GraphRepresentation<unsigned int>* gr = FileManager::readFile(
			"task-graphs/adj-lists/descriptions_seed-73_multi-start-node_weight-range-1-100_dimension-4x4_in-out-degrees-2-2.adjl",
			FileManager::FileType::adj_list);

	if (gr == NULL)
		return -1;

	AdjacencyMatrix<bool> adjm = AdjacencyMatrix<bool>(
			gr->getNumNodes(), true, true, false);
	GraphConverter::convert(gr, &adjm);

	delete gr;

	return 0;
}
