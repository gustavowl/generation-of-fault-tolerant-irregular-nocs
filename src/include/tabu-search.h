#ifndef __TABU_SEARCH__
#define __TABU_SEARCH__

#include "graph-representation.h"
#include "graph-converter.h"
#include "adjacency-matrix.h"
#include "dijkstra.h"
#include <vector>

template <class T>
class TabuSearch {
private:

	//removes/adds edges until |E| = epsilon.
	//called by generateInitSol.
	static void fitToEpsilon(AdjacencyMatrix<bool>* initSol,
			unsigned int epsilon);

	//returns whether initSol is feasible or not.
	//A solution is feasible if the degree of all of its nodes
	//is in the range [2, 4].
	//called by generateInitSol.
	static bool isFeasible(AdjacencyMatrix<bool>* initSol);

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
	//			2.2 - create empty tabuList
	//			2.3 - identify node neighbour with smallest degree
	//				node in tabu list
	//			2.4 - add edge
	//		3 - check if graph is disconnect
	//			3.1 - Dijkstra
	//			3.2 - if disconnected
	//				3.2.1 - remove edge
	//				3.2.2 - add edge to a TabuList
	//				3.2.3 - go back to step 2.3
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
