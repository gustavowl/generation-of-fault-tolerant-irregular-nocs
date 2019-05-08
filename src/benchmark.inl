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
void Benchmark<T>::setTabuStatsFilename(std::string tsf) {
	this->tabuStatsFilename = tsf;
}

template <class T>
void Benchmark<T>::setUsedTabuArgs(std::string inputGraph,
		std::string outputfn, size_t eps, size_t tlsize,
		size_t stopCrit, size_t numite) {
	this->inputGraph = inputGraph;
	this->outputFilename = outputfn;
	this->epsilon = eps;
	this->tabuListSize = tlsize;
	this->stopCriterion = stopCrit;
	this->performedIter = numite;
	this->graphName = outputfn.substr(0, outputfn.find('.'));
}

template <class T>
std::string Benchmark<T>::tabuSearchArgsToStr() {
	std::string igpath = "..";
	size_t size = std::count(resDir.begin(), resDir.end(), '/');
	for (size_t i = 0; i < size; i++)
		igpath += "/..";

	std::string args = "";
	args += igpath + '/' + inputGraph + ',';
	args += graphName + '/' + outputFilename + ',';
	args += std::to_string(epsilon) + ',';
	args += std::to_string(tabuListSize) + ',';
	args += std::to_string(stopCriterion) + ',';
	args += std::to_string(performedIter);
	return args;
}

template <class T>
void Benchmark<T>::saveGraph(Statistics<T>* stats,
		std::string outputfile) {

	FileManager::writeFile(resDir + '/' + outputfile,
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

	FileManager::writeLine(resDir + '/' + outputfile, out);
}

template <class T>
void Benchmark<T>::start() {
	if (taskGraph == NULL || topology == NULL)
		return;

	char rd[resDir.length() + 1];
	strcpy(rd, resDir.c_str());
	mkdir(rd, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	std::string gext = ".adjl"; //graph extension
	std::string sext = ".csv"; //stats extension

	Statistics<T> stats;

	std::string gnPath = resDir + '/' + graphName;
	char rdgn[gnPath.length() + 1];
	strcpy(rdgn, gnPath.c_str());
	mkdir(rdgn, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

	stats.computeTopologyStats(taskGraph, topology, weightInf);
	saveGraph(&stats, graphName + '/' + graphName + gext);
	std::string suffix = tabuSearchArgsToStr();
	saveGraphStats(&stats, tabuStatsFilename, suffix);

	//fault-injection dir path
	std::string finj = gnPath + '/' + "fault-injection";
	char arrFinj[finj.length() + 1];
	strcpy(arrFinj, finj.c_str());
	mkdir(arrFinj, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	finj = "fault-injection";

	TabuAdjMatrix<bool>* failTopology;
	std::string failName = "";
	for (double perc = 0.1; perc < 0.35; perc += 0.05) {
		for (unsigned int i = 0; i < 30; i++) {
			failName = graphName + "-perc" + std::to_string(
					int(perc*100)) + "-exec";
			if (i < 10)
				failName += '0';
			failName += std::to_string(i);

			failTopology = failLinks(perc);
			
			//stats
			stats.computeTopologyStats(taskGraph, failTopology,
					weightInf);
			saveGraph(&stats, graphName + '/' + finj + '/' +
					failName + gext);

			suffix = finj + '/' + failName + gext + ',';
			suffix += std::to_string(int(perc*100));
			saveGraphStats(&stats, graphName + '/' + "fault-stats" +
					sext, suffix);

			delete failTopology;
		}
	}
}
