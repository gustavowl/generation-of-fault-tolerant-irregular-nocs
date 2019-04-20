template <class ST, class DT>
void GraphConverter::convert(GraphRepresentation<ST>* source,
		GraphRepresentation<DT>* dest) {
	//Asserts that both representations have the same
	//number of nodes.
	unsigned int snodes = source->getNumNodes();
	unsigned int dnodes = dest->getNumNodes();
	if (snodes != dnodes)
		return;

	//Copies edges
	for (unsigned int i = 0; i < snodes; i++) {
		for (unsigned int j = 0; j < snodes; j++) {
			if (! source->edgeExists(i, j))
				continue;
			dest->addEdge(i, j, source->getEdgeValue(i, j));
		}
	}
}
