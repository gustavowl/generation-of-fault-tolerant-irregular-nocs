#ifndef __TABU_LIST__
#define __TABU_LIST__

#include "graph-representation.h"
#include <vector>

template <class T>
class TabuList {

private:
	std::vector<std::vector<grEdge>> tabuList;
	size_t index; //used for simulating circular queue
	bool cyclic;

	void sort(std::vector<grEdge>* edges);

public:
	//TODO: set capacity
	TabuList();
	TabuList(size_t maxSize, bool cyclic=true);

	~TabuList();

	//verifies is edge is in tabuList. Only the origin
	//and destination nodes are considered, not the
	//edge's weight/value.
	bool isTabu(grEdge edge);

	bool isTabu(std::vector<grEdge> edges);

	//verifies if graph contains tabu edges
	bool isTabu(GraphRepresentation<T>* graph);

	void add(grEdge edge);

	void add(std::vector<grEdge> edge);

	size_t size();

	std::vector<grEdge> at(size_t index) const;
};

#include "../tabu-list.inl"

#endif
