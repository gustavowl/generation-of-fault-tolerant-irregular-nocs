template <class T>
typename TriangAdjMatrix<T>::Edge
TriangAdjMatrix<T>::selectRandomEdge(bool existent) {
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
typename TriangAdjMatrix<T>::Edge
TriangAdjMatrix<T>::selectRandomEdge(size_t incidentNode,
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
void TriangAdjMatrix<T>::swapEdgesNodes(Edge* edge1, Edge* edge2) {
	//there are two possible swap for edges (1, 2), (3, 4)
	//	(1, 4), (3, 2) or (1, 3), (2, 4)
}

template <class T>
typename TriangAdjMatrix<T>::Edge
TriangAdjMatrix<T>::spinEdge(Edge edge, size_t fixedNode) {
	return Edge {.orig = 0, .dest = 0};
}
