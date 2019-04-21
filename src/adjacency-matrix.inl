template <class T>
void AdjacencyMatrix<T>::triangNodeIdSwap(unsigned int* origin,
		unsigned int* destination) {

	if (isTriangular && *origin < *destination) {
		unsigned int change = *origin;
		*origin = *destination;
		*destination = change;
	}
}

//TODO: verify if space to  be allocated is free.
//Otherwise, process may be killed.
template <class T>
AdjacencyMatrix<T>::AdjacencyMatrix(unsigned int numNodes,
		bool symmetric, bool triangular, T nullEdgeValue) {

	// Assert triangular -> symmetric
	if (numNodes == 0 || (triangular && !symmetric))
		return;

	this->adjm = new (std::nothrow) T* [numNodes];
	if (this->adjm == NULL) { //unable to allocate mem space
		setInvalid();
		return;
	}
	//removes garbage pointers
	for (unsigned int i = 0; i < numNodes; i++)
		this->adjm[i] = NULL;

	this->numNodes = numNodes;
	this->isSymmetric = symmetric;
	this->isTriangular = triangular;
	this->nullEdgeValue = nullEdgeValue;

	//resizes adjm columns to triangular matrix
	if (isTriangular) {
		for (unsigned int i = 0; i < numNodes; i++) {
			//allocates space
			this->adjm[i] = new (std::nothrow) T [i+1];

			if (this->adjm[i] == NULL) {
				//failed to allocate memory. Reset
				setInvalid();
				return;
			}
			//sets all values to nullEdgeValue
			for (unsigned int j = 0; j <= i; j++)
				this->adjm[i][j] = this->nullEdgeValue;
		}
		return;
	}
	
	//else, resizes adjm columns to square matrix
	for (unsigned int i = 0; i < numNodes; i++) {
		//allocates space
		this->adjm[i] = new (std::nothrow) T [numNodes];

		if (this->adjm[i] == NULL) {
			//failed to allocate memory. Reset
			setInvalid();
			return;
		}
		//sets all values to nullEdgeValue
		for (unsigned int j = 0; j < numNodes; j++)
			this->adjm[i][j] = this->nullEdgeValue;
		std::cout << "Allocated " << i+1 << " out of " <<
			numNodes << std::endl;
	}
}

template <class T>
AdjacencyMatrix<T>::~AdjacencyMatrix() {
	setInvalid();
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
GraphRepresentation<T>* AdjacencyMatrix<T>::copy() {
	//calls constructor to reserve mem space and initial instantiation
	AdjacencyMatrix<T>* ret = new AdjacencyMatrix(this->numNodes,
			this->isSymmetric, this->isTriangular, this->nullEdgeValue);

	//checks if matrix if valid
	if (ret->numNodes == 0)
		return NULL;

	//copies edges
	for (unsigned int i = 0; i < ret->numNodes; i++) {
		unsigned int max_j = ret->numNodes - 1;

		if (ret->isSymmetric)
			max_j = i;
		
		for(unsigned int j = 0; j <= max_j; j++)
			if (this->edgeExists(i, j))
				ret->addEdge(i, j, this->getEdgeValue(i, j));
	}

	return ret;
}

template <class T>
void AdjacencyMatrix<T>::setInvalid() {
	//Set graph as zero-order/invalid.
	std::cout << "SET INVALID" << std::endl;

	if (this->adjm != NULL) {
		//then there are some arrays allocated
		for (unsigned int i = 0; i < this->numNodes; i++) {

			if (this->adjm[i] == NULL) {
				//by the way the constructor works,
				//all pointers from here on are also NULL
				break;
			}

			delete[] this->adjm[i];
			std::cout << "Deallocated " << i + 1 << " out of " <<
				this->numNodes << std::endl;
		}
		delete[] this-> adjm;
	}

	this->numNodes = 0;
}
