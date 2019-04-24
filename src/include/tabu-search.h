#ifndef __TABU_SEARCH__
#define __TABU_SEARCH__

#include "graph-representation.h"
#include "graph-converter.h"
#include "adjacency-matrix.h"
#include "dijkstra.h"
#include <vector>
#include <cstdlib>
#include <iostream> //TODO: DELETEME (DEBUG)

template <class T>
class TabuSearch {
private:

	//movement used for neighbourhood search: edge position
	//swap. Movements are also added to tabuList (actually,
	//the edgeDeltd, not the movement itself).
	//Neighbourhood search basically changes the position
	//of an edge: Deletes a random existing edge, and adds
	//another edge randomly.
	struct Movement{
		size_t* edgeDeltd; //edge deleted
		size_t* edgeAdded; //edge added
	};

	//removes/adds edges until |E| = epsilon.
	//called by generateInitSol.
	static void fitToEpsilon(AdjacencyMatrix<bool>* initSol,
			size_t epsilon);

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
	//tg: task graph
	//sol: solution
	static T fitness(const GraphRepresentation<T>* tg,
			const AdjacencyMatrix<bool>* sol);

	static bool areEdgesEqual(size_t* edge1, size_t* edge2);

	static bool isInTabuList(const std::vector<size_t*>* tabuList, Movement mov);

	static void addToTabuList(std::vector<size_t*>* tabuList, size_t* tabuIndex,
			Movement mov);

	//this enum is used for neighbourhood search.
	//Thus, used by delRandomEdge(), addRandomEdge(), and
	//getRandomNeighbour() methos.
	enum neighbourStatus { del2deg2, //deleted a edge with 2 degree 2 nodes
		del1deg2, //deleted a edge with 1 degree 2 node
		dflt, // (default) delete edge with degree >2 nodes or
			//add edge with degree <4 nodes
		add1deg4, //add edge with 1 degree 4 node
		add2deg4 //add edge with 2 degree 4 nodes
	};

	static neighbourStatus delRandomEdge(AdjacencyMatrix<bool>* neighbour,
			size_t* retEdge);

	static bool addRandomEdge(AdjacencyMatrix<bool>* neighbour,
			neighbourStatus status, size_t* deltdEdge,
			bool aspirationCrit=true);

	//returns a movement for a random neighbour according to
	//the neighbourhood step. A neighbourhood steps basically
	//changes the position of an edge: deletes a random existing
	//edge, and adds another edge randomly.
	//
	//currSol: current solution. A neighbour will be compute one step
	//from here.
	//epsilon: passed to start(). It limits the number of edges in the
	//possible solution. Thus, it is used to calculate how many edges
	//are not in the solution so an element of these complementary sets
	//can be randomly chosen.
	//tabuList: a vector of the edges in the tabuList. It will only be
	//used if aspirationCrit is set to false.
	//aspirationCrit: aspiration criteria. If it is set to false, then
	//it will return a movement not in the tabuList.
	static Movement getRandomNeighbour(const AdjacencyMatrix<bool>* currSol,
			size_t epsilon, const std::vector<size_t*>* tabuList,
			bool aspirationCrit=true);

	//makes movement specified by mov.
	//if undo is set to true, it tracebacks
	//(undoes a movement done).
	//This method is called for neighbourhood search
	static void makeMovement(AdjacencyMatrix<bool>* currSol, Movement mov,
			bool undo=false);

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
			size_t tabuListSize, size_t stopCriteria,
			size_t epsilon);
};

#include "../tabu-search.inl"

#endif
