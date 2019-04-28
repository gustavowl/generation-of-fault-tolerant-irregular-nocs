#ifndef __TABU_SEARCH__
#define __TABU_SEARCH__

#include "graph-representation.h"
#include "graph-converter.h"
#include "tabu-list.h"
#include "tabu-adj-matrix.h"
#include "dijkstra.h"
#include <iostream> //TODO: DELETEME (DEBUG)

template <class T>
class TabuSearch {
private:
	size_t minDegree, maxDegree, stopCriteria, epsilon;
	T fitnessLimit;
	GraphRepresentation<T> taskGraph;
	TabuList<T> tabuList;

	//movement used for neighbourhood search: edge position
	//swap. Movements are also added to tabuList (actually,
	//the edgeDeltd, not the movement itself).
	//Neighbourhood search basically changes the position
	//of an edge: Deletes a random existing edge, and adds
	//another edge randomly.
	struct Movement{
		size_t* edgeToDel; //edge to delete
		size_t* edgeToAdd;
	};

	//removes/adds edges until |E| = epsilon.
	//called by generateInitSol.
	static void fitToEpsilon(AdjacencyMatrix<bool>* initSol);

	//returns whether initSol is feasible or not.
	//A solution is feasible if the degree of all of its nodes
	//is in the range [minDegree, maxDegree].
	//called by generateInitSol, and addEdgeDel2Deg2().
	static bool isFeasible(AdjacencyMatrix<bool>* sol);

	//swaps the edges until degree(node) is in [2, 4] range for
	//all nodes. The algorithm is divided in 3 main steps and works
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
	//			2.4 - add edge
	//		3 - check if graph is disconnect
	//			3.1 - Dijkstra
	//			3.2 - if disconnected
	//				3.2.1 - remove edge
	//				3.2.2 - add target node to a TabuList
	//				3.2.3 - go back to step 2.3
	//called by generateInitSol.
	static void makeFeasible(AdjacencyMatrix<bool>* initSol);

	//generates initial solution based on the task graph.
	//returns NULL if a valid solution cannot be generated
	//i.e. if epsilon is too restrictive or not restrictive
	//enough for \-/ node, degree(node) in [2, 4].
	static AdjacencyMatrix<bool>* generateInitSol();

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
	static T fitness(const AdjacencyMatrix<bool>* sol);

	//makes movement specified by mov.
	static void makeMovement(AdjacencyMatrix<bool>* graph, Movement mov);

	//called after searching neighbourhood
	static void deallocateMovement(Movement* mov);

	//called after tabu search finishes
	static void deallocateTabuList(std::vector<size_t*>* tabuList);

public:
	TabuSearch();
	~TabuSearch();
	//Every node in the solution must have degree in the
	//[minDegree, maxDegree]
	void setDegreeLimits(size_t minDegree, size_t maxDegree);

	//copies the taskGraph
	void setTaskGraph(const GraphRepresentation<T>* taskGraph);

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
	TabuAdjMatrix<T>* start();
};

#include "../tabu-search.inl"

#endif
