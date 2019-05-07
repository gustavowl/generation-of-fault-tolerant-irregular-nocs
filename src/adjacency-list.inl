template <class T>
size_t AdjacencyList<T>::getEdgePos(const grEdge edge) const {

	if (edge.orig >= this->numNodes || edge.dest >= this->numNodes)
		return this->numNodes;

	for (size_t i = 0; i < adjl[edge.orig].size(); i++) {
		if (adjl[edge.orig][i].dest == edge.dest)
			return i;
	}

	return this->numNodes;
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
void AdjacencyList<T>::addEdge(grEdge edge) {

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
void AdjacencyList<T>::delEdge(grEdge edge) {

	if (!this->edgeExists(edge))
		return;

	size_t pos = getEdgePos(edge);

	adjl[edge.orig].erase(adjl[edge.orig].begin() + pos);
	this->numEdges--;
}

template <class T>
bool AdjacencyList<T>::edgeExists(grEdge edge) const {
	if (edge.orig >= this->numNodes || edge.dest >= this->numNodes)
		return false;
	return getEdgeValue(edge) != this->nullEdgeValue;
}

template <class T>
T AdjacencyList<T>::getEdgeValue(grEdge edge) const {

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
			ret->addEdge(grEdge {
						.orig = i,
						.dest = this->adjl[i][j].dest,
						.value = this->adjl[i][j].value
						});
		}
	}

	ret->shrinkToFit();

	return ret;
}

template <class T>
std::string AdjacencyList<T>::toStr() const {
	//TODO: implement AdjacencyList toStr()
	return "TODO: implement AdjacencyList toStr()";
}

template <class T>
bool AdjacencyList<T>::areEdgesEqual(grEdge edge1,
		grEdge edge2) {
	return edge1.equalsTo(edge2);
}

template <class T>
void AdjacencyList<T>::shrinkToFit() {
	for (size_t i = 0; i < this->numNodes; i++)
		this->adjl[i].shrink_to_fit();
}
