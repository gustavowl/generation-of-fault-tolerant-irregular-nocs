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

	//TODO: this method only works if T = bool.
	//Since it depends on areEdgesEqual and it also
	//checks the edges' value. For the purpose of
	//this work, other implementation is not necessary.
	bool isInTabuList(grEdge edge);

	void addToTabuList(grEdge edge);
};
