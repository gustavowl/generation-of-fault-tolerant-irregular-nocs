template <class T>
T Statistics<T>::getMinWeight() const {
	return this->minWeight;
}

template <class T>
T Statistics<T>::getMaxWeight() const{
	return this->maxWeight;
}

template <class T>
double Statistics<T>::getMeanWeight() const{
	return this->meanWeight;
}

template <class T>
double Statistics<T>::getStdDev() const{
	return this->stdDev;
}

template <class T>
T Statistics<T>::getFitness(){
	return this->fitness;
}

template <class T>
Statistics<T>::Statistics(){
}

template <class T>
Statistics<T>::~Statistics(){
}

template <class T>
void Statistics<T>::computeGraphStats(TabuAdjMatrix<T>* graph){
	//compute min and max edge weight
	T val;
	fitness = 0; //alias for weight sum
	bool firstEdgeFound = false;
	size_t numNodes = graph->getNumNodes();
	grEdge edge;

	for (edge.orig = 1; edge.orig < numNodes; edge.orig++) {
		for (edge.dest = 0; edge.dest < edge.orig; edge.dest++) {
			if (graph->edgeExists(edge)) {
				val = graph->getEdgeValue(edge);
				if (firstEdgeFound) {
					maxWeight = (val > maxWeight)? val : maxWeight;
					minWeight = (val < minWeight)? val : minWeight;
				}
				else {
					minWeight = val;
					maxWeight = val;
					firstEdgeFound = true;
				}

				fitness += val;
			}
		}
	}

	meanWeight = fitness / (double) graph->getNumEdges();

	//computes standard deviation
	stdDev = 0;
	for (edge.orig = 1; edge.orig < numNodes; edge.orig++) {
		for (edge.dest = 0; edge.dest < edge.orig; edge.dest++) {
			if (graph->edgeExists(edge)) {
				stdDev += pow(graph->getEdgeValue(edge) - meanWeight, 2);
			}
		}
	}
	stdDev = stdDev / (graph->getNumEdges() - 1);
}
