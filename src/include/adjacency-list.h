#ifndef __ADJACENCY_LIST__
#define __ADJACENCY_LIST__

#include "graph-representation.h"

// This class is used for representing adjacency lists.
class AdjacencyList : public GraphRepresentation {

public:
	// Constructor
	AdjacencyList();

	// Destructor
	~AdjacencyList(); 

	void addEdge(unsigned int origin, unsigned int destination,
			unsigned int weight);

	void delEdge(unsigned int origin, unsigned int destination);
};

#endif
