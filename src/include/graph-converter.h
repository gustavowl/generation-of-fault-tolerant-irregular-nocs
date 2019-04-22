#ifndef __GRAPH_CONVERTER__
#define __GRAPH_CONVERTER__

#include "graph-representation.h"

class GraphConverter {
public:
	/* Converts a graph representation to another.
	 * It copies the data from source to dest.
	 * This method uses operator= to create the
	 * deep copy. Thus, override this operator
	 * in the DT (dest type) class if necessary.
	 *
	 * This method asserts that both representations
	 * have the same number of nodes, then copies
	 * the edges. It supposes that dest has no edges.
	 * Thus, it does not delete dest edges.
	 * If dest has edges, they may be overwritten
	 * throughout the process.
	 */
	template <class ST, class DT> //Source Type, Dest Type
	static void convert(const GraphRepresentation<ST>* source, GraphRepresentation<DT>* dest);

};

#include "../graph-converter.inl"

#endif
