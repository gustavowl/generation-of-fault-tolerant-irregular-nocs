template <class T>
GraphRepresentation<T>::GraphRepresentation() {
	numNodes = numEdges = 0;
}

template <class T>
GraphRepresentation<T>::~GraphRepresentation() {
}

template <class T>
size_t GraphRepresentation<T>::getNumNodes() {
	return numNodes;
}

template <class T>
size_t GraphRepresentation<T>::getNumEdges() {
	return numEdges;
}

template <class T>
std::vector<size_t> GraphRepresentation<T>::getNeighbours(size_t node) {
	size_t ncount = 0; //neighbours count
	std::vector<size_t> neighbours (this->numNodes);

	for (size_t dest = 0; dest < this->numNodes; dest++) {
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
	for (size_t orig = 0; orig < numNodes; orig++) {
		for (size_t dest = 0; dest < numNodes; dest++) {
			if (!edgeExists(orig, dest))
				continue;
			std::cout << "Edge from " << orig <<
				" to " << dest << ". " <<
				" Value = " << getEdgeValue(orig, dest) <<
				std::endl;

		}
	}
}

template <class T>
T GraphRepresentation<T>::getNullEdgeValue() {
	return nullEdgeValue;
}
