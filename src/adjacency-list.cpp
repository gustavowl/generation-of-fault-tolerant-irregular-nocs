#include "include/adjacency-list.h"
#include <iostream>

AdjacencyList::AdjacencyList() {
}

AdjacencyList::~AdjacencyList() {
}

void AdjacencyList::addEdge(unsigned int origin, unsigned int destination,
		unsigned int weight) {
}

void AdjacencyList::delEdge(unsigned int origin, unsigned int destination) {
	std::cout << this->getNumEdges() + 70 << std::endl;
}
