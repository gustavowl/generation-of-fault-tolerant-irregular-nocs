#ifndef __GRAPH_REPRESENTATION__
#define __GRAPH_REPRESENTATION__

// Abstract class for a graph representation.
// Contains methods that are overwritten by
// its sons.
class GraphRepresentation {
protected:
	unsigned int numNodes;
	unsigned int numEdges;
public:
	// Constructor
	GraphRepresentation();

	// Destructor
	virtual ~GraphRepresentation();

	unsigned int getNumNodes();
	unsigned int getNumEdges();

	virtual void addEdge(unsigned int origin, unsigned int destination,
			unsigned int weight) = 0;

	virtual void delEdge(unsigned int origin, unsigned int destination) = 0;
};

#endif
