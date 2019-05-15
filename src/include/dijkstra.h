#ifndef __DIJKSTRA__
#define __DIJKSTRA__

#include "graph-representation.h"
#include <vector>
#include <limits>

//defines constant for infinity hops
//Alias for unreachable
#define HOP_INF std::numeric_limits<size_t>::max()

//Groups different versions of Dijkstra's algorithm
template <class T>
class Dijkstra {
public:
	struct Node {
		size_t nodeId;
		T weightSum;
		size_t hops;
		std::vector<size_t> shortPath; //shortest path
	};

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
	static Node dijkstra(
			const GraphRepresentation<T>* graph,
			size_t orig, size_t dest,
			T weightInf, bool isWeighted=true);

private:
	static Node extractMin(std::vector<Node>* minPriority, bool isWeighted=true);

	static void relaxWeighted(Node* orig, Node* dest, T weight, T weightInf);

	static void relaxUnweighted(Node* orig, Node* dest);
};

#include "../dijkstra.inl"

#endif
