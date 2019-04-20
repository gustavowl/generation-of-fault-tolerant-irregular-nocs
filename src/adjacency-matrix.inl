template <class T>
void AdjacencyMatrix<T>::triangNodeIdSwap(unsigned int* origin,
		unsigned int* destination) {

	if (isTriangular && *origin < *destination) {
		unsigned int change = *origin;
		*origin = *destination;
		*destination = change;
	}
}

template <class T>
AdjacencyMatrix<T>::AdjacencyMatrix(unsigned int numNodes,
		bool symmetric, bool triangular, T nullEdgeValue) {

	// Assert triangular -> symmetric
	if (numNodes == 0 || (triangular && !symmetric))
		return;

	this->adjm.resize(numNodes);
	if (adjm.empty())
		return;
	this->adjm.shrink_to_fit(); //saves memory space

	this->numNodes = numNodes;
	this->isSymmetric = symmetric;
	this->isTriangular = triangular;
	this->nullEdgeValue = nullEdgeValue;

	//resizes adjm columns to triangular matrix
	if (isTriangular) {
		for (unsigned int i = 0; i < numNodes; i++) {
			//resize and sets all values to nullEdgeValue
			this->adjm[i].resize(i + 1, this->nullEdgeValue);
			//TODO: bad_alloc
			if (this->adjm[i].empty()) {
				//failed to allocate memory. Reset
				setInvalid();
				return;
			}
			this->adjm[i].shrink_to_fit();
		}
		return;
	}
	
	//else, resizes adjm columns to square matrix
	for (unsigned int i = 0; i < numNodes; i++) {
		//resizes and sets all values to nullEdgeValue
		try {
			this->adjm[i].resize(numNodes, this->nullEdgeValue);
			std::cout << "Allocated " << i << " out of " << numNodes << '\n';
		}
		catch (std::bad_alloc& ba) {
			std::cerr << "bad_alloc caught: " << ba.what() << '\n';
		}
		//TODO: bad_alloc
		if (this->adjm[i].empty()) {
			//failed to allocate memory. Reset
			setInvalid();
			return;
		}
		this->adjm[i].shrink_to_fit();
	}
}

template <class T>
AdjacencyMatrix<T>::~AdjacencyMatrix() {
}

template <class T>
void AdjacencyMatrix<T>::addEdge(unsigned int origin,
		unsigned int destination, T value) {
	//check if arguments are valid
	if (origin >= this->numNodes || destination >= this->numNodes ||
			value == this->nullEdgeValue)
		return;

	triangNodeIdSwap(&origin, &destination);

	adjm[origin][destination] = value;

	if (isSymmetric && !isTriangular)
		adjm[destination][origin] = value;

	this->numEdges++;
}

template <class T>
void AdjacencyMatrix<T>::delEdge(unsigned int origin,
		unsigned int destination){
	//check if arguments are valid
	if (origin >= this->numNodes || destination >= this->numNodes)
		return;

	triangNodeIdSwap(&origin, &destination);

	adjm[origin][destination] = this->nullEdgeValue;

	if (isSymmetric && !isTriangular)
		adjm[destination][origin] = this->nullEdgeValue;

	this->numEdges--;

}

template <class T>
bool AdjacencyMatrix<T>::edgeExists(unsigned int origin,
		unsigned int destination) {

	if (origin >= this->numNodes || destination >= this->numNodes)
		return false;

	triangNodeIdSwap(&origin, &destination);

	return adjm[origin][destination] != this->nullEdgeValue;
}

template <class T>
T AdjacencyMatrix<T>::getEdgeValue(unsigned int origin,
		unsigned int destination) {

	if (origin >= this->numNodes || destination >= this->numNodes)
		return this->nullEdgeValue;

	triangNodeIdSwap(&origin, &destination);

	return adjm[origin][destination];
}

template <class T>
void AdjacencyMatrix<T>::setInvalid() {
	//Set graph as zero-order/invalid.
	std::cout << "SET INVALID" << std::endl;
	this->adjm.resize(0);
	this->adjm.shrink_to_fit(); //saves memory
	this->numNodes = 0;
}
