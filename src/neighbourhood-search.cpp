#include "include/neighbourhood-search.h"

typename NeighbourhoodSearch::NeighbourStatus
NeighbourhoodSearch::predictDelActionStatus(
		Neighbour* neigh) {

	NeighbourStatus status = dflt;
	boolEdge edge = neigh->deltdEdge;

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
	if (edgeToAdd.orig == neigh->deltdEdge.orig ||
			edgeToAdd.orig == neigh->deltdEdge.dest) {
		addOrigDeg--;
	}
	if (edgeToAdd.dest == neigh->deltdEdge.orig ||
			edgeToAdd.dest == neigh->deltdEdge.dest) {
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

		edgeToSwap1 = neigh->deltdEdge;
		edgeToSwap2 = edgeToAdd;

		if (aspirationCrit) {
			//tabuSlctEdges' element will never conflict here
			neigh->sol->swapEdgesNodes(&edgeToSwap1, &edgeToSwap2,
					&tabuSlctdEdges);
		}
		else {
			neigh->sol->swapEdgesNodes(&edgeToSwap1, &edgeToSwap2,
					tabuList);
		}
		//edges swapped may not be valid. Add edgeToAdd to a
		//tabuList and select another edge.	
		if (neigh->sol->isEdgeInvalid(edgeToSwap1) ||
				neigh->sol->isEdgeInvalid(edgeToSwap2)) {

			tabuSlctdEdges.add(edgeToAdd);
			edgeToAdd = neigh->sol->selectRandomEdge(&tabuSlctdEdges);
			continue;
		}
		//check if edges swapped are tabu
		neigh->isTabu = tabuList->isTabu(edgeToSwap1) ||
			tabuList->isTabu(edgeToSwap2);
		return true;
	}
	//POINT OF kNOw RETURN
}

bool NeighbourhoodSearch::spinMinDegree(Neighbour* neigh) {
	//selects node to be fixed (remain unchanged in the spinning process)
	boolEdge edgeToDel = neigh->deltdEdge;
	size_t fixedNode = (neigh->sol->getNodeDegree(edgeToDel.orig) ==
			minDegree) ? edgeToDel.orig : edgeToDel.dest;

	boolEdge spinned;

	if (aspirationCrit) {
		TabuList<bool> emptyTabu;
		spinned = neigh->sol->spinEdge(edgeToDel, fixedNode, maxDegree,
				&emptyTabu);
	}
	else {
		spinned = neigh->sol->spinEdge(edgeToDel, fixedNode, maxDegree,
				tabuList);
	}

	neigh->isTabu = tabuList->isTabu(spinned);

	return !neigh->sol->isEdgeInvalid(spinned);
}

bool NeighbourhoodSearch::spinMaxDegree(Neighbour* neigh,
		boolEdge edgeToAdd) {
	//select node with max degree.
	//It will have one edge spinned randomly.
	//The centre of the spin is the node adjacent to
	//the one with max degree./
	size_t maxDegNode = (neigh->sol->getNodeDegree(edgeToAdd.orig) ==
			maxDegree) ? edgeToAdd.orig : edgeToAdd.dest;

	neigh->sol->delEdge(neigh->deltdEdge);
	//select edge incident to spinCentre randomly,
	//with the exception of the edgeToDel.
	TabuList<bool> tabuSpins;
	boolEdge edgeToSpin, spinned;

	TabuList<bool> tabuEdgesToAdd;
	if (!aspirationCrit)
		for (size_t i = 0; i < tabuList->size(); i++)
			tabuEdgesToAdd.add(tabuList->at(i));
	tabuEdgesToAdd.add(neigh->deltdEdge);

	while(tabuSpins.size() < neigh->sol->getNodeDegree(maxDegNode)) {
		edgeToSpin = neigh->sol->selectRandomEdge(maxDegNode, &tabuSpins,
				true);

		size_t spinCentre = (neigh->sol->getNodeDegree(edgeToSpin.orig)
				== maxDegree) ? edgeToSpin.dest : edgeToSpin.orig;

		spinned = neigh->sol->spinEdge(edgeToSpin, spinCentre,
					maxDegree, &tabuEdgesToAdd);

		if (neigh->sol->isEdgeInvalid(spinned)) {
			tabuSpins.add(edgeToSpin);
			continue;
		}

		neigh->sol->addEdge(neigh->deltdEdge);
		neigh->isTabu = tabuList->isTabu(spinned);
		return true;
	}

	neigh->sol->addEdge(neigh->deltdEdge);
	return false;
}

bool NeighbourhoodSearch::doubleSpinMaxDegree( Neighbour* neigh,
		boolEdge edgeToAdd) {

	neigh->sol->delEdge(neigh->deltdEdge);

	TabuList<bool> augmentedTabu;
	if (!aspirationCrit)
		for (size_t i = 0; i < tabuList->size(); i++)
			augmentedTabu.add(tabuList->at(i));
	augmentedTabu.add(neigh->deltdEdge);

	boolEdge* edgesAdded = neigh->sol->doubleSpinEdge(
			edgeToAdd, maxDegree, &augmentedTabu);

	neigh->sol->addEdge(neigh->deltdEdge);

	if (edgesAdded == NULL)
		return false;

	neigh->isTabu = tabuList->isTabu(edgesAdded[0]) ||
		tabuList->isTabu(edgesAdded[1]);
	delete[] edgesAdded;

	return true;
}

bool NeighbourhoodSearch::neighbourhoodStep(
		Neighbour* neigh, TabuList<bool>* tabuList,	bool aspirationCrit) {

	NeighbourStatus delStatus = predictDelActionStatus(neigh);

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

	size_t possibilities = neigh->sol->maxNumEdges() -
		neigh->sol->getNumEdges() - tabuEdgesToAdd.size();
	boolEdge edgeToAdd;
	NeighbourStatus addStatus;

	for (; possibilities > 0; possibilities--) {
		edgeToAdd = neigh->sol->selectRandomEdge(
				&tabuEdgesToAdd, false);
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
		neigh->sol->delEdge(neigh->deltdEdge);
		neigh->sol->addEdge(edgeToAdd);
		//if aspirationCrit = false, a tabu edge may have been selected
		if (tabuList->isTabu(edgeToAdd))
			neigh->isTabu = true;

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

	NeighbourhoodSearch::Neighbour neigh {
		.sol = solCopy,
		.deltdEdge = edgeToDel,
		.isTabu = false
	};

	while (! neighbourhoodStep(&neigh, tabuList, aspirationCrit)) {
		//delete neighbour; SHOULD NOT BE NECESSARY TODO: DEBUG
		delTabuList.add(edgeToDel);

		if (delTabuList.size() == currSol->getNumEdges()) {
			deallocateNeighbour(&neigh);
			return neigh;
		}

		//neighbour = currSol->copy(); SHOULD NOT BE NECESSARY TODO: DEBUG
		edgeToDel = neigh.sol->selectRandomEdge(&delTabuList);
		neigh.deltdEdge = edgeToDel;
	}

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
