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
