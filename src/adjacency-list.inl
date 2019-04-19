template<class T>
Edge<T>* AdjacencyList<T>::searchEdge(unsigned int origin,
		unsigned int destination) {
	if (origin >= numNodes, destination >= numNodes)
		return NULL;

	//begin search
	for (size_t i = 0; i < adjl[origin].size(); i++) {
		if (adjl[origin][i].dest == destination)
			return &adjl[origin][i];
	}

	return NULL;
}

template <class T>
AdjacencyList<T>::AdjacencyList() {
}

template <class T>
AdjacencyList<T>::AdjacencyList(unsigned int numNodes, T nullEdgeValue) {
	if (numNodes == 0) return;
	
	this->adjl.resize(numNodes);
	if (adjl.empty())
		return;
	this->numNodes = numNodes;
	this->nullEdgeValue = nullEdgeValue;
}

template <class T>
AdjacencyList<T>::~AdjacencyList() {
}

template <class T>
void AdjacencyList<T>::addEdge(unsigned int origin, unsigned int destination,
		T value) {

	//check if arguments are valid
	if (origin >= numNodes || destination >= numNodes ||
			value == nullEdgeValue)
		return;

	Edge e;
	e.dest = destination;
	e.value = value;

	adjl[origin].push_back(e);
	numEdges++;
}

template <class T>
void AdjacencyList<T>::delEdge(unsigned int origin, unsigned int destination) {

	Edge<T>* e = searchEdge(origin, destination);

	if (e == NULL)
		return;

	size_t pos = getEdgePos(origin, e);

	adjl[origin].erase(adjl[origin].begin() + pos);
	numEdges--;
}

template <class T>
bool AdjacencyList<T>::edgeExists(unsigned int origin, unsigned int destination) {
	if (origin >= numNodes || destination >= numNodes)
		return false;
	return getEdgeValue(origin, destination) != nullEdgeValue;
}

template <class T>
T AdjacencyList<T>::getEdgeValue(unsigned int origin, unsigned int destination) {

	Edge<T>* e = searchEdge(origin, destination);

	if (e != NULL)
		return e->value;
	return nullEdgeValue;
}
