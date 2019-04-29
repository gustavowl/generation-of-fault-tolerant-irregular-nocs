typename NeighbourhoodSearch::NeighbourStatus
NeighbourhoodSearch::predictDelActionStatus(
		Neighbour* neigh) {

	NeighbourStatus status = dflt;
	boolEdge edge = neigh->deltdEdge;

	//delete action
	if (neigh->sol->getNodeDegree(edge.orig) == MIN_DEGREE && 
			neigh->sol->getNodeDegree(edge.dest) == MIN_DEGREE) {
		status = del2deg2;
	}
	else if (neigh->sol->getNodeDegree(edge.orig) == MIN_DEGREE ||
			neigh->sol->getNodeDegree(edge.dest) == MIN_DEGREE) {
		status = del1deg2;
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
	if (addOrigDeg > MAX_DEGREE && addDestDeg > MAX_DEGREE) {
		status = add2deg4;
	}
	
	else if (addOrigDeg > MAX_DEGREE || addDestDeg > MAX_DEGREE) {
		status = add1deg4;
	}

	return status;
}

bool NeighbourhoodSearch::swap(Neighbour* neigh, TabuList<bool>* tabuList,
		bool aspirationCrit) {

	TabuList<bool> tabuSlctdEdges;
	if (!aspirationCrit) { //populate tabuList
		for (size_t i = 0; i < tabuList->size(); i++)
			tabuSlctdEdges.add(tabuList->at(i));
	}

	//select edge to swap
	edgeToAdd = neigh->sol->selectRandomEdge(&tabuSlctdEdges);
	boolEdge edgeToSwap1, edgeToSwap2;


	while (true) {
		if (neighbour->isEdgeInvalid(edgeToAdd))
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
					&tabuList);
		}
		//edges swapped may not be valid. Add edgeToAdd to a
		//tabuList and select another edge.	
		if (neigh->sol->isEdgeInvalid(edgeToSwap1) ||
				neigh->sol->isEdgeInvalid(edgeToSwap2)) {

			tabuSlctdEdges.push_back(edgeToAdd);
			edgeToAdd = neighbour->selectRandomEdge(&tabuSlctdEdges);
			continue;
		}
		//check if edges swapp are tabu
		neigh->isTabu = tabuList->isTabu(edgeToSwap1) ||
			tabuList->isTabu(edgeToSwap2);
		return true;
	}
	//POINT OF kNOw RETURN
}

bool NeighbourhoodSearch::spinMinDegree(Neighbour* neigh,
		TabuList<bool>* tabuList, bool aspirationCrit) {
	//selects node to be fixed (remain unchanged in the spinning process)
	boolEdge edgeToDel = neigh->deltdEdge;
	size_t fixedNode = (neigh->sol->getNodeDegree(edgeToDel.orig) ==
			MIN_DEGREE) ? edgeToDel.orig : edgeToDel.dest;

	boolEdge spinned;

	if (aspirationCrit) {
		TabuList<bool> emptyTabu;
		spinned = neigh->sol->spinEdge(edgeToDel, fixedNode, MAX_DEGREE,
				&emptyTabu);
	}
	else {
		spinned = neigh->sol->spinEdge(edgeToDel, fixedNode, MAX_DEGREE,
				tabuList);
	}

	neigh->isTabu = tabuList->isTabu(spinned);

	return !neighbour->isEdgeInvalid(spinned);
}

bool NeighbourhoodSearch::spinMaxDegree(Neighbour* neigh,
		boolEdge edgeToAdd, TabuList<bool>* tabuList,
		bool aspirationCrit) {
	//select node with max degree.
	//It will have one edge spinned randomly.
	//The centre of the spin is the node adjacent to
	//the one with max degree./
	size_t spinCentre = (neighbour->getNodeDegree(edgeToAdd.orig) ==
			MAX_DEGREE) ? edgeToAdd.dest : edgeToAdd.orig;

	neigh->sol->delEdge(neigh->deltdEdge);
	//select edge incident to spinCentre randomly,
	//with the exception of the edgeToDel.
	TabuList<bool> tabuSpins;
	boolEdge edgeToSpin, spinned;

	TabuList<bool> tabuEdgesToAdd;
	if (!aspirationCrit)
		for (size_t = 0; i < tabuList->size(); i++)
			tabuEdgesToAdd.add(tabuList->at(i));
	tabuEdgesToAdd.add(neigh->deltdEdge);

	while(tabuSpins.size() < neighbour->getNodeDegree(spinCentre)) {
		edgeToSpin = neighbour->selectRandomEdge(spinCentre, MAX_DEGREE,
				true, &tabuSpins);

		spinned = neigh->sol->spinEdge(edgeToSpin, spinCentre,
					MAX_DEGREE, &tabuEdgesToAdd);

		if (neighbour->isEdgeInvalid(spinned)) {
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

bool NeighbourhoodSearch::doubleSpinMaxDegree(
		TabuAdjMatrix<bool>* neighbour,	boolEdge edgeToDel,
		boolEdge edgeToAdd, TabuList<bool>* tabuList,
		bool aspirationCrit) {

	neighbour->delEdge(edgeToDel);
	bool success;

	if (aspirationCrit) {
		TabuList<bool> emptyTabu;
		success = neighbour->doubleSpinEdge(edgeToAdd, MAX_DEGREE,
				emptyTabu);
	}
	else {
		ret = neighbour->doubleSpinEdge(edgeToAdd, MAX_DEGREE, tabuList);
	}

	neighbour->addEdge(edgeToDel);
	return success;
}

bool NeighbourhoodSearch::neighbourhoodStep(
		Neighbour* neigh, TabuList<bool>* tabuList,	bool aspirationCrit) {

	NeighbourStatus delStatus = this->predictDelActionStatus(neigh);
	boolEdge edgeToAdd;

	switch (delStatus) {
		case del2mindeg:
			return this->swap(neigh, tabuList, aspirationCrit);
		case del1mindeg:
			return this->spin(neigh, tabuList, aspirationCrit);
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
				false, &tabuEdgesToAdd);
		addStatus = this->predictAddActionStatus(neigh, edgeToAdd, true);

		switch (addStatus) {
			case (add1maxdeg):
				if (!this->spinMaxDegree(neigh, edgeToAdd,
							tabuList, aspirationCrit)) {
					tabuEdgesToAdd.add(edgeToAdd);
					continue;
				}
				break;

			case (add2maxdeg):
				if (!this->doubleSpinEdge(neigh, edgeToAdd, tabuList,
							aspirationCrit)) {
					tabuEdgesToAdd.add(edgeToAdd);
					continue;
				}
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

Neighbour NeighbourhoodSearch::generateNeighbour(
		const TabuAdjMatrix<bool>* currSol,
		TabuList<bool>* tabuList, bool aspirationCrit) {
	//creates a copy of the current solution
	TabuAdjMatrix<bool>* solCopy = currSol->copy();
	//creates tabuList of edges to be deleted (edges that cannot be
	//removed without violating the aspiration criteria
	TabuList<bool> delTabuList;

	boolEdge edgeToDel = solCopy->selectRandomEdge(&delTabuList);

	Neighbour neigh {
		.sol = solCopy,
		.deltdEdge = edgeToDel,
		.isTabu = false
	};

	while (! this->neighbourhoodStep(&neigh, tabuList,
				aspirationCrit)) {
		//delete neighbour; SHOULD NOT BE NECESSARY TODO: DEBUG
		delTabuList.add(edgeToDel);

		if (delTabuList.size() == currSol->getNumEdges()) {
			deallocateNeighbour(&neighbour);
			return neigh;
		}

		//neighbour = currSol->copy(); SHOULD NOT BE NECESSARY TODO: DEBUG
		edgeToDel = neigh->sol->selectRandomEdge(&delTabuList);
		neigh.deltdEdge = edgeToDel;
	}

	return neigh;
}

void NeighbourhoodSearch::deallocateNeighbour(Neighbour* neigh) {
	delete neigh->sol;
	neigh->sol = NULL;
}
