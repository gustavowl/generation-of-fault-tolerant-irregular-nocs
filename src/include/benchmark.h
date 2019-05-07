#ifndef __BENCHMARK_H_
#define __BENCHMARK_H_

#include "graph-representation.h"
#include "tabu-search.h"
#include "tabu-adj-matrix.h"
#include "dijkstra.h"
#include "rng.h"
#include "tabu-list.h"
#include <vector>
#include "statistics.h"

template <class T>
class Benchmark {
private:
	GraphRepresentation<T>* taskGraph;
	T weightInf;
	TabuAdjMatrix<bool>* topology;
	//returns a copy of graph after randomly deleting
	//floor(perc * |E|) links.
	//perc \in [0, 1]
	TabuAdjMatrix<bool>* failLinks(double perc);

public:
	Benchmark();
	~Benchmark();

	void setTaskGraph(const GraphRepresentation<T>* tg);
	void setTopology(const TabuAdjMatrix<bool>* graph);
	void setWeightInf(const T inf);

	void start();
};

#include "../benchmark.inl"

#endif
