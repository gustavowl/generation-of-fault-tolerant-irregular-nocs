template <class T>
void TabuAdjMatrix<T>::nodeIdSwap(grEdge* edge) const {
	if (edge->orig < edge->dest) {
		size_t change = edge->orig;
		edge->orig = edge->dest;
		edge->dest = change;
	}
}

template <class T>
void TabuAdjMatrix<T>::setInvalid() {
	//Set graph as zero-order/invalid.
	if (this->adjm != NULL) {
		//then there are some arrays allocated
		for (size_t i = 0; i < this->numNodes; i++) {

			if (this->adjm[i] != NULL)
				delete[] this->adjm[i];

		}
		delete[] this-> adjm;
	}

	if (this->degrees != NULL)
		delete[] this->degrees;

	this->numNodes = 0;
	this->numEdges = 0;
}

template <class T>
bool TabuAdjMatrix<T>::isEdgeInvalid(grEdge edge,
		bool checkValue)  const {

	bool ret = edge.orig == edge.dest ||
			edge.orig >= this->numNodes ||
			edge.dest >= this->numNodes;

	if (checkValue)
		return ret || edge.value == this->nullEdgeValue;

	return ret;
}

template <class T>
grEdge TabuAdjMatrix<T>::generateInvalidEdge() {
	return grEdge {
		.orig = this->numNodes,
		.dest = this->numNodes,
		.value = this->nullEdgeValue
	};
}

//TODO: verify if space to  be allocated is free.
//Otherwise, process may be killed.
template <class T>
TabuAdjMatrix<T>::TabuAdjMatrix(size_t numNodes, T nullEdgeValue) {

	if (numNodes <= 1)
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
	this->nullEdgeValue = nullEdgeValue;

	//resizes adjm columns to triangular matrix w/ no
	//main diagonal
	for (size_t i = 1; i < numNodes; i++) {
		//allocates space
		this->adjm[i] = new (std::nothrow) T [i];

		if (this->adjm[i] == NULL) {
			//failed to allocate memory. Reset
			setInvalid();
			return;
		}
		//sets all values to nullEdgeValue
		for (size_t j = 0; j < i; j++)
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
	if (isEdgeInvalid(edge))
		return;

	nodeIdSwap(&edge);

	if (this->edgeExists(edge)) {
		return;
	}

	adjm[edge.orig][edge.dest] = edge.value;

	degrees[edge.orig]++;
	degrees[edge.dest]++;

	this->numEdges++;
}

template <class T>
void TabuAdjMatrix<T>::delEdge(grEdge edge){
	//check if arguments are valid
	if (isEdgeInvalid(edge, false))
		return;

	nodeIdSwap(&edge);

	if (!this->edgeExists(edge)) {
		return;
	}

	adjm[edge.orig][edge.dest] = this->nullEdgeValue;

	degrees[edge.orig]--;
	degrees[edge.dest]--;

	this->numEdges--;
}

template <class T>
bool TabuAdjMatrix<T>::edgeExists(grEdge edge) const {

	if (isEdgeInvalid(edge, false))
		return false;

	nodeIdSwap(&edge);

	return adjm[edge.orig][edge.dest] != this->nullEdgeValue;
}

template <class T>
T TabuAdjMatrix<T>::getEdgeValue(grEdge edge) const {

	if (isEdgeInvalid(edge, false))
		return this->nullEdgeValue;

	nodeIdSwap(&edge);

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
			this->nullEdgeValue); //TODO (std::nothrow) and check

	//checks if matrix if valid
	if (ret->numNodes == 0)
		return NULL;

	grEdge edge;
	//copies edges
	for (size_t i = 1; i < ret->numNodes; i++) {
		edge.orig = i;

		for(size_t j = 0; j < i; j++) {
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
	nodeIdSwap(&edge1);
	nodeIdSwap(&edge2);

	return edge1.orig == edge2.orig &&
		edge1.dest == edge2.dest &&
		edge1.value == edge2.value;
}

template <class T>
grEdge TabuAdjMatrix<T>::selectRandomEdge(bool existent) {
	grEdge e;
	size_t randomVal;

	if (existent) {
		randomVal = rng() % this->numEdges;
	}
	else {
		//since the matrix is triangular and has no
		//main diagonal, it has (n^2 - n)/2 possible
		//edges. This formula can be easily obtained
		//via arithmetic progression.
		randomVal = rng() % (
				(this->numNodes*this->numNodes - this->numNodes)/2 -
				this->numEdges );
	}

	size_t count = 0;
	for (e.orig = 1; e.orig < this->numNodes; e.orig++) {

		for (e.dest = 0; e.dest < this->numNodes; e.dest++) {

			if ( (existent && this->edgeExists(e)) ||
					(!existent && !this->edgeExists(e))) {

				if (count == randomVal) {
					e.value = this->getEdgeValue(e);
					return e;
				}

				count++;
			}
		}
	}

	return this->generateInvalidEdge();
}

template <class T>
grEdge TabuAdjMatrix<T>::selectRandomEdge(size_t incidentNode,
		bool existent) {
	grEdge e;
	size_t randomVal;

	if (incidentNode >= this->numNodes)
		return this->generateInvalidEdge();

	if (existent) {
		randomVal = rng() % this->getNodeDegree(incidentNode);
	}
	else {
		//any node has |V| - 1 possible edges since no
		//self-loops are allowed.
		randomVal = rng() % (this->numNodes - 1 -
				this->getNodeDegree(incidentNode));
	}

	size_t count = 0;
	e.orig = incidentNode;
	for (e.dest = 0; e.dest < this->numNodes; e.dest++) {

		if (e.orig == e.dest)
			continue;

		if ( (existent && this->edgeExists(e)) ||
				(!existent && !this->edgeExists(e))) {

			if (count == randomVal) {
				e.value = this->getEdgeValue(e);
				return e;
			}

			count++;
		}
	}

	return this->generateInvalidEdge();
}

template <class T>
grEdge TabuAdjMatrix<T>::selectRandomEdge(size_t incidentNode,
		size_t upperDegLim, bool existent) {
	grEdge e;
	//counts nodes with degree < upperDegLim
	size_t inLimit = 0;

	if (incidentNode >= this->numNodes) {
		e = this->generateInvalidEdge();
		return e;
	}

	e.orig = incidentNode;
	for (e.dest = 0; e.dest < this->numNodes; e.dest++) {

		if (e.orig == e.dest)
			continue;

		if ( (existent && this->edgeExists(e)) ||
				(!existent && !this->edgeExists(e)) &&
				this->getNodeDegree(e.dest) < upperDegLim ) {

			inLimit++;
		}
	}

	size_t randomVal = rng() % inLimit;
	size_t count = 0;

	for (e.dest = 0; e.dest < this->numNodes; e.dest++) {

		if (e.orig == e.dest)
			continue;

		if ( (existent && this->edgeExists(e)) ||
				(!existent && !this->edgeExists(e)) &&
				this->getNodeDegree(e.dest) < upperDegLim ) {

			if (count == randomVal) {
				e.value = this->getEdgeValue(e);
				return e;
			}

			count++;
		}
	}

	return this->generateInvalidEdge();
}

template <class T>
void TabuAdjMatrix<T>::swapEdgesNodes(grEdge* edge1, grEdge* edge2) {
	//there are two possible swap for edges (1, 2), (3, 4)
	//	(1, 4), (3, 2) or (1, 3), (2, 4)
	
	grEdge swapEdge1 = this->generateInvalidEdge();
	grEdge swapEdge2 = swapEdge1;

	int scenario = rng() % 2;
	int possibleScenariosCount = 0;
	size_t swapNode;

	if (!edgeExists(*edge1) || !edgeExists(*edge2)) {
		possibleScenariosCount = 3;
	}


	for (; possibleScenariosCount < 2; possibleScenariosCount++) {

		swapEdge1 = *edge1;
		swapEdge2 = *edge2;
		
		if (scenario == 0) {
			//(1, 2), (3, 4) -> (1, 4), (3, 2)
			swapNode = swapEdge1.dest;
			swapEdge1.dest = swapEdge2.dest;
			swapEdge2.dest = swapNode;
		}
		else {
			//(1, 2), (3, 4) -> (1, 3), (2, 4)
			swapNode = swapEdge1.dest;
			swapEdge1.dest = swapEdge2.orig;
			swapEdge2.orig = swapNode;
		}

		scenario = (scenario + 1) % 2;

		//check if swap is possible
		if (isEdgeInvalid(swapEdge1, false) ||
				isEdgeInvalid(swapEdge2, false)) {
			//this condition is necessary because swapping
			//edges (0, 1), and (0, 2) may generate edge
			//(0, 0), which is invalid and would not be
			//detected by edgeExists()
			continue;
		}
		if (edgeExists(swapEdge1) || edgeExists(swapEdge2)) {
			//one of the swapped edges already exists.
			//Thus, the graph cannot be changed
			continue;
		}

		//valid swap. Remove edges, choose weights randomly,
		//and add new swapped edges.
		this->delEdge(*edge1);
		this->delEdge(*edge2);

		if (rng() % 2 == 0) {
			swapEdge1.value = getEdgeValue(*edge1); 
			swapEdge2.value = getEdgeValue(*edge2); 
		}
		else {
			swapEdge1.value = getEdgeValue(*edge2); 
			swapEdge2.value = getEdgeValue(*edge1);
		}

		this->addEdge(swapEdge1);
		this->addEdge(swapEdge2);

		*edge1 = swapEdge1;
		*edge2 = swapEdge2;
		return;
	}
	
	//set edges invalid
	*edge1 = this->generateInvalidEdge();
	*edge2 = *edge1;
}

template <class T>
grEdge TabuAdjMatrix<T>::spinEdge(grEdge edge, size_t fixedNode) {

	if (!edgeExists(edge) || ( edge.orig != fixedNode &&
				edge.dest != fixedNode) ) {
		return this->generateInvalidEdge();
	}

	grEdge spinned = selectRandomEdge(fixedNode, false);

	if (this->isEdgeInvalid(spinned)) {
		//SHOULD BE NEVER REACHED
		return this->generateInvalidEdge();
	}

	spinned.value = edge.value;
	this->delEdge(edge);
	this->addEdge(spinned);

	return spinned;
}

template <class T>
grEdge TabuAdjMatrix<T>::spinEdge(grEdge edge, size_t fixedNode,
		size_t upperDegLim) {

	if (!edgeExists(edge) || ( edge.orig != fixedNode &&
				edge.dest != fixedNode) ) {
		return this->generateInvalidEdge();
	}

	grEdge spinned = selectRandomEdge(fixedNode, upperDegLim,
			false);

	if (this->isEdgeInvalid(spinned))
		return spinned;

	spinned.value = edge.value;
	this->delEdge(edge);
	this->addEdge(spinned);

	return spinned;
}

template <class T>
size_t TabuAdjMatrix<T>::getNodeWithNthDegree(size_t rankPos,
		bool largest) {

	if (rankPos >= this->numNodes)
		return this->numNodes;


	std::vector<size_t> nodes;
	nodes.reserve(this->numNodes);
	for (size_t i = 0; i < this->numNodes; i++)
		nodes.push_back(i);

	size_t node; //selected node
	size_t degree;

	for (; rankPos >= 0; rankPos--) {
		size_t index = 0; //index of selected node in nodes
		node = nodes[index];
		degree = this->getNodeDegree(node);

		for (size_t i = 1; i < nodes.size(); i++) {
			if ( (largest && this->getNodeDegree(nodes[i]) > degree) ||
				   (!largest && this->getNodeDegree(nodes[i]) < degree) ) {
				index = i;
				node = nodes[index];
				degree = this->getNodeDegree(node);
			}	
		}
		//removes node to search for other positions
		nodes.erase(nodes.begin() + index);
	}

	return selectedNode;
}
