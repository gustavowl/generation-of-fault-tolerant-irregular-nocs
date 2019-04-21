#ifndef __TABU_SEARCH__
#define __TABU_SEARCH__

#include "graph-representation.h"
#include "graph-converter.h"
#include "adjacency-matrix.h"
#include "dijkstra.h"

template <class T>
class TabuSearch {
private:

	//removes/adds edges until |E| = epsilon.
	//called by generateInitSol.
	static void fitToEpsilon(AdjacencyMatrix<bool>* initSol,
			unsigned int epsilon);

	//swaps the edges until degree(node) is in [2, 4] for all nodes.
	//called by generateInitSol.
	static void makeFeasible(AdjacencyMatrix<bool>* initSol);

	//generates initial solution based on the task graph.
	//returns NULL if a valid solution cannot be generated
	//i.e. if epsilon is too restrictive or not restrictive
	//enough for \-/ node, degree(node) in [2, 4].
	static AdjacencyMatrix<bool>* generateInitSol(
			const GraphRepresentation<T>* tg, unsigned int epsilon);

	//Computes QAP
	static T fitness(const GraphRepresentation<T>* tg);

public:
	//Tabu search receives a task graph and attempts to minimise
	//the QAP function. The QAP function is directly relationed
	//with latency. The Tabu Search should maximise fault tolerance
	//as well. Thus, it is a multi-objective problem. There are
	//two was to deal with such problem class: pareto principle,
	//and epsilon restriction (TODO check translation).
	//This implementation uses epsilon restriction by fixing
	//the number of edges.
	//This code supposes that the task graphs are directed and
	//acycled.
	//
	//Problem restrictions: the generated graphs node's will
	//have degree in the range [2, 4].
	//
	//tg: Task Graph
	//tabuListSize: self-explanatory (recommended: = epsilon?)
	//stopCriteria: number of iterations with no improvement
	//epsilon: used for epsilon restriction (TODO: check translation).
	//	it limits the number of edges.
	//returns: best solution found as an undirected graph (it may
	//	contain cycles). Returns NULL is no solution is feasible
	//	(there exists a node for which degree(node) not in [2, 4].
	//	TODO: return set of solutions.
	static AdjacencyMatrix<T>* start(const GraphRepresentation<T>* tg,
			unsigned int tabuListSize, unsigned int stopCriteria,
			unsigned int epsilon);
};

#include "../tabu-search.inl"

#endif
