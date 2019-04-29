#ifndef __NEIGHBOURHOOD_SEARCH__
#define __NEIGHBOURHOOD_SEARCH__

#include "tabu-adj-matrix.h"
#include "tabu-list.h"
#define boolEdge typename GraphRepresentation<bool>::Edge

class NeighbourhoodSearch {
public:
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

	//This method is called for neighbourhood search.
	//A neighbour is created considering the current solution and returned.
	//The neighbour is feasible (within the degree range).
	//It guarantees that the resulting solution after the movement will
	//be feasible.
	static Neighbour generateNeighbour(TabuAdjMatrix<bool>* currSol,
			std::vector<size_t>* tabuList, bool aspirationCrit);

	//this enum is used for neighbourhood search.
	enum NeighbourStatus { del2mindeg, //deleted a edge with 2 degree 2 nodes
		del1mindeg, //deleted a edge with 1 degree 2 node
		dflt, // (default) delete edge with degree >2 nodes or
		//		add edge with degree <4 nodes
		add1maxdeg, //add edge with 1 degree 4 node
		add2maxdeg //add edge with 2 degree 4 nodes
	};

	struct Neighbour {
		TabuAdjMatrix<bool>* sol; //solution
		boolEdge deltdEdge; //deleted edge
		bool isTabu; //was tabu edge added
	}

	static void deallocateNeighbour(Neighbour* neigh);

private:
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
	static NeighbourStatus predictDelActionStatus(
			Neighbour* neigh);

	static NeighbourStatus predictAddActionStatus(
			Neighbour* neigh, boolEdge edgeToAdd);

	static bool swap(Neighbour* neigh, TabuList<bool>* tabuList,
			bool aspirationCrit);

	static bool spinMinDegree(Neighbour* neigh, TabuList<bool>* tabuList,
			bool aspirationCrit);

	static bool spinMaxDegree(Neighbour* neigh, boolEdge edgeToAdd,
			TabuList<bool>* tabuList, bool aspirationCrit);

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
	static bool neighbourhoodStep(
			Neighbour* neigh, TabuList<bool>* tabuList,
			bool aspirationCrit=true);
	
}

#endif
