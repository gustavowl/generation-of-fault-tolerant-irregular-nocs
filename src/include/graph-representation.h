#ifndef __GRAPH_REPRESENTATION__
#define __GRAPH_REPRESENTATION__

//use this as a shortcut in other template classes
#define grEdge typename GraphRepresentation<T>::Edge

#include <vector>
#include "rng.h"

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
	//NOTE: used the grEdge definition as a shortcut in other classes
	struct Edge { //TODO: move to class file
		size_t orig; //origin node
		size_t dest; //destination node
		T value; //node weight

		bool equalsTo(Edge edge, bool checkValue=true) {
			bool ret = this->orig == edge.orig && this->dest == edge.dest;
			if (checkValue)
				return ret && this->value == edge.value;
			return ret; 
		}
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

	//compares if edges are euqal accordingly to representation,
	//e.g. edges (0, 1), (1, 0) are different in directed graphs,
	//but equal in undirected graphs.
	virtual bool areEdgesEqual(Edge edge1, Edge edge2) = 0;
	
	//alias to isZeroOrder.
	bool isValid() const;

	T getNullEdgeValue() const;

	//TODO: DELETEME (DEBUG)
	void print() const;
};

#include <iostream> //TODO: DELETEME (DEBUG)
#include "../graph-representation.inl"

#endif
