#ifndef __GRAPH_REPRESENTATION__
#define __GRAPH_REPRESENTATION__

#include <vector>
#include <time.h> //rng
#include <chrono> //rng
#include <random> //rng 
//rng macro
std::mt19937 rng(
		std::chrono::steady_clock::now().time_since_epoch().count());

// Abstract class for a graph representation.
// Contains methods that are overwritten by
// its sons.
template <class T>
class GraphRepresentation {
protected:
	size_t numNodes;
	size_t numEdges;
	
	// Value to be assigned to an Edge if it
	// is deleted, i.e. does not exist.
	T nullEdgeValue;
public:
	//struct used to represent an edge.
	//The sons may not necessarilly represent matrices using this
	//struct. But it is used for methods invocations.
	struct Edge {
		size_t orig; //origin node
		size_t dest; //destination node
		T value; //node weight
	};
	// Constructor
	GraphRepresentation();

	// Destructor
	virtual ~GraphRepresentation();

	size_t getNumNodes() const;
	size_t getNumEdges() const;

	virtual void addEdge(Edge edge) = 0;

	virtual void delEdge(Edge edge) = 0;

	virtual bool edgeExists(Edge edge) const = 0;

	virtual T getEdgeValue(Edge edge) const = 0;

	virtual size_t getNodeDegree(size_t node) const = 0;

	virtual GraphRepresentation<T>* copy() const = 0;

	//returns a vector containing the next step neighbours.
	//For an undirected graph:
	//	a----b----c
	//	getneighbours(b) shall return the list [a, c]
	//For a directed graph,
	//	a---->b---->c
	//	getneighbours(b) shall return the list [c]
	virtual std::vector<size_t> getNeighbours(size_t node) const;

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
	//(1, 4), (3, 2) or (1, 3), (4, 2), etc.
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
	
	//alias to isZeroOrder.
	bool isValid() const;

	T getNullEdgeValue() const;

	//TODO: DELETEME (DEBUG)
	void print() const;
};

#include <iostream> //TODO: DELETEME (DEBUG)
#include "../graph-representation.inl"

#endif
