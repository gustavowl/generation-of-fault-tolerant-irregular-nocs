#ifndef __ADJACENCY_LIST__
#define __ADJACENCY_LIST__

#include "graph-representation.h"
#include <vector>

// This class is used for storing DIRECTED GRAPHS
// as adjacency lists.
template <class T>
class AdjacencyList : public GraphRepresentation<T> {
private:
	// The adjacency list itself
	std::vector<std::vector<Edge>> adjl;

	//returns edge position.
	//if edge does not exist, returns a invalid position
	//(number of nodes)
	size_t getEdgePos(Edge edge) const;

public:
	// Constructors
	AdjacencyList();
	//only initializes if numNodes > 0
	AdjacencyList(size_t numNodes, T nullEdgeValue=NULL);

	// Destructor
	~AdjacencyList(); 

	//only adds edges if adjl is not empty and arguments are valid,
	//and it does not exist already.
	//Not empty if constructor AdjacencyList(numNodes)
	//was called with valid arguments
	//Valid arguments if origin and destination in [0, numNodes)
	void addEdge(Edge edge);

	void delEdge(Edge edge);

	// returns if adjl[origin][destination] != nullEdgeValue
	bool edgeExists(Edge edge) const;

	T getEdgeValue(Edge edge) const;

	//returns the out-degree
	size_t getNodeDegree(size_t node) const;

	GraphRepresentation<T>* copy() const;

	bool areEdgesEqual(Edge edge1, Edge edge2);

	// This function should be called in order to
	// save memory usage. It is recommended to call it
	// after multiple calls to addEdge or delEdge.
	// For instance, it may be called after a loop finishes,
	// not in each iteration.
	void shrinkToFit();
};

#include "../adjacency-list.inl"

#endif
