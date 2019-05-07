#ifndef __BENCHMARK_H_
#define __BENCHMARK_H_

#include "graph-representation.h"
#include "tabu-search.h"
#include "tabu-adj-matrix.h"
#include "dijkstra.h"
#include "rng.h"
#include "tabu-list.h"
#include <vector>

template <class T>
class Benchmark {
public:
	//returns a copy of graph after randomly deleting
	//floor(perc * |E|) links.
	//perc \in [0, 1]
	static TabuAdjMatrix<bool>* failLinks(
			const TabuAdjMatrix<bool>* graph,
			double perc);

	//calculates the communication of each graph's edge
	//according to tg by using TabuSearch's fitness
	//(QAP function) and returns the weighted graph.
	//For instance, if tg has an edge (0, 2) with weight 7,
	//and the shortest path from 0 to 2 is through the edges
	//(0, 1) -> (1, 2), then these edges will have minimum
	//weight of 7 in the resulting graph.
	//(0, 1).weight += 7
	//(1, 2).weight += 7
	static TabuAdjMatrix<T>* edgeCommCost(
			const GraphRepresentation<T>* tg,
			const TabuAdjMatrix<bool>* graph);
};

#include "../benchmark.inl"

#endif
