//TODO: clean includes
//#include <iostream>
//#include "include/file-manager.h"
//#include "include/graph-converter.h"
#include "include/graph-representation.h"
#include "include/adjacency-matrix.h"
//#include "include/tabu-search.h"

int main(int argc, char *argv[]) {
	/*
	if (argc != 6) {
		std::cout << "Invalid number of arguments! Five expected:\n" <<
			"\t1 - Path to .adjl file;\n" <<
			"\t2 - Value separator (',' is the standard);\n" <<
			"\t3 - Number of edges in the output graph (epsilon);\n" <<
			"\t4 - Tabu list size;\n" <<
			"\t5 - Number of iterations with no improvements " <<
			"(stop criteria)" << std::endl;
		return -1;
	}

	std::cout << "Reading \"" << argv[1] << "\" file..." << std::endl;

	GraphRepresentation<size_t>* gr = FileManager::readFile(
			argv[1], FileManager::FileType::adj_list, argv[2][0]);

	if (gr == NULL) {
		std::cout << "Failed to read \"" << argv[0] <<
			"\" file using separator '" << argv[1][0] <<
			"'." << std::endl;
		return -1;
	}

	std::cout << "Done." << std::endl;

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

	std::cout << "Begins Tabu Search..." << std::endl;
	AdjacencyMatrix<size_t>* res = TabuSearch<size_t>::start(
			gr, std::numeric_limits<size_t>::max(),
			std::stoi(argv[4]), std::stoi(argv[5]),
			std::stoi(argv[3]));

	delete gr;

	if (res == NULL) {
		std::cout << "Error running Tabu Search.\nProbably invalid " <<
			"epsilon or tabu list size values" << std::endl;
		return -1;
	}

	delete res;*/

	return 0;
}
