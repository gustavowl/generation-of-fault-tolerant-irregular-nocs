#ifndef __ADJACENCY_MATRIX__
#define __ADJACENCY_MATRIX__

#include "graph-representation.h"
#include <vector>

template <class T>
// This class is used for representing square adjacency matrices.
class AdjacencyMatrix : public GraphRepresentation {

private:
	bool isSymmetric;
	//if matrix is symetric and triangular is set to true,
	//it will only store the lower triangular matrix
	bool isTriangular; 
	//value to be assigned when and Edge is deleted
	T noEdgeValue;

	// The adjacency matrix itself:
	// adjm[origin][destiny] = weight;
	std::vector<std::vector<T>> adjm;

	//conditional triangular matrix Node Id Swap:
	//swaps the nodes ids if matrix is triangular and
	//origin > destination (equivalent to attempting to access
	//the upper triangle matrix)
	void triangNodeIdSwap(unsigned int* origin, unsigned int* destination);

public:
	// Constructor
	AdjacencyMatrix();
	//expects numNodes > 0 and that triangular -> symmetric
	AdjacencyMatrix(unsigned int numNodes, bool symmetric=false,
			bool triangular=false, T noEdgeValue=0);

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
