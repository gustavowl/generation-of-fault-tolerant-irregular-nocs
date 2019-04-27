//TODO: verify if space to  be allocated is free.
//Otherwise, process may be killed.
template <class T>
TabuAdjMatrix<T>::TabuAdjMatrix(size_t numNodes,
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
TabuAdjMatrix<T>::~TabuAdjMatrix() {
	setInvalid();
}

template <class T>
void TabuAdjMatrix<T>::addEdge(grEdge edge) {
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
void TabuAdjMatrix<T>::delEdge(grEdge edge){
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
bool TabuAdjMatrix<T>::edgeExists(grEdge edge) const {

	if (edge.orig >= this->numNodes || edge.dest >= this->numNodes)
		return false;

	triangNodeIdSwap(&edge);

	return adjm[edge.orig][edge.dest] != this->nullEdgeValue;
}

template <class T>
T TabuAdjMatrix<T>::getEdgeValue(grEdge edge) const {

	if (edge.orig >= this->numNodes || edge.dest >= this->numNodes)
		return this->nullEdgeValue;

	triangNodeIdSwap(&edge);

	return adjm[edge.orig][edge.dest];
}

template <class T>
size_t TabuAdjMatrix<T>::getNodeDegree(size_t node) const {
	if (node >= this->numNodes)
		return 0;
	return degrees[node];
}

template <class T>
GraphRepresentation<T>* TabuAdjMatrix<T>::copy() const {
	//calls constructor to reserve mem space and initial instantiation
	TabuAdjMatrix<T>* ret = new TabuAdjMatrix(this->numNodes,
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
bool TabuAdjMatrix<T>::areEdgesEqual(grEdge edge1, grEdge edge2) {
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
void TabuAdjMatrix<T>::setInvalid() {
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

template <class T>
typename TabuAdjMatrix<T>::Edge
TabuAdjMatrix<T>::selectRandomEdge(bool existent) {
	//TODO: TEST
	Edge e;
	size_t randomVal;

	if (existent)
		randomVal = rng() % numEdges;
	else
		//2 times considering directed graphs
		randomVal = rng() % (2*numNodes*numNodes - numEdges);

	size_t count = 0;
	for (size_t i = 0; i < numNodes; i++) {
		e.orig = i;

		for (size_t j = 0; j < numNodes; j++) {
			e.dest = j;

			if ( (existent && this->edgeExists(e)) ||
					(!existent && ! this->edgeExists(e))) {

				if (count == randomVal) {
					return e;
				}

				count++;
			}
		}
	}

	//return invalid edge
	e.orig = numNodes + 1;
	e.dest = numNodes + 1;
	return e;
}

template <class T>
typename TabuAdjMatrix<T>::Edge
TabuAdjMatrix<T>::selectRandomEdge(size_t incidentNode,
		bool existent) {
	//TODO: TEST
	Edge e;
	size_t randomVal;

	if (incidentNode >= numNodes)
		return Edge {.orig=numNodes+1, .dest=numNodes+1}; 

	if (existent) {
		randomVal = rng() % this->getNodeDegree(incidentNode);
	}
	else {
		//2 times considering directed graphs
		randomVal = rng() % (2 * numNodes -
				this->getNodeDegree(incidentNode));
	}

	size_t count = 0;
	//nodes with incidentNode as origin
	e.orig = incidentNode;
	for (size_t i = 0; i < numNodes; i++) {
		e.dest = i;

		if ( (existent && this->edgeExists(e)) ||
				(!existent && ! this->edgeExists(e))) {

			if (count == randomVal) {
				return e;
			}

			count++;
		}
	}
	//nodes with incidentNode as dest
	e.dest = incidentNode;
	for (size_t i = 0; i < numNodes; i++) {
		e.orig = i;

		if ( (existent && this->edgeExists(e)) ||
				(!existent && ! this->edgeExists(e))) {

			if (count == randomVal) {
				return e;
			}

			count++;
		}
	}

	//return invalid edge
	e.orig = numNodes + 1;
	e.dest = numNodes + 1;
	return e;
}

template <class T>
void TabuAdjMatrix<T>::swapEdgesNodes(Edge* edge1, Edge* edge2) {
	//there are two possible swap for edges (1, 2), (3, 4)
	//	(1, 4), (3, 2) or (1, 3), (2, 4)
}

template <class T>
typename TabuAdjMatrix<T>::Edge
TabuAdjMatrix<T>::spinEdge(Edge edge, size_t fixedNode) {
	return Edge {.orig = 0, .dest = 0};
}

template <class T>
void TabuAdjMatrix<T>::triangNodeIdSwap(grEdge* edge) const {

	if (isTriangular && edge->orig < edge->dest) {
		size_t change = edge->orig;
		edge->orig = edge->dest;
		edge->dest = change;
	}
}
