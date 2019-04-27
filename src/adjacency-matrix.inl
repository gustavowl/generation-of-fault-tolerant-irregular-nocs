template <class T>
void AdjacencyMatrix<T>::triangNodeIdSwap(grEdge* edge) const {

	if (isTriangular && edge->orig < edge->dest) {
		size_t change = edge->orig;
		edge->orig = edge->dest;
		edge->dest = change;
	}
}

//TODO: verify if space to  be allocated is free.
//Otherwise, process may be killed.
template <class T>
AdjacencyMatrix<T>::AdjacencyMatrix(size_t numNodes,
		bool symmetric, bool triangular, T nullEdgeValue) {

	// Assert triangular -> symmetric
	if (numNodes == 0 || (triangular && !symmetric))
		return;

	this->adjm = new (std::nothrow) T* [numNodes];
	if (this->adjm == NULL) { //unable to allocate mem space
		setInvalid();
		return;
	}
	//removes garbage content
	for (size_t i = 0; i < numNodes; i++)
		this->adjm[i] = NULL;

	this->degrees = new (std::nothrow) size_t [numNodes];
	if (this->degrees == NULL) {
		setInvalid();
		return;
	}
	//removes garbage content
	for (size_t i = 0; i < numNodes; i++)
		this->degrees[i] = 0;

	this->numNodes = numNodes;
	this->isSymmetric = symmetric;
	this->isTriangular = triangular;
	this->nullEdgeValue = nullEdgeValue;

	//resizes adjm columns to triangular matrix
	if (isTriangular) {
		for (size_t i = 0; i < numNodes; i++) {
			//allocates space
			this->adjm[i] = new (std::nothrow) T [i+1];

			if (this->adjm[i] == NULL) {
				//failed to allocate memory. Reset
				setInvalid();
				return;
			}
			//sets all values to nullEdgeValue
			for (size_t j = 0; j <= i; j++)
				this->adjm[i][j] = this->nullEdgeValue;
		}
		return;
	}
	
	//else, resizes adjm columns to square matrix
	for (size_t i = 0; i < numNodes; i++) {
		//allocates space
		this->adjm[i] = new (std::nothrow) T [numNodes];

		if (this->adjm[i] == NULL) {
			//failed to allocate memory. Reset
			setInvalid();
			return;
		}
		//sets all values to nullEdgeValue
		for (size_t j = 0; j < numNodes; j++)
			this->adjm[i][j] = this->nullEdgeValue;
	}
}

template <class T>
AdjacencyMatrix<T>::~AdjacencyMatrix() {
	setInvalid();
}

template <class T>
void AdjacencyMatrix<T>::addEdge(grEdge edge) {
	//check if arguments are valid
	if (edge.orig >= this->numNodes || edge.dest >= this->numNodes ||
			edge.value == this->nullEdgeValue)
		return;

	triangNodeIdSwap(&edge);

	if (this->edgeExists(edge)) {
		return;
	}

	adjm[edge.orig][edge.dest] = edge.value;

	if (isSymmetric && !isTriangular)
		adjm[edge.dest][edge.orig] = edge.value;

	degrees[edge.orig]++;
	degrees[edge.dest]++;

	this->numEdges++;
}

template <class T>
void AdjacencyMatrix<T>::delEdge(grEdge edge){
	//check if arguments are valid
	if (edge.orig >= this->numNodes || edge.dest >= this->numNodes)
		return;

	triangNodeIdSwap(&edge);

	if (!this->edgeExists(edge)) {
		return;
	}

	adjm[edge.orig][edge.dest] = this->nullEdgeValue;

	if (isSymmetric && !isTriangular)
		adjm[edge.dest][edge.orig] = this->nullEdgeValue;

	degrees[edge.orig]--;
	degrees[edge.dest]--;

	this->numEdges--;
}

template <class T>
bool AdjacencyMatrix<T>::edgeExists(grEdge edge) const {

	if (edge.orig >= this->numNodes || edge.dest >= this->numNodes)
		return false;

	triangNodeIdSwap(&edge);

	return adjm[edge.orig][edge.dest] != this->nullEdgeValue;
}

template <class T>
T AdjacencyMatrix<T>::getEdgeValue(grEdge edge) const {

	if (edge.orig >= this->numNodes || edge.dest >= this->numNodes)
		return this->nullEdgeValue;

	triangNodeIdSwap(&edge);

	return adjm[edge.orig][edge.dest];
}

template <class T>
size_t AdjacencyMatrix<T>::getNodeDegree(size_t node) const {
	if (node >= this->numNodes)
		return 0;
	return degrees[node];
}

template <class T>
GraphRepresentation<T>* AdjacencyMatrix<T>::copy() const {
	//calls constructor to reserve mem space and initial instantiation
	AdjacencyMatrix<T>* ret = new AdjacencyMatrix(this->numNodes,
			this->isSymmetric, this->isTriangular, this->nullEdgeValue);

	//checks if matrix if valid
	if (ret->numNodes == 0)
		return NULL;

	grEdge edge;
	//copies edges
	for (size_t i = 0; i < ret->numNodes; i++) {
		size_t max_j = ret->numNodes - 1;
		edge.orig = i;

		if (ret->isSymmetric)
			max_j = i;
		
		for(size_t j = 0; j <= max_j; j++) {
			edge.dest = j;
			if (this->edgeExists(edge)) {
				edge.value = this->getEdgeValue(edge);
				ret->addEdge(edge);
			}
		}
	}

	return ret;
}

template <class T>
bool AdjacencyMatrix<T>::areEdgesEqual(grEdge edge1, grEdge edge2) {
	if (isSymmetric) {
		return edge1.value == edge2.value &&
			( (edge1.orig == edge2.orig &&
			  edge1.dest == edge2.dest)
			 ||
			 (edge1.orig == edge2.dest &&
			  edge2.orig == edge1.dest) );
	}

	return edge1.orig == edge2.orig &&
		edge1.dest == edge2.orig &&
		edge1.value == edge2.value;
}

template <class T>
void AdjacencyMatrix<T>::setInvalid() {
	//Set graph as zero-order/invalid.
	if (this->adjm != NULL) {
		//then there are some arrays allocated
		for (size_t i = 0; i < this->numNodes; i++) {

			if (this->adjm[i] == NULL) {
				//by the way the constructor works,
				//all pointers from here on are also NULL
				break;
			}

			delete[] this->adjm[i];
		}
		delete[] this-> adjm;
	}

	if (this->degrees != NULL)
		delete[] this->degrees;

	this->numNodes = 0;
}
