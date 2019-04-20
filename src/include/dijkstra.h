#ifndef __DIJKSTRA__
#define __DIJKSTRA__

#include "graph-representation.h"
#include <vector>

//Groups different versions of Dijkstra's algorithm
class Dijkstra {
public:

	//Dijkstra's Algorithm interface.
	//graph is the graph representation itself.
	//orig: origin node.
	//deset: destination node.
	//isWeight states whether the graph is weighted or not.
	//If false; then dijkstra's algorithm can return the first
	//path found.
	//
	//It returns a vector pointer containing the shortest path
	//found. NULL is returned if no path is found.
	//NOTE: remember to deallocate pointer.
	template <class T>
	static vector<unsigned int>* dijkstra(
			const GraphRepresentation<T>* graph,
			unsigned int orig, unsigned int dest,
			bool isWeighted=true);

private:
	//implementation of weighted Dijkstra's Algorithm
	template <class T>
	static vector<unsigned int>* weighted(
			const GraphRepresentation<T>* graph,
			unsigned int orig, unsigned int dest);

	//implementation of unweighted Dijkstr'as Algorithm:
	//returns once the first path is found.
	template <class T>
	static vector<unsigned int>* unweighted(
			const GraphRepresentation<T>* graph,
			unsigned int orig, unsigned int dest);
};

#include "../dijkstra.inl"

#endif
