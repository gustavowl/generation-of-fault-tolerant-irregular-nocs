#include <iostream> //TODO: DELETE ME

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

	std<T>::cout << origin << ',' << destination << ',' << value << std::endl;
}

template <class T>
void AdjacencyList<T>::delEdge(unsigned int origin, unsigned int destination) {
	//numEdges--;
	//TODO
}

template <class T>
bool AdjacencyList<T>::edgeExists(unsigned int origin, unsigned int destination) {
	return false;
}

template <class T>
T AdjacencyList<T>::getEdgeValue(unsigned int origin, unsigned int destination) {
	return NULL;
}
