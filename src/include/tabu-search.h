#ifndef __TABU_SEARCH__
#define __TABU_SEARCH__

#include "graph-representation.h"
#include "graph-converter.h"
#include "tabu-list.h"
#include "tabu-adj-matrix.h"
#include "dijkstra.h"
#include "neighbourhood-search.h"
#include <iostream> //TODO: DELETEME (DEBUG)

template <class T>
class TabuSearch {
private:
	size_t minDegree, maxDegree, stopCriteria, epsilon;
	T fitnessLimit;
	GraphRepresentation<T>* taskGraph;
	//solutions are computed as boolean graphs
	TabuList<bool>* tabuList;
	NeighbourhoodSearch neighSearch;
	size_t performedIterations;

	//removes/adds edges until |E| = epsilon.
	//called by generateInitSol.
	void fitToEpsilon(TabuAdjMatrix<bool>* initSol);

	//returns whether initSol is feasible or not.
	//A solution is feasible if the degree of all of its nodes
	//is in the range [minDegree, maxDegree].
	//called by generateInitSol, and addEdgeDel2Deg2().
	bool isFeasible(TabuAdjMatrix<bool>* sol);

	//swaps the edges until degree(node) is in [2, 4] range for
	//all nodes. The algorithm is divided in 2 main steps and works
	//as follows.
	//	while (not feasible)
	//		1 - remove edge
	//			1.1 - identify node with largest degree
	//			1.2 - identify its neighbour with largest degree
	//			1.3 - remove edge
	//		2 - add edge
	//			2.1 - identify node with smallest degree
	//			2.2 - create empty tabuList of target nodes
	//			2.3 - identify second node with smallest degree
	//				(not in tabuList)
	//			2.4 - add edge between these two nodes
	//called by generateInitSol.
	void makeFeasible(TabuAdjMatrix<bool>* initSol);

	//generates initial solution based on the task graph.
	//returns NULL if a valid solution cannot be generated
	//i.e. if epsilon is too restrictive or not restrictive
	//enough for \-/ node, degree(node) in [2, 4].
	TabuAdjMatrix<bool>* generateInitSol();

	size_t selectBestNeighbour(std::vector<T>* neighboursFit);


	size_t searchAspirationCriteria(std::vector<T>* neighboursFit,
			T bestFit);

	void removeTabuNeighbours(
			std::vector<NeighbourhoodSearch::Neighbour>* neighbours,
			std::vector<T>* neighboursFit);

	bool generateNeighbourhood(TabuAdjMatrix<bool>* currSol, 
			std::vector<NeighbourhoodSearch::Neighbour>* neighbours,
			std::vector<T>* neighboursFit, bool aspirationCrit);

	//TODO: DELETEME used for debugging:
	//anchor for gdb
	void assert(bool value);

public:
	TabuSearch();
	~TabuSearch();
	//Every node in the solution must have degree in the
	//[minDegree, maxDegree]
	void setDegreeLimits(size_t minDegree, size_t maxDegree);

	//copies the taskGraph
	void setTaskGraph(const GraphRepresentation<T>* tg);

	//defines the size of the tabu list. TODO: recommend a size
	void setTabuListSize(size_t tabuListSize);

	//sets the max number of iterations with no improvements.
	//If this number is reached, the Tabu Search stops.
	void setStopCriteria(size_t stopCriteria);

	//set the max value of the fitness function.
	//Normaly it is the largest value possible (infinity).
	void setFitnessLimit(T fitnessLimit);

	//since this is a multiobjective problem, epsilon restriction
	//technique is used. Epsilon fixes the number of edges in the
	//solutions and in the neighbours.
	void setEpsilon(size_t epsilon);

	size_t getPerformedIterations();

	//Computes QAP. The tg nodes are mapped to sol nodes.
	//QAP = sum_{e_{ij}} sol.hops(i, j) * tg.value(i, j)
	//for all edges e_{ij} in tg.
	//
	//This method returns a T value. If a path does
	//not exist from i to j in the solution, then
	//valueLimit is returned, indicating that sol is
	//unfiasible (and disconnected).
	//tg.value is an alias for the edge's weight.
	//
	//tg: task graph
	//sol: solution
	//valueLimit: max value for T
	T fitness(const TabuAdjMatrix<bool>* sol);

	//Tabu search receives a task graph and attempts to minimise
	//the QAP function. The QAP function is directly relationed
	//with latency. The Tabu Search should maximise fault tolerance
	//as well. Thus, it is a multi-objective problem. There are
	//two was to deal with such problem class: pareto dominance,
	//and epsilon restriction (TODO check translation).
	//This implementation uses epsilon restriction by fixing
	//the number of edges.
	//This code supposes that the task graphs are directed and
	//acycled.
	//
	//Problem restrictions: the generated graphs node's will
	//have degree in the range [minDegree, maxDegree].
	//
	//ATTENTION: set the Tabu Search parameters before starting it:
	//setDegreeLimits, setTaskGraph, setTabuListSize, setStopCriteria,
	//setFitnessLimit, and setEpsilon.
	//TODO: return set of solutions.
	TabuAdjMatrix<bool>* start();
};

#include "../tabu-search.inl"

#endif
