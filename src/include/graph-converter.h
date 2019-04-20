#ifndef __GRAPH_CONVERTER__
#define __GRAPH_CONVERTER__

#include "graph-representation.h"

class GraphConverter {
public:
	template <class ST, class DT> //Source Type, Dest Type
	static void convert(GraphRepresentation<ST>* source, GraphRepresentation<DT>* dest);

};

#include "../graph-converter.inl"

#endif
