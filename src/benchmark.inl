template <class T>
TabuAdjMatrix<bool>* Benchmark<T>::failLinks(double perc) {

	if (perc < 0 || perc > 1)
		return NULL;

	TabuAdjMatrix<bool>* failed = topology->copy();
	boolEdge edgeToFail;
	TabuList<bool> emptyTabuList;
	size_t numEdgesToFail = floor(perc * failed->getNumEdges());

	for (size_t i = 0; i < numEdgesToFail; i++) {
		edgeToFail = failed->selectRandomEdge(&emptyTabuList);
		failed->delEdge(edgeToFail);
	}

	return failed;
}

template <class T>
TabuAdjMatrix<T>* Benchmark<T>::edgeCommCost(
		TabuAdjMatrix<bool>* graph) {

	size_t numNodes = topology->getNumNodes();//==taskGraph->getNumNodes()
	TabuAdjMatrix<T>* costGraph = new TabuAdjMatrix<T>(numNodes, 0);

	grEdge tgEdge;
	grEdge cgEdge;
	for (tgEdge.orig = 0; tgEdge.orig < numNodes; tgEdge.orig++) {
		for (tgEdge.dest = 0; tgEdge.dest < numNodes; tgEdge.dest++) {
			if (taskGraph->edgeExists(tgEdge)) {
				//saves shortest path
				std::vector<size_t> shortPath = Dijkstra<bool>::dijkstra(
						graph, tgEdge.orig, tgEdge.dest,
						weightInf, false).shortPath;

				//updates the weight of the costGraph acording to
				//shortestPath found.
				//shortest path is a list of nodes.
				for (size_t i = 1; i < shortPath.size(); i++) {
					cgEdge.orig = shortPath[i - 1];
					cgEdge.dest = shortPath[i];
					cgEdge.value = costGraph->getEdgeValue(cgEdge);
					cgEdge.value += taskGraph->getEdgeValue(tgEdge);
					costGraph->delEdge(cgEdge);
					costGraph->addEdge(cgEdge);
				}
			}
		}
	}	

	return costGraph;
}

template <class T>
Benchmark<T>::Benchmark() {
	taskGraph = NULL;
	topology = NULL;
	weightInf = 0;
}

template<class T>
Benchmark<T>::~Benchmark() {
	if (taskGraph != NULL)
		delete taskGraph;
	if (topology != NULL)
		delete topology;

	taskGraph = NULL;
	topology = NULL;
	weightInf = 0;
}

template <class T>
void Benchmark<T>::setTaskGraph(const GraphRepresentation<T>* tg) {
	if (tg != NULL)
		this->taskGraph = tg->copy();
}

template <class T>
void Benchmark<T>::setTopology(const TabuAdjMatrix<bool>* graph) {
	if (graph != NULL)
		this->topology = graph->copy();
}

template <class T>
void Benchmark<T>::setWeightInf(const T inf) {
	this->weightInf = inf;
}

template <class T>
void Benchmark<T>::start() {
	if (taskGraph == NULL || topology == NULL)
		return;

	TabuAdjMatrix<bool>* failTopology;
	TabuAdjMatrix<T>* weightedTopology;
	Statistics<T> stats;
	for (double perc = 0.1; perc < 0.35; perc += 0.05) {
		for (unsigned int i = 0; i < 30; i++) {
			failTopology = failLinks(perc);
			weightedTopology = edgeCommCost(failTopology);
			
			//stats
			stats.computeGraphStats(weightedTopology);
			
			//TODO: write to file
			std::cout << "\n=========" << perc*100 <<
				"%=====" << i + 1 << "/30=====\n";
			std::cout << stats.getMinWeight() << "\n";
			std::cout << stats.getMaxWeight() << "\n";
			std::cout << stats.getMeanWeight() << "\n";
			std::cout << stats.getStdDev() << "\n";
			std::cout << stats.getFitness() << std::endl;

			delete failTopology;
			delete weightedTopology;
		}
	}
}
