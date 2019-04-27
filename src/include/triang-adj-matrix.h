#ifndef __TRIANG_ADJ_MATRIX__
#define __TRIANG_ADJ_MATRIX__

//class used by the Tabu Search.
//It stores the graph as an triangular adjacency matrix.
//The edges are symmetric and it does not have the
//identity column sine no self loops are allowed in the
//tabu search.
template <class T>
class TriangAdjMatrix {
public:
	// Constructor
	TriangAdjMatrix();

	// Destructor
	virtual ~TriangAdjMatrix();

	virtual void addEdge(Edge edge) = 0;

	virtual void delEdge(Edge edge) = 0;

	virtual bool edgeExists(Edge edge) const = 0;

	virtual T getEdgeValue(Edge edge) const = 0;

	virtual size_t getNodeDegree(size_t node) const = 0;

	virtual TriangAdjMatrix<T>* copy() const = 0;

	virtual bool areEdgesEqual(Edge edge1, Edge edge2) = 0;

	//returns a randomly chosen edge.
	//if exists is set to false, returns a edge that is NOT
	//in the graph.
	Edge selectRandomEdge(bool existent=true);

	//returns a randomly chosen edge with incident node incidentNode.
	//if exists is set to false, returns a edge that is NOT
	//in the graph AND is incident to incidentNode.
	//NOTE: IT DEPENDS ON THE IMPLEMENTATION OF getNodeDegree():
	//	it is supposed that it will return the in-degree + out-degree
	//override this method if necessary.
	Edge selectRandomEdge(size_t incidentNode,
			bool existent=true);

	//another random existing edge will be chosen and
	//The edge's incident nodes will be randomly swaped.
	//For instance, (1, 2), (3, 4) may be swapped to
	//(1, 4), (3, 2) or (1, 3), (4, 2).
	//This method is not executed if edge1 or edge2 exist in the graph
	//or if for all possible scenarios, one of the two swapped edges is
	//already in the graph (i.e. does not overwrite edges).
	//
	//The parameters are pointers because they will receive
	//the edges to be deleted and will have the value of
	//the added (swapped )edges by the end of the method.
	void swapEdgesNodes(Edge* edge1, Edge* edge2);
	
	//Swap a random edge of the node with another
	//For example, consider that edge (0, 5)
	//will be added. However, node 0 has degree 4: (0, 1),
	//(0, 2), (0, 3), and (0, 4). Choose one of these edges
	//randomly to be deleted (0, [1, 2, 3, 4]).
	//Choose another random node n and add the edge ([1, 2, 3, 4], n).
	//This process is hereby called "spin" since one node is fixed while
	//the other changes, like spinning a clock pointer.
	//
	//edge: the edge to be spinned.
	//fixedNode: central node. The spinning process will NOT change the
	//	edge's remaining node.
	//returns the edge after spinning (Thus, one node is different).
	Edge spinEdge(Edge edge, size_t fixedNode);
};

#include <iostream> //TODO: DELETEME (DEBUG)
#include "../graph-representation.inl"

#endif
