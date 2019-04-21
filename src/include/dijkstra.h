#ifndef __DIJKSTRA__
#define __DIJKSTRA__

#include "graph-representation.h"
#include <vector>
#include <limits>

//Groups different versions of Dijkstra's algorithm
template <class T>
class Dijkstra {
public:
	struct Node {
		unsigned int nodeId;
		T weightSum;
		unsigned int hops;
	}

	//Dijkstra's Algorithm interface.
	//graph is the graph representation itself.
	//orig: origin node.
	//deset: destination node.
	//weightInf: the value correspondent to infinity for type T.
	//isWeighted: states whether the graph is weighted or not.
	//If false; then dijkstra's algorithm can return the first
	//path found (pruning the search space).
	//
	//Returns a node struct that contains:
	//reached node id (assert that nodeId == dest),
	//weightSum (sum of the weights from orig to dest),
	//and hops (number of hops from orig to dest).
	static node dijkstra(
			const GraphRepresentation<T>* graph,
			unsigned int orig, unsigned int dest,
			bool isWeighted=true);

private:
	bool isWeighted;

	static node extractMin(std::vector<node>* minPriority, T weightInf,
			bool isWeighted=true);

	static void relaxWeighted(Node* orig, Node* dest, T weight);

	static void relaxUnweighted(Node* orig, Node* dest);
};

#include "../dijkstra.inl"

#endif
