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
