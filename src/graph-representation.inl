template <class T>
GraphRepresentation<T>::GraphRepresentation() {
	numNodes = numEdges = 0;
}

template <class T>
GraphRepresentation<T>::~GraphRepresentation() {
}

template <class T>
unsigned int GraphRepresentation<T>::getNumNodes() {
	return numNodes;
}

template <class T>
unsigned int GraphRepresentation<T>::getNumEdges() {
	return numEdges;
}

template <class T>
std::vector<unsigned int> GraphRepresentation<T>::getNeighbours(unsigned int node) {
	unsigned int ncount = 0; //neighbours count
	std::vector<unsigned int> neighbours (this->numNodes);

	for (unsigned int dest = 0; dest < this->numNodes; dest++) {
		if (this->edgeExists(node, dest))
			neighbours[ncount++] = dest;
	}

	neighbours.resize(ncount);
	neighbours.shrink_to_fit();
	return neighbours;
}

template <class T>
bool GraphRepresentation<T>::isValid() {
	return numNodes != 0;
}

template <class T>
void GraphRepresentation<T>::print() {
	for (unsigned int orig = 0; orig < numNodes; orig++) {
		for (unsigned int dest = 0; dest < numNodes; dest++) {
			if (!edgeExists(orig, dest))
				continue;
			std::cout << "Edge from " << orig <<
				" to " << dest << ". " <<
				" Value = " << getEdgeValue(orig, dest) <<
				std::endl;

		}
	}
}
