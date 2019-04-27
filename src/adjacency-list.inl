template <class T>
size_t AdjacencyList<T>::getEdgePos(const Edge edge) const {

	if (edge.orig >= numNodes || edge.dest >= numNodes)
		return numNodes;

	for (size_t i = 0; i < adjl[edge.orig].size(); i++) {
		if (adjl[edge.orig][i].dest == edge.dest)
			return i;
	}

	return numNodes;
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
void AdjacencyList<T>::addEdge(Edge edge) {

	//check if arguments are valid
	if (edge.orig >= this->numNodes || edge.dest >= this->numNodes ||
			edge.value == this->nullEdgeValue)
		return;

	if (this->edgeExists(edge))
		return;

	adjl[edge.orig].push_back(edge); //TODO: bad_alloc
	this->numEdges++;
}

template <class T>
void AdjacencyList<T>::delEdge(Edge edge) {

	if (!this->edgeExists(edge))
		return;

	size_t pos = getEdgePos(edge);

	adjl[edge.orig].erase(adjl[edge.orig].begin() + pos);
	this->numEdges--;
}

template <class T>
bool AdjacencyList<T>::edgeExists(Edge edge) const {
	if (edge.orig >= this->numNodes || edge.dest >= this->numNodes)
		return false;
	return getEdgeValue(origin, destination) != this->nullEdgeValue;
}

template <class T>
T AdjacencyList<T>::getEdgeValue(Edge edge) const {

	size_t pos = getEdgePos(edge);

	if (pos != this->numNodes)
		return adjl[edge.orig][pos].value;
	return this->nullEdgeValue;
}

template <class T>
size_t AdjacencyList<T>::getNodeDegree(size_t node) const {
	return this->getNeighbours(node).size();
}

template <class T>
GraphRepresentation<T>* AdjacencyList<T>::copy() const {
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
bool areEdgesEqual(Edge edge1, Edge edge2) {
	return edge1.orig == edge2.orig &&
		edge1.dest == edge2.dest &&
		edge1.value == edge2.value;
}

template <class T>
void AdjacencyList<T>::shrinkToFit() {
	for (size_t i = 0; i < this->numNodes; i++)
		this->adjl[i].shrink_to_fit();
}
