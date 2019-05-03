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
void TabuList<T>::sort(std::vector<grEdge>* edges) {
	grEdge swap;
	size_t small;

	for (size_t i = 0; i < edges->size(); i++) {
		small = i;

		for (size_t j = i+1; j < edges->size(); j++) {
			if ( edges->at(j).orig < edges->at(small).orig ||
					(edges->at(j).orig == edges->at(small).orig &&
					edges->at(j).dest < edges->at(small).dest) ) {
				small = j;
			}
		}

		if (small != i) {
			swap = edges->at(i);
			edges->at(i) = edges->at(small);
			edges->at(small) = swap;
		}
	}
}

template <class T>
bool TabuList<T>::isTabu(grEdge edge) {
	std::vector<grEdge> vec;
	vec.push_back(edge);
	return this->isTabu(vec);
}

template <class T>
bool TabuList<T>::isTabu(std::vector<grEdge> edges) {
	this->sort(&edges);
	//searches tabuList
	bool comparison;
	for (size_t i = 0; i < tabuList.size(); i++) {
		if (edges.size() == tabuList[i].size()) {
			comparison = true;

			for (size_t j = 0; j < edges.size(); j++) {
				if (!edges[j].equalsTo(tabuList[i][j], false)) {
					comparison = false;
					break;
				}
			}

			if (comparison)
				return true;
		}
	}

	return false;
}

template <class T>
bool TabuList<T>::isTabu(GraphRepresentation<T>* graph) {
	bool comparison;

	for (size_t i = 0; i < tabuList.size(); i++) {
		comparison = true;
		std::vector<grEdge>* tabuItem = &(tabuList[i]);

		for (size_t j = 0; j < tabuItem->size(); j++) {
			if (!graph->edgeExists(tabuItem->at(j))) {
				comparison = false;
				break;
			}
		}
		
		if (comparison)
			return true;
	}
	return false;
}

template <class T>
void TabuList<T>::add(grEdge edge) {
	std::vector<grEdge> vec;
	vec.push_back(edge);
	this->add(vec);
}

template <class T>
void TabuList<T>::add(std::vector<grEdge> edges) {
	this->sort(&edges);

	if (this->cyclic && tabuList.size() == tabuList.capacity()) {
		//cycle
		tabuList[index] = edges;
		index = (index + 1) % tabuList.size();
		return;
	}
	//tabuList is not full, no need to cycle
	tabuList.push_back(edges);
}

template <class T>
size_t TabuList<T>::size() {
	return tabuList.size();
}

template <class T>
std::vector<grEdge> TabuList<T>::at(size_t index) const {
	return tabuList.at(index);
}
