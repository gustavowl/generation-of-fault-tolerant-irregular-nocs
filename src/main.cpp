//TODO: clean includes
#include <iostream>
#include "include/file-manager.h"
#include "include/graph-representation.h"
#include "include/adjacency-matrix.h"
#include "include/graph-converter.h"
#include "include/dijkstra.h"
#include <limits>

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
	
	GraphRepresentation<size_t>* adjm2 = adjm.copy();
	delete adjm2;
//	AdjacencyMatrix<size_t> ba = AdjacencyMatrix<size_t>(
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

	std::cout << "DEBUGS DJIKSTRA'S ALGORITHM" << std::endl;
	for (size_t i = 1; i < adjm.getNumNodes(); i++)
		std::cout << "0 to " << i << " = " <<
			(Dijkstra<size_t>::dijkstra(&adjm, 0, i,
					std::numeric_limits<size_t>::max(), false).hops == HOP_INF) <<
			std::endl;

	delete gr;

	return 0;
}
