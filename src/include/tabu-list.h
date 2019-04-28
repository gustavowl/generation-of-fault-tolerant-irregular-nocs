#ifndef __TABU_LIST__
#define __TABU_LIST__

#include "graph-representation.h"
#include <vector>

template <class T>
class TabuList {
	std::vector<grEdge> tabuList;
	size_t index; //used for simulating circular queue

	//TODO: set capacity
	TabuList();
	TabuList(size_t maxSize);

	//verifies is edge is in tabuList. Only the origin
	//and destination nodes are considered, not the
	//edge's weight/value.
	bool isTabu(grEdge edge);

	void add(grEdge edge);

	size_t size();
};
