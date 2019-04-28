#include "include/tabu-list.h"

template <class T>
TabuList<T>::TabuList() {
	index = 0;
	tabuList.reserve(1);
}

template <class T>
TabuList<T>::TabuList(size_t maxSize) {
	index = 0;
	tabuList.reserve(maxSize);
}

template <class T>
bool TabuList<T>::isTabu(grEdge edge) {
	//searches tabuList
	for (size_t i = 0; i < tabuList.size(); i++) {
		if (areEdgesEqual(tabuList[i], edge)) {
			return true;
		}
	}

	return false;
}

template <class T>
void TabuList<T>::add(grEdge edge) {
	if (tabuList->size() == tabuList->capacity()) {
		//cycle
		tabuList[index] = edge;
		index = (index + 1) % tabuList.size();
		return;
	}
	//tabuList is not full, no need to cycle
	tabuList->push_back(newEdge);
}
