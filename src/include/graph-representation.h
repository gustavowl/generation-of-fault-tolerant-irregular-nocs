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

	size_t getNumNodes() const;
	size_t getNumEdges() const;

	virtual void addEdge(size_t origin, size_t destination, T value) = 0;

	virtual void delEdge(size_t origin, size_t destination) = 0;

	virtual bool edgeExists(size_t origin, size_t destination) const = 0;

	virtual T getEdgeValue(size_t origin, size_t destination) const = 0;

	virtual size_t getNodeDegree(size_t node) const = 0;

	virtual GraphRepresentation<T>* copy() const = 0;

	//returns a vector containing the next step neighbours.
	//For an undirected graph:
	//	a----b----c
	//	getneighbours(b) shall return the list [a, c]
	//For a directed graph,
	//	a---->b---->c
	//	getneighbours(b) shall return the list [c]
	virtual std::vector<size_t> getNeighbours(size_t node) const;
	
	//alias to isZeroOrder.
	bool isValid() const;

	T getNullEdgeValue() const;

	//TODO: DELETEME (DEBUG)
	void print() const;
};

#include <iostream> //TODO: DELETEME (DEBUG)
#include "../graph-representation.inl"

#endif
