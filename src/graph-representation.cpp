#include "include/graph-representation.h"

GraphRepresentation::GraphRepresentation() {
	numNodes = numEdges = 0;
}

GraphRepresentation::~GraphRepresentation() {
}

unsigned int GraphRepresentation::getNumNodes() {
	return numNodes;
}

unsigned int GraphRepresentation::getNumEdges() {
	return numEdges;
}
