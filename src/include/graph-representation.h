#ifndef __GRAPH_REPRESENTATION__
#define __GRAPH_REPRESENTATION__

#include <vector>

// Abstract class for a graph representation.
// Contains methods that are overwritten by
// its sons.
template <class T>
class GraphRepresentation {
protected:
	size_t numNodes;
	size_t numEdges;
	
	// Value to be assigned to an Edge if it
	// is deleted, i.e. does not exist.
	T nullEdgeValue;
public:
	// Constructor
	GraphRepresentation();

	// Destructor
	virtual ~GraphRepresentation();

	size_t getNumNodes();
	size_t getNumEdges();

	virtual void addEdge(size_t origin, size_t destination, T value) = 0;

	virtual void delEdge(size_t origin, size_t destination) = 0;

	virtual bool edgeExists(size_t origin, size_t destination) = 0;

	virtual T getEdgeValue(size_t origin, size_t destination) = 0;

	virtual size_t getNodeDegree(size_t node) = 0;

	virtual GraphRepresentation<T>* copy() = 0;

	//returns a vector containing the next step neighbours.
	//For an undirected graph:
	//	a----b----c
	//	getneighbours(b) shall return the list [a, c]
	//For a directed graph,
	//	a---->b---->c
	//	getneighbours(b) shall return the list [c]
	virtual std::vector<size_t> getNeighbours(size_t node);
	
	//alias to isZeroOrder.
	bool isValid();

	T getNullEdgeValue();

	//TODO: DELETEME (DEBUG)
	void print();
};

#include <iostream> //TODO: DELETEME (DEBUG)
#include "../graph-representation.inl"

#endif
