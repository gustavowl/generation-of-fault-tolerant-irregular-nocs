template <class T>
TabuList<T>::TabuList() {
	index = 0;
	cyclic = false;
}

template <class T>
TabuList<T>::TabuList(size_t maxSize, bool cyclic) {
	index = 0;
	tabuList.reserve(maxSize);
	this->cyclic = cyclic;
}

template <class T>
TabuList<T>::~TabuList() {
}

template <class T>
bool TabuList<T>::isTabu(grEdge edge) {
	//searches tabuList
	for (size_t i = 0; i < tabuList.size(); i++) {
		if (edge.equalsTo(tabuList[i]), false) {
			return true;
		}
	}

	return false;
}

template <class T>
void TabuList<T>::add(grEdge edge) {

	if (this->cyclic && tabuList.size() == tabuList.capacity()) {
		//cycle
		tabuList[index] = edge;
		index = (index + 1) % tabuList.size();
		return;
	}
	//tabuList is not full, no need to cycle
	tabuList.push_back(edge);
}

template <class T>
size_t TabuList<T>::size() {
	return tabuList.size();
}

template <class T>
grEdge TabuList<T>::at(size_t index) const {
	return tabuList.at(index);
}
