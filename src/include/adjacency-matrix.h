#ifndef __ADJACENCY_MATRIX__
#define __ADJACENCY_MATRIX__

#include "graph-representation.h"
#include <vector>

template <class T>
// This class is used for representing square adjacency matrices.
class AdjacencyMatrix : public GraphRepresentation {

private:
	bool isSymmetric;
	//if matrix is symetric and shortRep is set to true,
	//it will only store the lower triangular matrix
	bool shortRep; 

	// The adjacency matrix itself:
	// adjm[origin][destiny] = weight;
	std::vector<std::vector<T>> adjm;

public:
	// Constructor
	AdjacencyMatrix();
	//expects numNodes > 0 and that shortRep -> symmetric
	AdjacencyMatrix(unsigned int numNodes, bool symmetric=false,
			bool shortRep=false);

	// Destructor
	~AdjacencyMatrix(); 

	//adds edge for valid values of origin and destination:
	//[0, numNodes). Adds edge if weight > 0.
	void addEdge(unsigned int origin, unsigned int destination,
			unsigned int weight);

	//deletes edge if origin and destination values are in
	//the valid range: [0, numNodes)
	void delEdge(unsigned int origin, unsigned int destination);

	bool edgeExists(unsigned int origin, unsigned int destination);

	T getEdgeWeight(unsigned int origin, unsigned int destination);
};

#include "../adjacency-matrix.inl"

#endif
