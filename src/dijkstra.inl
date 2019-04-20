template <class T>
vector<unsigned int>* Dijkstra::dijkstra (
		const GraphRepresentation<T>* graph,
		unsigned int orig, unsigned int dest,
		bool isWeighted) {
	if (isWeighted)
		return this->weighted(graph, orig, dest);
	return this->unweighted(graph, orig, dest);
}

template <class T>
vector<unsigned int>* Dijkstra::weighted(
		const GraphRepresentation<T>* graph,
		unsigned int orig, unsigned int dest) {
	return NULL;
}

template <class T>
vector<unsigned int>* Dijkstra::unweighted(
		const GraphRepresentation<T>* graph,
		unsigned int orig, unsigned int dest) {
	return NULL;
}
