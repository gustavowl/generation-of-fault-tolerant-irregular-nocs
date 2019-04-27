template <class T>
GraphRepresentation<T>::GraphRepresentation() {
	numNodes = numEdges = 0;
}

template <class T>
GraphRepresentation<T>::~GraphRepresentation() {
}

template <class T>
size_t GraphRepresentation<T>::getNumNodes() const {
	return numNodes;
}

template <class T>
size_t GraphRepresentation<T>::getNumEdges() const {
	return numEdges;
}

template <class T>
std::vector<size_t> GraphRepresentation<T>::getNeighbours(
		size_t node) const {

	size_t ncount = 0; //neighbours count
	std::vector<size_t> neighbours (this->numNodes);

	for (size_t dest = 0; dest < this->numNodes; dest++) {
		if (this->edgeExists( Edge{.orig = node, .dest = dest} ))
			neighbours[ncount++] = dest;
	}

	neighbours.resize(ncount);
	neighbours.shrink_to_fit();
	return neighbours;
}

template <class T>
bool GraphRepresentation<T>::isValid() const {
	return numNodes != 0;
}

template <class T>
T GraphRepresentation<T>::getNullEdgeValue() const {
	return nullEdgeValue;
}

template <class T>
void GraphRepresentation<T>::print() const {
	std::string linesep = "=============================================";
	std::cout << linesep << std::endl;
	std::cout << numNodes << " nodes, " << numEdges << " edges\n\n";
	for (size_t orig = 0; orig < numNodes; orig++) {
		for (size_t dest = 0; dest < numNodes; dest++) {
			if (!edgeExists( Edge{.orig = orig, .dest = dest} ))
				continue;
			std::cout << "Edge from " << orig <<
				" to " << dest << ". " <<
				" Value = " << getEdgeValue(orig, dest) <<
				std::endl;
		}
	}

	std::cout << '\n';
	for (size_t i = 0; i < numNodes; i++)
		std::cout << "Node " << i << " degree = " <<
			this->getNodeDegree(i) << std::endl;

	std::cout << linesep << std::endl;
}

template <class T>
typename GraphRepresentation<T>::Edge
GraphRepresentation<T>::selectRandomEdge(bool existent) {
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
typename GraphRepresentation<T>::Edge
GraphRepresentation<T>::selectRandomEdge(size_t incidentNode,
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
void GraphRepresentation<T>::swapEdgesNodes(Edge* edge1, Edge* edge2) {
}

template <class T>
typename GraphRepresentation<T>::Edge
GraphRepresentation<T>::spinEdge(Edge edge, size_t fixedNode) {
	return Edge {.orig = 0, .dest = 0};
}
