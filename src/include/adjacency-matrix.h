#ifndef __ADJACENCY_MATRIX__
#define __ADJACENCY_MATRIX__

#include "graph-representation.h"
#include <vector>

// This class is used for representing graphs
// as square adjacency matrices.
//
// NOTE: In order for this implementation to work
// with 0-weighted edges, it will be probably
// recommended to create a struct to store it.
// Simply because 0 would be the standard value
// for nullEdgeValue.
// The nullEdgeValue affects the behaviour of
// edgeExists() and delEdge().
// The value of nullEdgeValue can be changed in
// the constructor.
template <class T>
class AdjacencyMatrix : public GraphRepresentation<T> {

private:
	bool isSymmetric;
	//if matrix is symetric and triangular is set to true,
	//it will only store the lower triangular matrix
	bool isTriangular; 

	// The adjacency matrix itself:
	// adjm[origin][destiny] = weight;
	std::vector<std::vector<T>> adjm;

	//conditional triangular matrix Node Id Swap:
	//swaps the nodes ids if matrix is triangular and
	//origin < destination (equivalent to attempting to access
	//the upper triangle matrix)
	void triangNodeIdSwap(unsigned int* origin, unsigned int* destination);

	//Called when constructor is unable to allocate memory.
	//If any element (edge or node) was added,
	//it is destroyed.
	void setInvalid();

public:
	// Constructor
	AdjacencyMatrix();
	//expects numNodes > 0 and that triangular -> symmetric
	AdjacencyMatrix(unsigned int numNodes, bool symmetric=false,
			bool triangular=false, T nullEdgeValue=NULL);

	// Destructor
	~AdjacencyMatrix(); 

	//adds edge for valid values of origin and destination:
	//[0, numNodes). Adds edge if value != nullEdgeValue.
	void addEdge(unsigned int origin, unsigned int destination, T value);

	//deletes edge if origin and destination values are in
	//the valid range: [0, numNodes)
	void delEdge(unsigned int origin, unsigned int destination);

	// returns if adjm[origin][destination] != nullEdgeValue
	bool edgeExists(unsigned int origin, unsigned int destination);

	T getEdgeValue(unsigned int origin, unsigned int destination);
};

#include "../adjacency-matrix.inl"

#endif
