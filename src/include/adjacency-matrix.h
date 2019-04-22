#ifndef __ADJACENCY_MATRIX__
#define __ADJACENCY_MATRIX__

#include "graph-representation.h"

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
	T** adjm;

	//stores the degrees of the vertices (reger to getNodeDegree())
	size_t* degrees;

	//conditional triangular matrix Node Id Swap:
	//swaps the nodes ids if matrix is triangular and
	//origin < destination (equivalent to attempting to access
	//the upper triangle matrix)
	void triangNodeIdSwap(size_t* origin, size_t* destination) const;

	//Called when constructor is unable to allocate memory.
	//If any element (edge or node) was added,
	//it is destroyed.
	void setInvalid();

public:
	// Constructor
	AdjacencyMatrix();
	//expects numNodes > 0 and that triangular -> symmetric
	AdjacencyMatrix(size_t numNodes, bool symmetric=false,
			bool triangular=false, T nullEdgeValue=NULL);

	// Destructor
	~AdjacencyMatrix(); 

	//adds edge for valid values of origin and destination:
	//[0, numNodes). Adds edge if value != nullEdgeValue.
	//and if an edge from origin to destination does not
	//exist yet.
	void addEdge(size_t origin, size_t destination, T value);

	//deletes edge if origin and destination values are in
	//the valid range: [0, numNodes).
	void delEdge(size_t origin, size_t destination);

	// returns if adjm[origin][destination] != nullEdgeValue
	bool edgeExists(size_t origin, size_t destination) const;

	T getEdgeValue(size_t origin, size_t destination) const;

	//returns the number of incident edges. For both directed (asymmetric)
	//and undirected (symmetric) graphs. For directed graphs, it computes
	//in-degree + out-degree.
	size_t getNodeDegree(size_t node) const;

	GraphRepresentation<T>* copy() const;
};

#include "../adjacency-matrix.inl"

#endif
