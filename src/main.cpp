#include <iostream>
#include "include/file-manager.h"
#include "include/graph-representation.h"
#include "include/adjacency-matrix.h"
#include "include/graph-converter.h"

int main(int argc, char *argv[]) {
	std::cout << "Hello World" << std::endl;

//	GraphRepresentation<unsigned int>* gr = FileManager::readFile(
//			"task-graphs/adj-lists/descriptions_seed-73_multi-start-node_weight-range-1-100_dimension-4x4_in-out-degrees-2-2.adjl",
//			FileManager::FileType::adj_list);

	GraphRepresentation<unsigned int>* gr = FileManager::readFile(
			"dijkstra.adjl", FileManager::FileType::adj_list, ' ');

	if (gr == NULL)
		return -1;

	AdjacencyMatrix<unsigned int> adjm = AdjacencyMatrix<unsigned int>(
			gr->getNumNodes(), false, false, 0);
	GraphConverter::convert(gr, &adjm);
//	AdjacencyMatrix<unsigned int> ba = AdjacencyMatrix<unsigned int>(
//			1000, false, false, 0);
//
	std::cout << "Graph read from file:\n" <<
		"==============================" << std::endl;
	gr->print();
	std::cout << "==============================" << std::endl;
	std::cout << "Converted Graph:\n" <<
		"==============================" << std::endl;
	adjm.print();

	std::cout << "==============================" << std::endl;
	std::cout << gr->getNumEdges() << ' ' << adjm.getNumEdges() << std::endl;


	delete gr;

	return 0;
}
