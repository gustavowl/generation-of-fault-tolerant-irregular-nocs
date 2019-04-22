template <class T>
typename AdjacencyList<T>::Edge* AdjacencyList<T>::searchEdge(size_t origin,
		size_t destination) {
	if (origin >= this->numNodes || destination >= this->numNodes)
		return NULL;

	//begin search
	for (size_t i = 0; i < adjl[origin].size(); i++) {
		if (adjl[origin][i].dest == destination)
			return &adjl[origin][i];
	}

	return NULL;
}

template <class T>
size_t AdjacencyList<T>::getEdgePos(size_t origin, Edge* edge) {
	for (size_t i = 0; i < adjl[origin].size(); i++) {
		if (&adjl[origin][i] == edge)
			return i;
	}
	// removes warning
	// SHOULD NOT BE USED TO VERIFY IF EDGE EXISTS
	return std::string::npos;
}

template <class T>
AdjacencyList<T>::AdjacencyList() {
}

template <class T>
AdjacencyList<T>::AdjacencyList(size_t numNodes, T nullEdgeValue) {
	if (numNodes == 0) return;
	
	this->adjl.resize(numNodes);
	if (adjl.empty()) //TODO: bad_alloc
		return;
	this->adjl.shrink_to_fit(); // saves memory space
	this->numNodes = numNodes;
	this->nullEdgeValue = nullEdgeValue;
}

template <class T>
AdjacencyList<T>::~AdjacencyList() {
}

template <class T>
void AdjacencyList<T>::addEdge(size_t origin, size_t destination,
		T value) {

	//check if arguments are valid
	if (origin >= this->numNodes || destination >= this->numNodes ||
			value == this->nullEdgeValue)
		return;

	AdjacencyList<T>::Edge e;
	e.dest = destination;
	e.value = value;

	adjl[origin].push_back(e); //TODO: bad_alloc
	this->numEdges++;
}

template <class T>
void AdjacencyList<T>::delEdge(size_t origin, size_t destination) {

	AdjacencyList<T>::Edge* e = searchEdge(origin, destination);

	if (e == NULL)
		return;

	size_t pos = getEdgePos(origin, e);

	adjl[origin].erase(adjl[origin].begin() + pos);
	this->numEdges--;
}

template <class T>
bool AdjacencyList<T>::edgeExists(size_t origin, size_t destination) {
	if (origin >= this->numNodes || destination >= this->numNodes)
		return false;
	return getEdgeValue(origin, destination) != this->nullEdgeValue;
}

template <class T>
T AdjacencyList<T>::getEdgeValue(size_t origin, size_t destination) {

	AdjacencyList<T>::Edge* e = searchEdge(origin, destination);

	if (e != NULL)
		return e->value;
	return this->nullEdgeValue;
}

template <class T>
size_t AdjacencyList<T>::getNodeDegree(size_t node) {
	return this->getNeighbours(node).size();
}

template <class T>
GraphRepresentation<T>* AdjacencyList<T>::copy() {
	//initial instantiations
	AdjacencyList<T>* ret = new AdjacencyList<T>(this->numNodes,
			this->nullEdgeValue);

	//checks if graph created is valid
	if (ret->numNodes == 0)
		return NULL;

	//copies edges
	for (size_t i = 0; i < this->numNodes; i++) {
		for (size_t j = 0; j < this->adjl[i].size(); j++) {
			ret->addEdge(i, this->adjl[i][j].dest,
					this->adjl[i][j].value);
		}
	}

	ret->shrinkToFit();

	return ret;
}

template <class T>
void AdjacencyList<T>::shrinkToFit() {
	for (size_t i = 0; i < this->numNodes; i++)
		this->adjl[i].shrink_to_fit();
}
