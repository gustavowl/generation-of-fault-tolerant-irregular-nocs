template <class T>
void AdjacencyMatrix<T>::triangNodeIdSwap(unsigned int* origin,
		unsigned int* destination) {

	if (isTriangular && *origin > *destination) {
		unsigned int change = *origin;
		*origin = *destination;
		*destination = change;
	}
}

template <class T>
AdjacencyMatrix<T>::AdjacencyMatrix(unsigned int numNodes,
		bool symmetric, bool triangular, T noEdgeValue) {

	// Assert triangular -> symmetric
	if (numNodes == 0 || (triangular && !symmetric))
		return;

	this->isSymmetric = symmetric;
	this->isTriangular = triangular;
	this->adjm.resize(numNodes);
	this->noEdgeValue = noEdgeValue;

	//resizes adjm columns to triangular matrix
	if (isTriangular) {
		for (unsigned int i = 0; i < numNodes; i++)
			this->adjm[i].resize(i + 1);
		return;
	}
	
	//else, resizes adjm columns to square matrix
	for (unsigned int i = 0; i < numNodes; i++)
		this->adjm[i].resize(numNodes);
}

template <class T>
AdjacencyMatrix<T>::~AdjacencyMatrix() {
}

template <class T>
void AdjacencyMatrix<T>::addEdge(unsigned int origin,
		unsigned int destination, unsigned int weight) {
	//check if arguments are valid
	if (origin >= numNodes || destination >= numNodes ||
			weight == 0)
		return;

	triangNodeIdSwap(&origin, &destination);

	adjm[origin][destination] = weight;

	if (isSymmetric && !isTriangular)
		adjm[destination][origin] = weight;
}

template <class T>
void AdjacencyMatrix<T>::delEdge(unsigned int origin,
		unsigned int destination){
	//check if arguments are valid
	if (origin >= numNodes || destination >= numNodes)
		return;

	triangNodeIdSwap(&origin, &destination);

	adjm[origin][destination] = noEdgeValue;

	if (isSymmetric && !isTriangular)
		adjm[destination][origin] = noEdgeValue;

}

template <class T>
bool AdjacencyMatrix<T>::edgeExists(unsigned int origin,
		unsigned int destination) {

	if (origin >= numNodes || destination >= numNodes)
		return false;

	triangNodeIdSwap(&origin, &destination);

	return adjm[origin][destination] != noEdgeValue;
}

template <class T>
T AdjacencyMatrix<T>::getEdgeWeight(unsigned int origin,
		unsigned int destination) {

	if (origin >= numNodes || destination >= numNodes)
		return false;

	triangNodeIdSwap(&origin, &destination);

	return adjm[origin][destination];
}
