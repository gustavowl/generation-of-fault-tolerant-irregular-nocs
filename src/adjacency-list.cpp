#include "include/adjacency-list.h"
#include <iostream> //TODO: DELETE ME

AdjacencyList::AdjacencyList() {
}

AdjacencyList::AdjacencyList(unsigned int numNodes) {
	if (numNodes == 0) return;
	
	this->numNodes = numNodes;
	this->numEdges = numEdges;
	this->adjl.resize(numNodes);
}

AdjacencyList::~AdjacencyList() {
	//TODO: Use valgrind to check whether adjl was deallocated
}

void AdjacencyList::addEdge(unsigned int origin, unsigned int destination,
		unsigned int weight) {

	if (adjl.empty()) return;
	if (origin >= numNodes || destination >= numNodes) return;

	Edge e;
	e.dest = destination;
	e.weight = weight;

	adjl[origin].push_back(e);
}

void AdjacencyList::delEdge(unsigned int origin, unsigned int destination) {
	std::cout << this->getNumEdges() + 70 << std::endl;
}
