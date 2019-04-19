#ifndef __ADJACENCY_LIST__
#define __ADJACENCY_LIST__

#include "graph-representation.h"
#include <vector>

// This class is used for storing DIRECTED GRAPHS
// as adjacency lists.
class AdjacencyList : public GraphRepresentation<T> {
private:
	//Struct used locally to represent an edge.
	//It has two fields: destination and weight.
	//The information about the origin is implicit
	//inside adjl vector.
	template <typename T>
	struct Edge {
		unsigned int dest; 
		T value;
	};
	// The adjacency list itself
	std::vector<std::vector<Edge<T>>> adjl;

public:
	// Constructors
	AdjacencyList();
	//only initializes if numNodes > 0
	AdjacencyList(unsigned int numNodes, T nullEdgeValue=NULL);

	// Destructor
	~AdjacencyList(); 

	//only adds edges if adjl is not empty and arguments are valid.
	//Not empty if constructor AdjacencyList(numNodes)
	//was called with valid arguments
	//Valid arguments if origin and destination in [0, numNodes)
	void addEdge(unsigned int origin, unsigned int destination,
			T value);

	void delEdge(unsigned int origin, unsigned int destination);

	bool edgeExists(unsigned int origin, unsigned int destination);

	T getEdgeValue(unsigned int origin, unsigned int destination);
};

#include "../adjacency-list.inl"

#endif
