#include "include/neighbourhood-search.h"

typename NeighbourhoodSearch::NeighbourStatus
NeighbourhoodSearch::predictDelActionStatus(
		Neighbour* neigh) {

	NeighbourStatus status = dflt;
	boolEdge edge = neigh->deltdEdges[0];

	//delete action
	if (neigh->sol->getNodeDegree(edge.orig) == minDegree && 
			neigh->sol->getNodeDegree(edge.dest) == minDegree) {
		status = del2mindeg;
	}
	else if (neigh->sol->getNodeDegree(edge.orig) == minDegree ||
			neigh->sol->getNodeDegree(edge.dest) == minDegree) {
		status = del1mindeg;
	}

	return status;
}

typename NeighbourhoodSearch::NeighbourStatus
NeighbourhoodSearch::predictAddActionStatus(
		Neighbour* neigh, boolEdge edgeToAdd) {

	NeighbourStatus status = dflt;

	//simulates the degrees after edgeToDel is deleted
	size_t addOrigDeg = neigh->sol->getNodeDegree(edgeToAdd.orig) + 1;
	size_t addDestDeg = neigh->sol->getNodeDegree(edgeToAdd.dest) + 1;
	if (edgeToAdd.orig == neigh->deltdEdges[0].orig ||
			edgeToAdd.orig == neigh->deltdEdges[0].dest) {
		addOrigDeg--;
	}
	if (edgeToAdd.dest == neigh->deltdEdges[0].orig ||
			edgeToAdd.dest == neigh->deltdEdges[0].dest) {
		addDestDeg--;
	}

	//add action
	if (addOrigDeg > maxDegree && addDestDeg > maxDegree) {
		status = add2maxdeg;
	}
	
	else if (addOrigDeg > maxDegree || addDestDeg > maxDegree) {
		status = add1maxdeg;
	}

	return status;
}

bool NeighbourhoodSearch::swap(Neighbour* neigh) {

	TabuList<bool> tabuSlctdEdges;
	if (!aspirationCrit) { //populate tabuList
		for (size_t i = 0; i < tabuList->size(); i++)
			tabuSlctdEdges.add(tabuList->at(i));
	}

	//select edge to swap
	boolEdge edgeToAdd = neigh->sol->selectRandomEdge(&tabuSlctdEdges);
	boolEdge edgeToSwap1, edgeToSwap2;


	while (true) {
		if (neigh->sol->isEdgeInvalid(edgeToAdd))
			return false;

		edgeToSwap1 = neigh->deltdEdges[0];
		edgeToSwap2 = edgeToAdd;

		//there are two possible swaps for each selected edge
		TabuList<bool> tabuSwaps;
		for (size_t i = 0; i < 2; i++) {
			neigh->sol->swapEdgesNodes(&edgeToSwap1, &edgeToSwap2,
					&tabuSwaps);
			//edges swapped may not be valid.
			if (neigh->sol->isEdgeInvalid(edgeToSwap1) ||
					neigh->sol->isEdgeInvalid(edgeToSwap2)) {
				break;
			}
			//If solution is tabu, traceback and
			//add swaped edges to a tabuList and re-run
			if (!aspirationCrit && tabuList->isTabu(neigh->sol)) {
				neigh->sol->delEdge(edgeToSwap1);
				neigh->sol->delEdge(edgeToSwap2);
				neigh->sol->addEdge(neigh->deltdEdges[0]);
				neigh->sol->addEdge(edgeToAdd);

				tabuSwaps.add(edgeToSwap1);
				tabuSwaps.add(edgeToSwap2);

				if (i + 1 < 2) {
					edgeToSwap1 = neigh->deltdEdges[0];
					edgeToSwap2 = edgeToAdd;
				}
				else { //i == 2, stop
					//used to verify that swap is valid
					edgeToSwap1 = neigh->sol->generateInvalidEdge();
					edgeToSwap2 = neigh->sol->generateInvalidEdge();
				}
				continue;
			}
			//if tabu is not considered, or valid solution found, stop
			break;
		}

		//No swap for edgeToAdd may be valid.
		//Add edgeToAdd to a tabu list and select another edge.	
		if (neigh->sol->isEdgeInvalid(edgeToSwap1) ||
				neigh->sol->isEdgeInvalid(edgeToSwap2)) {

			tabuSlctdEdges.add(edgeToAdd);
			edgeToAdd = neigh->sol->selectRandomEdge(&tabuSlctdEdges);
			continue;
		}
		//adds the second edge deleted (before swap) to list
		neigh->deltdEdges.push_back(edgeToAdd);
		//check if edges swapped are tabu
		neigh->isTabu = tabuList->isTabu(neigh->sol);
		return true;
	}
	//POINT OF kNOw RETURN
}

bool NeighbourhoodSearch::spinMinDegree(Neighbour* neigh) {
	//selects node to be fixed (remain unchanged in the spinning process)
	boolEdge edgeToDel = neigh->deltdEdges[0];
	size_t fixedNode = (neigh->sol->getNodeDegree(edgeToDel.orig) ==
			minDegree) ? edgeToDel.orig : edgeToDel.dest;

	boolEdge spinned;
	TabuList<bool> tabuSpins;

	if (aspirationCrit) {
		spinned = neigh->sol->spinEdge(edgeToDel, fixedNode, maxDegree,
				&tabuSpins);
	}
	else {
		for (size_t i = 0; i < tabuList->size(); i++)
			tabuSpins.add(tabuList->at(i));
		for (size_t i = 0; i < neigh->sol->getNodeDegree(fixedNode);
				i++) {
			spinned = neigh->sol->spinEdge(edgeToDel, fixedNode,
					maxDegree, &tabuSpins);

			//unable to select an edge not in tabuList
			if (neigh->sol->isEdgeInvalid(spinned))
				break;
			//checks if solution is tabu
			if (tabuList->isTabu(neigh->sol)) {
				//traceback
				neigh->sol->delEdge(spinned);
				neigh->sol->addEdge(edgeToDel);
				//spinned edge cannot be chosen again
				tabuSpins.add(spinned);
				//used to verify if solution is valid
				spinned = neigh->sol->generateInvalidEdge();
				//rerun
				continue;
			}
			//valid solution
			break;
		}
	}

	neigh->isTabu = tabuList->isTabu(neigh->sol);

	return !neigh->sol->isEdgeInvalid(spinned);
}

bool NeighbourhoodSearch::spinMaxDegree(Neighbour* neigh,
		boolEdge edgeToAdd) {

	neigh->sol->delEdge(neigh->deltdEdges[0]);
	//select node with max degree.
	//It will have one edge spinned randomly.
	//The centre of the spin is the node adjacent to
	//the one with max degree./
	size_t maxDegNode = (neigh->sol->getNodeDegree(edgeToAdd.orig) ==
			maxDegree) ? edgeToAdd.orig : edgeToAdd.dest;
	//select edge incident to spinCentre randomly,
	//with the exception of the edgeToDel.
	TabuList<bool> tabuSpins;
	boolEdge edgeToSpin, spinned;

	TabuList<bool> tabuEdgesToAdd;
	tabuEdgesToAdd.add(neigh->deltdEdges[0]);

	std::cout << maxDegNode << std::endl;
	while(tabuSpins.size() < maxDegree) {
		edgeToSpin = neigh->sol->selectRandomEdge(maxDegNode, &tabuSpins,
				true);

		size_t spinCentre = (edgeToSpin.orig == maxDegNode) ?
			edgeToSpin.dest : edgeToSpin.orig;

		spinned = neigh->sol->spinEdge(edgeToSpin, spinCentre,
					maxDegree, &tabuEdgesToAdd);

		if (neigh->sol->isEdgeInvalid(spinned)) {
			tabuSpins.add(edgeToSpin);
			continue;
		}

		neigh->sol->addEdge(edgeToAdd);
		neigh->isTabu = tabuList->isTabu(neigh->sol);
		if (!aspirationCrit && neigh->isTabu) {
			//if movement is tabu, undo
			neigh->sol->delEdge(spinned);
			neigh->sol->addEdge(edgeToSpin);
			neigh->sol->delEdge(edgeToAdd);
			tabuSpins.add(edgeToSpin);
			continue;
		}

		std::cout << "edge to spin: " << 
			edgeToSpin.orig << "-" <<
			edgeToSpin.dest << std::endl;
		std::cout << "spinned edge: " << 
			spinned.orig << "-" <<
			spinned.dest << std::endl;
		neigh->deltdEdges.push_back(edgeToSpin);
		neigh->sol->delEdge(edgeToAdd);
		neigh->sol->addEdge(neigh->deltdEdges[0]);
		return true;
	}

	neigh->sol->addEdge(neigh->deltdEdges[0]);
	return false;
}

bool NeighbourhoodSearch::doubleSpinMaxDegree( Neighbour* neigh,
		boolEdge edgeToAdd) {

	neigh->sol->delEdge(neigh->deltdEdges[0]);

	TabuList<bool> tabuSpins;
	if (!aspirationCrit)
		for (size_t i = 0; i < tabuList->size(); i++)
			tabuSpins.add(tabuList->at(i));
	tabuSpins.add(neigh->deltdEdges[0]);
	boolEdge* edgesAdded;

	while (true) {
		edgesAdded = neigh->sol->doubleSpinEdge( neigh->deltdEdges[0],
				edgeToAdd, maxDegree, &tabuSpins);

		if (edgesAdded == NULL) {
			neigh->sol->addEdge(neigh->deltdEdges[0]);
			return false;
		}
		neigh->sol->addEdge(edgeToAdd);
		neigh->isTabu = tabuList->isTabu(neigh->sol);

		if (!aspirationCrit && neigh->isTabu) {
			//traceback and re-run
			neigh->sol->addEdge(edgesAdded[0]);
			neigh->sol->addEdge(edgesAdded[1]);
			neigh->sol->delEdge(edgesAdded[2]);
			neigh->sol->delEdge(edgesAdded[3]);
			std::vector<boolEdge> mv;
			mv.push_back(edgesAdded[2]);
			mv.push_back(edgesAdded[3]);
			delete[] edgesAdded;
			tabuSpins.add(mv);
			neigh->sol->delEdge(edgeToAdd);
			continue;
		}

		neigh->sol->delEdge(edgeToAdd);
		neigh->sol->addEdge(neigh->deltdEdges[0]);

		for (int i = 0; i < 4; i++) {
			std::cout << "double spinned " << i << ": " << 
				edgesAdded[i].orig << "-" <<
				edgesAdded[i].dest << std::endl;
		}

		neigh->deltdEdges.push_back(edgesAdded[0]);
		neigh->deltdEdges.push_back(edgesAdded[1]);
		delete[] edgesAdded;

		return true;
	}
	//POINT OF kNOw RETURN
}

bool NeighbourhoodSearch::neighbourhoodStep(
		Neighbour* neigh, TabuList<bool>* tabuList,	bool aspirationCrit) {

	NeighbourStatus delStatus = predictDelActionStatus(neigh);

	std::cout << "deleted edge: " << 
		neigh->deltdEdges[0].orig << "-" <<
		neigh->deltdEdges[0].dest << " (status " <<
		delStatus << ')' << std::endl;
	switch (delStatus) {
		case del2mindeg:
			return swap(neigh);
		case del1mindeg:
			return spinMinDegree(neigh);
		default:
			break;
	}

	//no problem with the edge to be deleted.
	//Select edge to add and verify if it has problems	
	TabuList<bool> tabuEdgesToAdd;

	if (!aspirationCrit) {
		for (size_t i = 0; i < tabuList->size(); i++)
			tabuEdgesToAdd.add(tabuList->at(i));
	}

	boolEdge edgeToAdd;
	NeighbourStatus addStatus;

	while (true) {
		edgeToAdd = neigh->sol->selectRandomEdge(
				&tabuEdgesToAdd, false);
		if (neigh->sol->isEdgeInvalid(edgeToAdd, false))
			return false;

		edgeToAdd.value = !neigh->sol->getNullEdgeValue();
		addStatus = predictAddActionStatus(neigh, edgeToAdd);

		switch (addStatus) {
			case (add1maxdeg):
				if (!spinMaxDegree(neigh, edgeToAdd)) {
					tabuEdgesToAdd.add(edgeToAdd);
					continue;
				}
				break;

			case (add2maxdeg):
				if (!doubleSpinMaxDegree(neigh, edgeToAdd)) {
					tabuEdgesToAdd.add(edgeToAdd);
					continue;
				}
				break;
			default:
				break;
		}
		//default case. Add edge
		neigh->sol->delEdge(neigh->deltdEdges[0]);
		neigh->sol->addEdge(edgeToAdd);
		neigh->isTabu = tabuList->isTabu(neigh->sol);
		//if aspirationCrit = false, a tabu edge may have been selected.
		//This value may have already been set in previously called
		//methods.
		if (!aspirationCrit && neigh->isTabu) {
			neigh->sol->addEdge(neigh->deltdEdges[0]);
			neigh->sol->delEdge(edgeToAdd);
			tabuEdgesToAdd.add(edgeToAdd);
			continue;
		}

		std::cout << "added edge: " << 
			edgeToAdd.orig << "-" <<
			edgeToAdd.dest << " (status " <<
			addStatus << ')' << std::endl;
		return true;
	}

	return false;
}

typename NeighbourhoodSearch::Neighbour
NeighbourhoodSearch::generateNeighbour(
		TabuAdjMatrix<bool>* currSol, TabuList<bool>* tabuList,
		bool aspirationCrit) {
	//sets the environment
	//creates a copy of the current solution
	TabuAdjMatrix<bool>* solCopy = new TabuAdjMatrix<bool>(
			currSol->getNumNodes(), currSol->getNullEdgeValue());
	GraphConverter::convert(currSol, solCopy);
	//tabuList
	this->tabuList = tabuList;
	this->aspirationCrit = aspirationCrit;

	//creates tabuList of edges to be deleted (edges that cannot be
	//removed without violating the aspiration criteria
	TabuList<bool> delTabuList;

	boolEdge edgeToDel = solCopy->selectRandomEdge(&delTabuList);

	NeighbourhoodSearch::Neighbour neigh;
	neigh.sol = solCopy;
	neigh.deltdEdges.push_back(edgeToDel);
	neigh.isTabu = false;

	std::cout << std::endl;
	while (! neighbourhoodStep(&neigh, tabuList, aspirationCrit)) {
		//delete neighbour; SHOULD NOT BE NECESSARY TODO: DEBUG
		std::cout << std::endl;
		delTabuList.add(edgeToDel);

		if (delTabuList.size() == currSol->getNumEdges()) {
			deallocateNeighbour(&neigh);
			break;
		}

		//neighbour = currSol->copy(); SHOULD NOT BE NECESSARY TODO: DEBUG
		edgeToDel = neigh.sol->selectRandomEdge(&delTabuList);
		neigh.deltdEdges[0] = edgeToDel;
	}
	std::cout << "EXIT" << std::endl;

	this->tabuList = NULL;

	return neigh;
}

NeighbourhoodSearch::NeighbourhoodSearch() {
	minDegree = maxDegree = 0;
	tabuList = NULL;
	aspirationCrit = false;
}

NeighbourhoodSearch::~NeighbourhoodSearch() {
	if (tabuList != NULL)
		delete tabuList;
}

void NeighbourhoodSearch::deallocateNeighbour(Neighbour* neigh) {
	delete neigh->sol;
	neigh->sol = NULL;
}

void NeighbourhoodSearch::setDegreeLimits(size_t minDegree, size_t maxDegree) {
	if (maxDegree < minDegree)
		return;

	this->minDegree = minDegree;
	this->maxDegree = maxDegree;
}
