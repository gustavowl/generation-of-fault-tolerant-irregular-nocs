#ifndef __ADJACENCY_LIST__
#define __ADJACENCY_LIST__

#include "graph-representation.h"
#include <vector>

// This class is used for storing DIRECTED GRAPHS
// as adjacency lists.
template <class T>
class AdjacencyList : public GraphRepresentation<T> {
private:
	//Struct used locally to represent an edge.
	//It has two fields: destination and weight.
	//The information about the origin is implicit
	//inside adjl vector.
	struct Edge {
		size_t dest; 
		T value;
	};
	// The adjacency list itself
	std::vector<std::vector<Edge>> adjl;

	// returns a NULL pointer if either origin or destination
	// are out of the range [0, numNodes); or if
	// adjl[origin][destination == nullEdgeValue
	Edge* searchEdge(size_t origin, size_t destination);
	// returns edge position.
	// only call this method if edge is in adjl.
	size_t getEdgePos(size_t origin, Edge* edge);

public:
	// Constructors
	AdjacencyList();
	//only initializes if numNodes > 0
	AdjacencyList(size_t numNodes, T nullEdgeValue=NULL);

	// Destructor
	~AdjacencyList(); 

	//only adds edges if adjl is not empty and arguments are valid.
	//Not empty if constructor AdjacencyList(numNodes)
	//was called with valid arguments
	//Valid arguments if origin and destination in [0, numNodes)
	void addEdge(size_t origin, size_t destination,
			T value);

	void delEdge(size_t origin, size_t destination);

	// returns if adjl[origin][destination] != nullEdgeValue
	bool edgeExists(size_t origin, size_t destination);

	T getEdgeValue(size_t origin, size_t destination);

	//returns the out-degree
	size_t getNodeDegree(size_t node);

	GraphRepresentation<T>* copy();

	// This function should be called in order to
	// save memory usage. It is recommended to call it
	// after multiple calls to addEdge or delEdge.
	// For instance, it may be called after a loop finishes,
	// not in each iteration.
	void shrinkToFit();
};

#include "../adjacency-list.inl"

#endif
