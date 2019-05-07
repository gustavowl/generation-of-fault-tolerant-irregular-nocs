template <class T>
TabuAdjMatrix<bool>* Benchmark<T>::failLinks(
		const TabuAdjMatrix<bool>* graph,
		double perc) {

	if (perc < 0 || perc > 1)
		return NULL;

	TabuAdjMatrix<bool>* failed = graph->copy();
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
		const GraphRepresentation<T>* tg,
		const TabuAdjMatrix<bool>* graph, T weightInf) {

	size_t numNodes = graph->getNumNodes(); // == tg->getNumNodes()
	TabuAdjMatrix<T>* costGraph = new TabuAdjMatrix<T>(numNodes, 0);

	grEdge tgEdge;
	grEdge cgEdge;
	for (tgEdge.orig = 0; tgEdge.orig < numNodes; tgEdge.orig++) {
		for (tgEdge.dest = 0; tgEdge.dest < numNodes; tgEdge.dest++) {
			if (tg->edgeExists(tgEdge)) {
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
					cgEdge.value += tg->getEdgeValue(tgEdge);
					costGraph->delEdge(cgEdge);
					costGraph->addEdge(cgEdge);
				}
			}
		}
	}	

	return costGraph;
}
