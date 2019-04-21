#ifndef __GRAPH_REPRESENTATION__
#define __GRAPH_REPRESENTATION__

#include <vector>

// Abstract class for a graph representation.
// Contains methods that are overwritten by
// its sons.
template <class T>
class GraphRepresentation {
protected:
	unsigned int numNodes;
	unsigned int numEdges;
	
	// Value to be assigned to an Edge if it
	// is deleted, i.e. does not exist.
	T nullEdgeValue;
public:
	// Constructor
	GraphRepresentation();

	// Destructor
	virtual ~GraphRepresentation();

	unsigned int getNumNodes();
	unsigned int getNumEdges();

	virtual void addEdge(unsigned int origin, unsigned int destination,
			T value) = 0;

	virtual void delEdge(unsigned int origin, unsigned int destination) = 0;

	virtual bool edgeExists(unsigned int origin, unsigned int destination) = 0;

	virtual T getEdgeValue(unsigned int origin, unsigned int destination) = 0;

	virtual GraphRepresentation<T>* copy() = 0;

	virtual std::vector<unsigned int> getNeighbours(unsigned int node);
	
	//alias to isZeroOrder.
	bool isValid();

	//TODO: DELETEME (DEBUG)
	void print();
};

#include <iostream> //TODO: DELETEME (DEBUG)
#include "../graph-representation.inl"

#endif
