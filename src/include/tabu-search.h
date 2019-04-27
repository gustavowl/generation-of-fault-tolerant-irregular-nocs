#ifndef __TABU_SEARCH__
#define __TABU_SEARCH__

#include "graph-representation.h"
#include "graph-converter.h"
#include "adjacency-matrix.h"
#include "dijkstra.h"
#include <vector>
//#include <cstdlib>
#include <iostream> //TODO: DELETEME (DEBUG)

template <class T>
class TabuSearch {
private:
	static const size_t MIN_DEGREE = 2;
	static const size_t MAX_DEGREE = 4;

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
	static void fitToEpsilon(AdjacencyMatrix<bool>* initSol,
			size_t epsilon);

	//returns whether initSol is feasible or not.
	//A solution is feasible if the degree of all of its nodes
	//is in the range [2, 4].
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
	static AdjacencyMatrix<bool>* generateInitSol(
			const GraphRepresentation<T>* tg, size_t epsilon);

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
	static T fitness(const GraphRepresentation<T>* tg,
			const AdjacencyMatrix<bool>* sol, T valueLimit);

	//makes movement specified by mov.
	static void makeMovement(AdjacencyMatrix<bool>* graph, Movement mov);

	//called after searching neighbourhood
	static void deallocateMovement(Movement* mov);

	//called after tabu search finishes
	static void deallocateTabuList(std::vector<size_t*>* tabuList);

public:
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
			T valueLimit, size_t tabuListSize, size_t stopCriteria,
			size_t epsilon);
};

#include "../tabu-search.inl"

#endif
