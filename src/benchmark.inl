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
void Benchmark<T>::setGraphName(std::string gn) {
	this->graphName = gn;
}

template <class T>
void Benchmark<T>::setTabuStatsFilename(std::string tsf) {
	this->tabuStatsFilename = tsf;
}

template <class T>
std::string Benchmark<T>::tabuSearchArgsToStr() {
	std::string args = "";
	args += inputGraph + ',';
	args += outputFilename + ',';
	args += std::to_string(epsilon) + ',';
	args += std::to_string(tabuListSize) + ',';
	args += std::to_string(stopCriterion) + ',';
	args += std::to_string(numIterations);
	return args;
}

template <class T>
void Benchmark<T>::saveGraph(Statistics<T>* stats,
		std::string outputfile) {

	FileManager::writeFile(outputfile,
			stats->getWeightedGraph());
}

template <class T>
void Benchmark<T>::saveGraphStats(Statistics<T>* stats,
		std::string outputfile, std::string suffix) {

	std::string out = suffix + ',';
	//format:
	//fitness, is-disconnected, min-weight, max-weight,
	//mean-weight, weight-std-dev
	out += std::to_string(stats->getFitness()) + ',';
	out += std::to_string(stats->isDisconnected()) + ',';
	out += std::to_string(stats->getMinWeight()) + ',';
	out += std::to_string(stats->getMaxWeight()) + ',';
	out += std::to_string(stats->getMeanWeight()) + ',';
	out += std::to_string(stats->getStdDev());

	FileManager::writeLine(outputfile, out);
}

template <class T>
void Benchmark<T>::start() {
	if (taskGraph == NULL || topology == NULL)
		return;

	std::string gext = ".adjl"; //graph extension
	std::string sext = ".csv"; //stats extension

	Statistics<T> stats;

	char gn[graphName.length() + 1];
	strcpy(gn, graphName.c_str());
	mkdir(gn, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

	stats.computeTopologyStats(taskGraph, topology, weightInf);
	saveGraph(&stats, graphName + '/' + graphName + gext);
	std::string suffix = tabuSearchArgsToStr();
	saveGraphStats(&stats, tabuStatsFilename, suffix);

	//fault-injection dir path
	std::string finj = graphName + '/' + "fault-injection";
	char arrFinj[finj.length() + 1];
	strcpy(arrFinj, finj.c_str());
	mkdir(arrFinj, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

	TabuAdjMatrix<bool>* failTopology;
	for (double perc = 0.1; perc < 0.35; perc += 0.05) {
		for (unsigned int i = 0; i < 30; i++) {

			failTopology = failLinks(perc);
			
			//stats
			stats.computeTopologyStats(taskGraph, failTopology,
					weightInf);
			saveGraph(&stats, finj + '/' + graphName + outputfile + gext);
			saveGraphStats(&stats, graphName + '/' + "fault-stats" +
					sext);

			delete failTopology;
		}
	}
}
