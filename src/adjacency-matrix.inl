template <class T>
AdjacencyMatrix<T>::AdjacencyMatrix(unsigned int numNodes,
		bool symmetric, bool shortRep) {

	// Assert shortRep -> symmetric
	if (! (!shortRep || symmetric) )
		return;

	this->isSymmetric = symmetric;
	this->shortRep = shortRep;
}

template <class T>
AdjacencyMatrix<T>::~AdjacencyMatrix() {
}

template <class T>
void AdjacencyMatrix<T>::addEdge(unsigned int origin,
		unsigned int destination, unsigned int weight) {
}

template <class T>
void AdjacencyMatrix<T>::delEdge(unsigned int origin,
		unsigned int weight){
}

template <class T>
bool AdjacencyMatrix<T>::edgeExists(unsigned int origin,
		unsigned int destination) {
	return false;
}

template <class T>
T AdjacencyMatrix<T>::getEdgeWeight(unsigned int origin,
		unsigned int destination) {
	return adjm[0][0];
}
