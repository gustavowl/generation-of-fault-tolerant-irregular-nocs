template <class ST, class DT>
void GraphConverter::convert(const GraphRepresentation<ST>* source,
		GraphRepresentation<DT>* dest) {
	//Asserts that both representations have the same
	//number of nodes.
	size_t snodes = source->getNumNodes();
	size_t dnodes = dest->getNumNodes();
	if (snodes != dnodes)
		return;

	//Copies edges
	for (size_t i = 0; i < snodes; i++) {
		for (size_t j = 0; j < snodes; j++) {
			if (! source->edgeExists(i, j))
				continue;
			dest->addEdge(i, j, source->getEdgeValue(i, j));
		}
	}
}
