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
#include "file-manager.h"
#include <sys/stat.h>
#include <string>
#include <cstring>

template <class T>
class Benchmark {
private:
	GraphRepresentation<T>* taskGraph;
	T weightInf;
	TabuAdjMatrix<bool>* topology;
	std::string graphName;
	std::string tabuStatsFilename;
	//tabu search args
	std::string inputGraph;
	std::string outputFilename;
	size_t epsilon;
	size_t tabuListSize;
	size_t stopCriterion;
	size_t performedIter;

	//returns a copy of graph after randomly deleting
	//floor(perc * |E|) links.
	//perc \in [0, 1]
	TabuAdjMatrix<bool>* failLinks(double perc);

	std::string tabuSearchArgsToStr();
	void saveGraph(Statistics<T>* stats, std::string outputfile);
	void saveGraphStats(Statistics<T>* stats, std::string outputfile,
			std::string suffix);

public:
	Benchmark();
	~Benchmark();

	void setTaskGraph(const GraphRepresentation<T>* tg);
	void setTopology(const TabuAdjMatrix<bool>* graph);
	void setWeightInf(const T inf);
	void setTabuStatsFilename(std::string tsf);
	void setUsedTabuArgs(std::string inputGraph, std::string outputfn,
			size_t eps, size_t tlsize, size_t stopCrit, size_t numite);

	void start();
};

#include "../benchmark.inl"

#endif
