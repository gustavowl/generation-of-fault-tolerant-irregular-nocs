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
	Statistics<T> stats;
	for (double perc = 0.1; perc < 0.35; perc += 0.05) {
		for (unsigned int i = 0; i < 30; i++) {

			failTopology = failLinks(perc);
			
			//stats
			stats.computeTopologyStats(taskGraph,
					failTopology, weightInf);
			
			//TODO: write to file
			std::cout << "\n=========" << perc*100 <<
				"%=====" << i + 1 << "/30=====\n";
			failTopology->print();
			std::cout << "min: " << stats.getMinWeight() << "\n";
			std::cout << "max: " << stats.getMaxWeight() << "\n";
			std::cout << "mean: " << stats.getMeanWeight() << "\n";
			std::cout << "stdDev: " << stats.getStdDev() << "\n";
			std::cout << "fitness: " << stats.getFitness() << "\n";
			std::cout << "discon: "<< stats.isDisconnected() << std::endl;

			delete failTopology;
		}
	}
}
