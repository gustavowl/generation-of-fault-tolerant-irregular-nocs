#ifndef __TRIANG_ADJ_MATRIX__
#define __TRIANG_ADJ_MATRIX__

#include "graph-representation.h"

//class used by the Tabu Search.
//It stores the graph as a lower triangular adjacency matrix.
//All the elements are below the main diagonal.
//The edges are symmetric and the matrix does not have the
//main diagonal since no self loops are allowed in the
//tabu search.
template <class T>
class TabuAdjMatrix : public GraphRepresentation<T> {
private:
	T** adjm;

	//stores the nodes' degree 9refer to getNodeDegree())
	size_t* degrees;

	//swaps the nodes' ids to fit in the format of the
	//triangular matrix. For instance, (0, 1) is mapped
	//to (1, 0) while (1, 0) would remain unchanged.
	void nodeIdSwap(grEdge* edge) const;

	//Called by destructor.
	//Called when constructors are unable to allocate memory.
	//If any element (edge or node) was added, it is destroyed.
	//Deallocades both adjm and degrees pointers.
	void setInvalid();

	//returns if an edge is valid.
	//If the nodes' ids are in the limited range (< numNodes),
	//if edge is not a self-loop and if edge's value is
	//valid (!= nullEdgeValue).
	//If checkValue is set to false, then it does not verify
	//whether value != nullEdgeValue
	bool isEdgeInvalid(grEdge edge, bool checkValue=true) const;

	grEdge generateInvalidEdge();

	//returns the n-th node with largest or smallest degree from the
	//list sent.
	size_t getNodeWithNthDegreeFromList(std::vector<size_t> nodes,
			size_t rankPos, bool largest);
public:
	// Constructors
	TabuAdjMatrix();
	//expects numNodes > 1
	TabuAdjMatrix(size_t numNodes, T nullEdgeValue);

	// Destructor
	virtual ~TabuAdjMatrix();

	virtual void addEdge(grEdge edge);

	virtual void delEdge(grEdge edge);

	virtual bool edgeExists(grEdge edge) const;

	virtual T getEdgeValue(grEdge edge) const;

	virtual size_t getNodeDegree(size_t node) const;

	virtual GraphRepresentation<T>* copy() const;

	virtual bool areEdgesEqual(grEdge edge1, grEdge edge2);

	//============================================
	//======= METHODS USED FOR TABU SEARCH =======
	//============================================

	//returns a randomly chosen edge.
	//if exists is set to false, returns a edge that is NOT
	//in the graph.
	grEdge selectRandomEdge(bool existent=true);

	//returns a randomly chosen edge with incident node incidentNode.
	//if exists is set to false, returns a edge that is NOT
	//in the graph AND is incident to incidentNode.
	//NOTE: IT DEPENDS ON THE IMPLEMENTATION OF getNodeDegree():
	//	it is supposed that it will return the in-degree + out-degree
	//override this method if necessary.
	grEdge selectRandomEdge(size_t incidentNode,
			bool existent=true);

	//Same as selectRandomEdge(incidentNode, existent), but
	//it selects a target node with degree < degreeUpLim (upper
	//degree limit)
	grEdge selectRandomEdge(size_t incidentNode, size_t upperDegLim,
			bool existent=true);

	//another random existing edge will be chosen and
	//The edge's incident nodes will be randomly swaped.
	//For instance, (1, 2), (3, 4) may be swapped to
	//(1, 4), (3, 2) or (1, 3), (4, 2).
	//This method is does not change the graph if either edge1 or edge2
	//already exist in the graph or if for all possible scenarios,
	//one of the two swapped edges is already in the graph
	//(i.e. does not overwrite edges) or invalid.
	//The edges values are also considered for swapping. Thus,
	//swapping edges (1, 2, 2112), and (3, 4, 73) may result
	//in both (1, 4, 2112), (3, 2, 73) or (1, 4, 73), (3, 2, 2112),
	//totalling four possible scenarios.
	//
	//The parameters are pointers because they will receive
	//the edges to be deleted and will have the value of
	//the added (swapped) edges by the end of the method.
	//If the graph remains unchanged, the final values of edge1, and
	//edge2 will be invalid.
	void swapEdgesNodes(grEdge* edge1, grEdge* edge2);
	
	//Swap a random edge of the node with another
	//For example, consider that edge (0, 5)
	//will be added. However, node 0 has degree 4: (0, 1),
	//(0, 2), (0, 3), and (0, 4). Choose one of these edges
	//randomly to be deleted (0, [1, 2, 3, 4]).
	//Choose another random node n and add the edge ([1, 2, 3, 4], n).
	//This process is hereby called "spin" since one node is fixed while
	//the other changes, like spinning a clock pointer.
	//After this method is called, the resulting graph is different
	//from the original, unless the values of edge and fixedNode are
	//invalid.
	//
	//edge: the edge to be spinned.
	//fixedNode: central node. The spinning process will NOT change the
	//	edge's remaining node.
	//returns the edge after spinning. It will be invalid if the
	//	arguments are invalid as well.
	grEdge spinEdge(grEdge edge, size_t fixedNode);

	//same as spinEdge, but a node with degree < upperDegLim is chosen.
	//For instance, consider a graph with 6 nodes and nullEdgeValue = 0:
	//0: NULL
	//1: 7
	//2: 0 0
	//3: 0 0 2
	//4: 0 0 3 0
	//5: 0 0 4 5 9
	//The nodes have degrees [1, 1, 3, 2, 2, 3], respectively.
	//Spinning edge (1, 0) with fixed node = 0 and degreeUpLim = 3
	//may result in either edge (0, 3) or (0, 4).
	//If no spin is possible, the graph will remain unchanged.
	//
	//edge: the edge to be spinned.
	//fixedNode: central node. The spinning process will NOT change the
	//	edge's remaining node.
	//returns the edge after spinning. It there is no valid spin,
	//	the edge will be invalid.
	grEdge spinEdge(grEdge edge, size_t fixedNode, size_t upperDegLim);

	//Returns the node with n-th smallest or largest degree.
	//The rankPos determines the n-th position in the [0, numNodes - 1]
	//range. When boolean is set to true, returns the n-th largest, and
	//n-th smallest otherwise. For instance, consider 10 nodes with the
	//following degrees: [8, 4, 8, 1, 1, 4, 2, 4, 5, 7].
	//getNodeWithNthDegree(0, true) = 0;
	//getNodeWithNthDegree(1, true) = 2;
	//getNodeWithNthDegree(0, false) = 3;
	//getNodeWithNthDegree(3, false) = 1.
	size_t getNodeWithNthDegree(size_t rankPos, bool largest);

	//Similar to getNodeWithNthDegree, but instead of calculating from
	//all the nodes, it only considers the neighbours of incidentNode.
	size_t getNeighbourWithNthDegree(size_t rankPos, size_t incidentNode,
			bool largest);
};

#include "../tabu-adj-matrix.inl"

#endif
