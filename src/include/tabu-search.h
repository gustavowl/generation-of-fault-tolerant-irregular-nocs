#ifndef __TABU_SEARCH__
#define __TABU_SEARCH__

#include "graph-representation.h"
#include "graph-converter.h"
#include "adjacency-matrix.h"
#include "dijkstra.h"
#include <vector>
//#include <cstdlib>
#include <iostream> //TODO: DELETEME (DEBUG)
#include <time.h> //rng
#include <chrono> //rng
#include <random> //rng 
//rng macro
std::mt19937 rng(
		std::chrono::steady_clock::now().time_since_epoch().count());

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

	static bool areEdgesEqual(size_t* edge1, size_t* edge2);

	static bool isInTabuList(const std::vector<size_t*>* tabuList,
			Movement mov);

	//deleted edges are added to tabu list.
	//Thus, the algorithm searches the neighbourhood for solutions
	//WITHOUT that edge
	static void addToTabuList(std::vector<size_t*>* tabuList,
			size_t* tabuIndex, Movement mov);

	//this enum is used for neighbourhood search.
	enum NeighbourStatus { del2deg2, //deleted a edge with 2 degree 2 nodes
		del1deg2, //deleted a edge with 1 degree 2 node
		dflt, // (default) delete edge with degree >2 nodes or
		//		add edge with degree <4 nodes
		add1deg4, //add edge with 1 degree 4 node
		add2deg4 //add edge with 2 degree 4 nodes
	};

	//returns a randomly chosen edge.
	//if exists is set to false, returns a edge that is NOT
	//in the graph.
	//Remember to deallocate the returned pointer.
	static size_t* selectRandomEdge(AdjacencyMatrix<bool>* graph,
			bool existent=true);

	//returns a randomly chosen edge incident to incidentNode.
	//if exists is set to false, returns a edge that is NOT
	//in the graph AND incident to incidentNode.
	//Remember to deallocate the returned pointer.
	static size_t* selectRandomEdge(AdjacencyMatrix<bool>* graph,
			size_t incidentNode, bool existent=true);

	//randomly selects an edge to be deleted.
	//It is called by randomNeighbourhoodStep().
	//returns the chosen edge.
	static size_t* selectEdgeToDel(AdjacencyMatrix<bool>* neighbour);

	//TODO: guarantee tabuList feasibleness, etc.
	//if not tabulist feasible, return NULL
	static size_t* selectEdgeToAdd(AdjacencyMatrix<bool>* neighbour,
			size_t* edgeToDel, NeighbourStatus delStatus,
			std::vector<size_t>* tabuList, bool aspirationCrit);

	//There are three possible scenarios when deleting edges (add=false):
	//1 - Delete an edge incident to 2 nodes of degree 2;
	//2 - Delete an edge incident to 1 node of degree 2;
	//3 - Otherwise.
	//There are three possible scenarios when adding edges (add=true):
	//1 - Add an edge incident to 1 node of degree 4;
	//2 - Add an edge incident to 2 nodes of degree 4;
	//3 - otherwise
	//The return type is one of these six status, listed in the
	//NeighbourStatus enum.
	static size_t* NeighbourStatus predictActionStatus(
			AdjacencyMatrix<bool>* graph, size_t* edge,
			bool add);

	//returns a movement for a random neighbour according to
	//the neighbourhood step. A neighbourhood steps basically
	//changes the position of an edge: deletes a random existing
	//edge, and adds another edge randomly. This method is aided by
	//selectEdgeToDel(), and selectEdgeToAdd(). These two methods are
	//responsible for ensuring that the generated solution is feasible.
	//In other words, that every node has degree in the [2, 4] range.
	//The number of edges is fixed by the epsilon value passed to
	//start(). Thus, this method shall not interfere in the number
	//of edges. TODO: ASSERT.
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
	static Movement randomNeighbourhoodStep(
			const AdjacencyMatrix<bool>* currSol,
			const std::vector<size_t*>* tabuList,
			bool aspirationCrit=true);

	//Called by guaranteeFeasibleStep when scenarion 1 occurs.
	//Scenario 1 Solution: another random existing edge will be chosen and
	//	their incident nodes will be swaped. For instance, (1, 2),
	//	(3, 4) may be swapped to (1, 4), (3, 2).
	//Due to the implementation of selectEdgeToAdd(), mov contains the
	//two original edges that shall be randomly swapped.
	static void swapEdgesNodes(AdjacencyMatrix<bool>* neighbour,
			Movement mov, std::vector<size_t>* tabuList, bool aspirationCrit);
	
	//Called by guaranteeFeasibleStep when Scenario 4 happens.
	//Scenario 4 Solution: Swap a random edge of the node with another
	//	of degree < 4. For example, consider that edge (0, 5)
	//	will be added. However, node 0 has degree 4: (0, 1),
	//	(0, 2), (0, 3), and (0, 4). Choose one of these edges
	//	randomly to be deleted (0, [1, 2, 3, 4]). Choose another random node
	//	n with degree < 4 and add the edge ([1, 2, 3, 4], n).
	//	This process is hereby called "spin" since one node is fixed while
	//	the other changes, like the spinning of a clock pointer.
	//	Note that allowing the degree of n to be 4 would trigger
	//	scenario 5. As a consequence, scenario 5 could be triggered
	//	multiple times depending on the current solution. This
	//	would cause a far step in the neighbourhood search space,
	//	(a leap, not a step) which is not desired.
	static void spinEdge(AdjacencyMatrix<bool>* neighbour,
			size_t* edge, std::vector<size_t>* tabuList, bool aspirationCrit);

	//Called by guaranteeFeasibleStep when Scenario 5 happens.
	//Scenario 5 Solution: it basically activates scenario 4 twice.
	//	For example, attempt to add the edge (0, 1), but
	//	degree(0) = degree(1) = 4. Then spin edges on nodes 0 and 1:
	//		delete random edge (0, x);
	//		delete random edge (1, y);
	//		add edge (0, random node n1) where degree(n1) < 4;
	//		add edge (1, random node n2) where degree(n2) < 4.
	//	Depending on the current solution, it may be necessary
	//	to rechoose the random edges (0, x) and (1, y).
	//	For instance, the algorithm may attempt to add edge
	//	(0, 1), (1, 0). This would reduce the number of edges
	//	in the graph, which is not desired in this tabu search
	//	since it would cause the fault tolerance to reduce.
	static void spinEdges(AdjacencyMatrix<bool>* neighbour,
			size_t* edge, std::vector<size_t>* tabuList, bool aspirationCrit);


	//it changes the neighbour graph to a feasible solution if needed.
	//This function is responsible for mantaining the solutions feasible.
	//There are 5 scenarios possible.
	//Their issues and detailed implementation can be
	//found in the tabu-search.inl file.
	//returns if the movement in the argument mov should be executed or not.
	//1 - The deleted edge was incident to two nodes of degree 2;
	//2 - The deleted edge was incident to one node of degree 2;
	//	Solution: An edge will be added incident to the degree 2
	//	node and a random one. For example:
	//	delete edge (1, 2) - nodes' degrees: 2 and 4, respectively,
	//	add edge (1, random( {nodes} - 2 ))
	//	Note: scenario 4 may be raisen.
	//3 - Default. The deleted edge is incident to nodes with
	//	degrees > 2; and the added edge is incident to nodes
	//	with degrees < 4.
	//4 - The added edge is incident to one node of degree 4.
	//5 - The added edge is incident to two nodes of degree 4.
	//
	//If aspirationCrit is set to false, the tabuList is considered
	//when adding edges (tabuList contains deleted edges, refer to
	//isInTabuList() and addToTabuList()).
	//
	//If false is returnedd, it does not necessarily mean that the step will
	//generate an unfeasible solution. Maybe the movement was already made,
	//e.g. Scenario 1.
	static bool guaranteeFeasibleStep(AdjacencyMatrix<bool>* neighbour,
			Movement mov, std::vector<size_t>* tabuList, bool aspirationCrit);

	//makes movement specified by mov.
	static void makeMovement(AdjacencyMatrix<bool>* graph, Movement mov);

	//This method is called for neighbourhood search.
	//It guarantees that the resulting solution after the movement will
	//be feasible.
	static void generateNeighbour(AdjacencyMatrix<bool>* neighbour, Movement mov,
			std::vector<size_t>* tabuList, bool aspirationCrit);

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
