//TODO: clean includes
#include <iostream>
#include "include/file-manager.h"
#include "include/graph-converter.h"
#include "include/graph-representation.h"
#include "include/adjacency-matrix.h"
#include "include/tabu-search.h"
#include <cstdlib>
#include <time.h>

int main(int argc, char *argv[]) {
	std::cout << "Hello World" << std::endl;

//	GraphRepresentation<size_t>* gr = FileManager::readFile(
//			"task-graphs/adj-lists/descriptions_seed-73_multi-start-node_weight-range-1-100_dimension-4x4_in-out-degrees-2-2.adjl",
//			FileManager::FileType::adj_list);

	GraphRepresentation<size_t>* gr = FileManager::readFile(
			"dijkstra.adjl", FileManager::FileType::adj_list, ' ');

	if (gr == NULL)
		return -1;

	AdjacencyMatrix<size_t> adjm = AdjacencyMatrix<size_t>(
			gr->getNumNodes(), false, false, 0);
	GraphConverter::convert(gr, &adjm);
	
	std::cout << "Graph read from file:\n" <<
		"==============================" << std::endl;
	gr->print();
	std::cout << "==============================" << std::endl;
	std::cout << "Converted Graph:\n" <<
		"==============================" << std::endl;
	adjm.print();

	std::cout << "==============================" << std::endl;
	std::cout << gr->getNumEdges() << ' ' << adjm.getNumEdges() <<
		std::endl;

	//initializes seed for pseudo-random number generation
	srand(time(NULL));
	AdjacencyMatrix<size_t>* res = TabuSearch<size_t>::start(
			gr, gr->getNumEdges(), 10000, 8);

	delete res;
	delete gr;

	return 0;
}
