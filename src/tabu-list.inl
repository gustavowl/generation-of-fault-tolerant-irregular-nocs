#include "include/tabu-list.h"

template <class T>
TabuList<T>::TabuList() {
	index = 0;
	lst.reserve(1);
}

template <class T>
TabuList<T>::TabuList(size_t maxSize) {
	index = 0;
	lst.reserve(maxSize);
}

template <class T>
bool TabuList<T>::isInTabuList(const std::vector<size_t*>* tabuList,
		size_t* edge) {
	//searches tabuList
	size_t tabuEdge[2];
	for (size_t i = 0; i < tabuList->size(); i++) {
		tabuEdge[0] = tabuList->at(i)[0];
		tabuEdge[1] = tabuList->at(i)[1];
		
		if (areEdgesEqual(tabuEdge, edge)) {
			return true;
		}
	}

	return false;
}

template <class T>
void TabuList<T>::addToTabuList(std::vector<size_t*>* tabuList,
		size_t* tabuIndex, Movement mov) {
	if (tabuList->size() == tabuList->capacity()) {
		//cycle
		tabuList->at(*tabuIndex)[0] = mov.edgeToDel[0];
		tabuList->at(*tabuIndex)[1] = mov.edgeToDel[1];
		*tabuIndex = (*tabuIndex + 1) % tabuList->size();

		return;
	}
	//tabuList is not full, no need to cycle
	size_t* newEdge = new size_t[2];
	newEdge[0] = mov.edgeToDel[0];
	newEdge[1] = mov.edgeToDel[1];
	tabuList->push_back(newEdge);
}
