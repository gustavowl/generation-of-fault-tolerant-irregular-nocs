#ifndef __ADJACENCY_LIST__
#define __ADJACENCY_LIST__

#include "graph-representation.h"
#include <vector>

// This class is used for representing adjacency lists.
class AdjacencyList : public GraphRepresentation {
private:
	//Struct used locally to represent an edge.
	//It has two fields: destination and weight.
	//The information about the origin is implicit
	//inside adjl vector.
	struct Edge {
		unsigned int dest; 
		unsigned int weight;
	};
	// The adjacency list itself
	std::vector<std::vector<Edge>> adjl;

public:
	// Constructors
	AdjacencyList();
	//only initializes if numNodes > 0
	AdjacencyList(unsigned int numNodes);

	// Destructor
	~AdjacencyList(); 

	//only adds edges if adjl is not empty and arguments are valid.
	//Not empty if constructor AdjacencyList(numNodes)
	//was called with valid arguments
	//Valid arguments if origin and destination in [0, numNodes)
	void addEdge(unsigned int origin, unsigned int destination,
			unsigned int weight);

	void delEdge(unsigned int origin, unsigned int destination);
};

#endif
