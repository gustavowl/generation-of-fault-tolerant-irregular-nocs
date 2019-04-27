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
	typename GraphRepresentation<ST>::Edge sedge;
	typename GraphRepresentation<DT>::Edge dedge;
	for (size_t i = 0; i < snodes; i++) {
		sedge.orig = i;

		for (size_t j = 0; j < snodes; j++) {
			sedge.dest = j;

			if ( source->edgeExists(sedge) ) {
				dedge.orig = sedge.orig;
				dedge.dest = sedge.dest;
				dedge.value = source->getEdgeValue(sedge);

				dest->addEdge(dedge);
			}
		}
	}
}
