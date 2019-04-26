#ifndef __TABU_LIST__
#define __TABU_LIST__

#include <vector>

template <class T>
class TabuList {
	std::vector<T> lst;
	size_t index;

	//TODO: set capacity
	TabuList();
	TabuList(size_t maxSize);

	bool isInTabuList(T value);

	void addToTabuList(T value);
};
