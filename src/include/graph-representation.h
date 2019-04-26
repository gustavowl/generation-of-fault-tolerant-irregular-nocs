#ifndef __GRAPH_REPRESENTATION__
#define __GRAPH_REPRESENTATION__

#include <vector>

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
	// Constructor
	GraphRepresentation();

	// Destructor
	virtual ~GraphRepresentation();

	size_t getNumNodes() const;
	size_t getNumEdges() const;

	virtual void addEdge(size_t origin, size_t destination, T value) = 0;

	virtual void delEdge(size_t origin, size_t destination) = 0;

	virtual bool edgeExists(size_t origin, size_t destination) const = 0;

	virtual T getEdgeValue(size_t origin, size_t destination) const = 0;

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

	virtual bool areEdgesEqual(size_t orig1, size_t dest1,
			size_t orig2, size_t dest2) = 0;

	//returns a randomly chosen edge.
	//if exists is set to false, returns a edge that is NOT
	//in the graph.
	//Remember to deallocate the returned pointer.
	size_t* selectRandomEdge(bool existent=true) = 0;

	//returns a randomly chosen edge incident to incidentNode.
	//if exists is set to false, returns a edge that is NOT
	//in the graph AND incident to incidentNode.
	//Remember to deallocate the returned pointer.
	size_t* selectRandomEdge(size_t incidentNode,
			bool existent=true) = 0;

	//another random existing edge will be chosen and
	//The edge's incident nodes will be randomly swaped.
	//For instance, (1, 2), (3, 4) may be swapped to
	//(1, 4), (3, 2) or (1, 3), (4, 2), etc.
	static void swapEdgesNodes(AdjacencyMatrix<bool>* neighbour,
			Movement mov, std::vector<size_t>* tabuList, bool aspirationCrit);
	
	//Swap a random edge of the node with another
	//For example, consider that edge (0, 5)
	//will be added. However, node 0 has degree 4: (0, 1),
	//(0, 2), (0, 3), and (0, 4). Choose one of these edges
	//randomly to be deleted (0, [1, 2, 3, 4]).
	//Choose another random node n and add the edge ([1, 2, 3, 4], n).
	//This process is hereby called "spin" since one node is fixed while
	//the other changes, like spinning a clock pointer.
	static void spinEdge(size_t* edge, size_t fixedNode);
	
	//alias to isZeroOrder.
	bool isValid() const;

	T getNullEdgeValue() const;

	//TODO: DELETEME (DEBUG)
	void print() const;
};

#include <iostream> //TODO: DELETEME (DEBUG)
#include "../graph-representation.inl"

#endif
