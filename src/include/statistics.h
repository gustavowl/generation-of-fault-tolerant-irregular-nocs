#ifndef __STATISTICS_H__
#define __STATISTICS_H__

#include "graph-representation.h"
#include "tabu-adj-matrix.h"

template <class T>
class Statistics {
private:
	T minWeight;
	T maxWeight;
	double meanWeight;
	double stdDev;
	T fitness; //alias to weightSum
	bool disconnected;
	TabuAdjMatrix<T>* weightedGraph;

public:
	T getMinWeight() const;
	T getMaxWeight() const;
	double getMeanWeight() const;
	double getStdDev() const;
	T getFitness() const;
	bool isDisconnected() const;
	TabuAdjMatrix<T>* getWeightedGraph();

	Statistics();
	~Statistics();

	//calculates the communication of each graph's edge
	//according to tg by using TabuSearch's fitness
	//(QAP function) and returns the weighted graph.
	//For instance, if tg has an edge (0, 2) with weight 7,
	//and the shortest path from 0 to 2 is through the edges
	//(0, 1) -> (1, 2), then these edges will have minimum
	//weight of 7 in the resulting graph.
	//(0, 1).weight += 7
	//(1, 2).weight += 7
	void edgeCommCost(
			const GraphRepresentation<T>* tg,
			const TabuAdjMatrix<bool>* graph,
			T weightInf);

	void computeTopologyStats(
			const GraphRepresentation<T>* tg,
			const TabuAdjMatrix<bool>* topology,
			T weightInf);

};

#include "../statistics.inl"

#endif
