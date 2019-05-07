template <class T>
T Statistics<T>::getMinWeight() const {
	return this->minWeight;
}

template <class T>
T Statistics<T>::getMaxWeight() const {
	return this->maxWeight;
}

template <class T>
double Statistics<T>::getMeanWeight() const {
	return this->meanWeight;
}

template <class T>
double Statistics<T>::getStdDev() const {
	return this->stdDev;
}

template <class T>
T Statistics<T>::getFitness() const {
	return this->fitness;
}

template <class T>
bool Statistics<T>::isDisconnected() const {
	return this->disconnected;
}

template <class T>
Statistics<T>::Statistics(){
}

template <class T>
Statistics<T>::~Statistics(){
}

template <class T>
TabuAdjMatrix<T>* Statistics<T>::edgeCommCost(
		const GraphRepresentation<T>* tg,
		const TabuAdjMatrix<bool>* graph,
		T weightInf) {

	this->disconnected = false;
	size_t numNodes = graph->getNumNodes();// == tg->getNumNodes()
	TabuAdjMatrix<T>* costGraph = new TabuAdjMatrix<T>(numNodes,
			weightInf);

	grEdge cgEdge;
	boolEdge bEdge;
	cgEdge.value = 0;
	for (bEdge.orig = 1; bEdge.orig < numNodes; bEdge.orig++) {
		for (bEdge.dest = 0; bEdge.dest < bEdge.orig; bEdge.dest++) {
			if (graph->edgeExists(bEdge)) {
				cgEdge.orig = bEdge.orig;
				cgEdge.dest = bEdge.dest;
				costGraph->addEdge(cgEdge);
			}
		}
	}

	grEdge tgEdge;
	for (tgEdge.orig = 0; tgEdge.orig < numNodes; tgEdge.orig++) {
		for (tgEdge.dest = 0; tgEdge.dest < numNodes; tgEdge.dest++) {
			if (tg->edgeExists(tgEdge)) {
				//saves shortest path
				Dijkstra<bool>::Node ret = Dijkstra<bool>::dijkstra(
						graph, tgEdge.orig, tgEdge.dest,
						weightInf, false);

				if (ret.hops == HOP_INF) {
					this->disconnected = true;
					continue;
				}
				//updates the weight of the costGraph acording to
				//shortestPath found.
				//shortest path is a list of nodes.
				for (size_t i = 1; i < ret.shortPath.size(); i++) {
					cgEdge.orig = ret.shortPath[i - 1];
					cgEdge.dest = ret.shortPath[i];
					cgEdge.value += tg->getEdgeValue(tgEdge);
					costGraph->delEdge(cgEdge);
					costGraph->addEdge(cgEdge);
				}
			}
		}
	}	

	return costGraph;
}

template <class T>
void Statistics<T>::computeTopologyStats(
		const GraphRepresentation<T>* tg,
		const TabuAdjMatrix<bool>* topology,
		T weightInf) {
	//compute min and max edge weight
	T val;
	fitness = 0; //alias for weight sum
	bool firstEdgeFound = false;
	size_t numNodes = topology->getNumNodes();
	grEdge edge;

	TabuAdjMatrix<T>* graph = edgeCommCost(tg, topology, weightInf);

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
				stdDev += pow((double)graph->getEdgeValue(edge) -
						(double)meanWeight, 2);
			}
		}
	}

	stdDev = sqrt(stdDev / graph->getNumEdges());

	graph->print();

	delete graph;
}
