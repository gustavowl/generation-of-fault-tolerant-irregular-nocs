template <class T>
typename TabuSearch<T>::NeighbourStatus TabuSearch<T>::predictDelActionStatus(
		const TabuAdjMatrix<bool>* graph, grEdge edge) {
	NeighbourStatus status = dflt;

	//delete action
	if (graph->getNodeDegree(edge.orig) == MIN_DEGREE && 
			graph->getNodeDegree(edge.dest) == MIN_DEGREE) {
		status = del2deg2;
	}
	else if (graph->getNodeDegree(edge.orig) == MIN_DEGREE ||
			graph->getNodeDegree(edge.dest) == MIN_DEGREE) {
		status = del1deg2;
	}

	return status;
}

template <class T>
typename TabuSearch<T>::NeighbourStatus TabuSearch<T>::predictAddActionStatus(
		const TabuAdjMatrix<bool>* graph, grEdge edgeToDel, grEdge edgeToAdd) {

	NeighbourStatus status = dflt;

	//simulates the degrees after edgeToDel is deleted
	size_t addOrigDeg = graph->getNodeDegree(edgeToAdd.orig) + 1;
	size_t addDestDeg = graph->getNodeDegree(edgeToAdd.dest) + 1;
	if (edgeToAdd.orig == edgeToDel.orig || edgeToAdd.orig == edgeToDel.dest)
		addOrigDeg--;
	if (edgeToAdd.dest == edgeToDel.orig || edgeToAdd.dest == edgeToDel.dest)
		addDestDeg--;

	//add action
	if (addOrigDeg > MAX_DEGREE && addDestDeg > MAX_DEGREE) {
		status = add2deg4;
	}
	
	else if (addOrigDeg > MAX_DEGREE || addDestDeg > MAX_DEGREE) {
		status = add1deg4;
	}

	return status;
}

template <class T>
size_t* TabuSearch<T>::selectEdgeToDel(TabuAdjMatrix<bool>* neighbour) {
	return neighbour->selectRandomEdge();
}

template <class T>
size_t* TabuSearch<T>::selectEdgeToAdd(TabuAdjMatrix<bool>* neighbour,
			size_t* edgeToDel, std::vector<size_t>* tabuList,
			bool aspirationCrit) {
	NeighbourStatus delStatus = predictDelActionStatus(neighbour, edgeToDel);
	size_t* edgeToAdd = NULL;

	switch (delStatus) {
		case del2deg2:
			//selects a random edge and ensurres that it is not incident
			//to whichever node in edgeToDel. This would cause a
			//disconnected graph to be generated
			while (true) {
				//chooses edge to swap
				edgeToAdd = selectRandomEdge(neighbour);

				if (edgeToDel[0] == edgeToAdd[0] ||
						edgeToDel[0] == edgeToAdd[1] ||
						edgeToDel[1] == edgeToAdd[0] ||
						edgeToDel[1] == edgeToAdd[1]) {
					delete[] edgeToAdd;
					continue;
				}

				if (!aspirationCrit && isInTabuList(tabuList, TODO CHECK SWAP)) {
					delete[] edgeToAdd;
					continue;
				}
					
				return edgeToAdd;
			}
			//POINT OF kNOw RETURN
		case del1deg2:
			size_t node = (neighbour->getNodeDegree(edgeToDel[0]) == MIN_DEGREE)?
				edgeToDel[0] : edgeToDel[1];

			while (true) {
				edgeToAdd = selectRandomEdge(neighbour, node, false);

				if (!aspirationCrit && isInTabuList(tabuList, edgeToAdd)) {
					delete[] edgeToAdd;
					continue;
				}
				
				return edgeToAdd;
			}
			//POINT OF kNOw RETURN
		default:
			while (true) {
				edgeToAdd = selectRandomEdge(neighbour, false);

				if (!aspirationCrit && isInTabuList(tabuList, edgeToAdd)) {
					delete[] edgeToAdd;
					continue;
				}

				return edgeToAdd;
			}
	}
}

template <class T>
bool TabuSearch<T>::swap(TabuAdjMatrix<bool>* neighbour,
		grEdge edgeToDel, TabuList<bool>* tabuList, bool aspirationCrit) {

	TabuList<T> tabuSlctdEdges;
	if (!aspirationCrit) { //populate tabuList
		for (size_t i = 0; i < tabuList->size(); i++)
			tabuSlctdEdges.add(tabuList->at(i));
	}

	//select edge to swap
	edgeToAdd = neighbour->selectRandomEdge(&tabuSlctdEdges);
	grEdge edgeToSwap1, edgeToSwap2;


	while (true) {
		if (neighbour->isEdgeInvalid(edgeToAdd))
			return false;

		edgeToSwap1 = edgeToDel;
		edgeToSwap2 = edgeToAdd;

		if (aspirationCrit) {
			//tabuSlctEdges' element will never conflict here
			neighbour->swapEdgesNodes(&edgeToSwap1, &edgeToSwap2,
					&tabuSlctdEdges);
		}
		else {
			neighbour->swapEdgesNodes(&edgeToSwap1, &edgeToSwap2,
					&tabuList);
		}
		//edges swapped may not be valid. Add edgeToAdd to a
		//tabuList and select another edge.	
		if (neighbour->isEdgeInvalid(edgeToSwap1) ||
				neighbour->isEdgeInvalid(edgeToSwap2)) {

			tabuSlctdEdges.push_back(edgeToAdd);
			edgeToAdd = neighbour->selectRandomEdge(&tabuSlctdEdges);
			continue;
		}
		return true;
	}
	//POINT OF kNOw RETURN
}

template <class T>
bool TabuSearch<T>::spinMinDegree(TabuAdjMatrix<bool>* neighbour,
		grEdge edgeToDel, TabuList<bool>* tabuList, bool aspirationCrit) {
	//selects node to be fixed (remain unchanged in the spinning process)
	size_t fixedNode = (neighbour->getNodeDegree(edgeToDel.orig) == MIN_DEGREE) ?
		edgeToDel.orig : edgeToDel.dest;

	grEdge spinned;

	if (aspirationCrit) {
		TabuList<bool> emptyTabu;
		spinned = neighbour->spinEdge(edgeToDel, fixedNode, MAX_DEGREE, &emptyTabu);
	}
	else {
		spinned = neighbour->spinEdge(edgeToDel, fixedNode, MAX_DEGREE, tabuList);
	}

	return !neighbour->isEdgeInvalid(spinned);
}

template <class T>
bool TabuSearch<T>::spinMaxDegree(TabuAdjMatrix<bool>* neighbour,
		grEdge edgeToAdd, TabuList<bool>* tabuList, bool aspirationCrit) {
	//select node with max degree.
	//It will have one edge spinned randomly.
	//The centre of the spin is the node adjacent to
	//the one with max degree./
	size_t spinCentre = (neighbour->getNodeDegree(edgeToAdd.orig) == MAX_DEGREE) ?
		edgeToAdd.dest : edgeToAdd.orig;

	//select edge incident to spinCentre randomly,
	//with the exception of the edgeToDel.
	TabuList<bool> tabuSpins;
	grEdge edgeToSpin, spinned;

	while(tabuSpins.size() < neighbour->getNodeDegree(spinCentre)) {
		edgeToSpin = neighbour->selectRandomEdge(spinCentre, MAX_DEGREE,
				true, &tabuSpins);

		if (aspirationCrit) {
			TabuList<bool> emptyTabu;
			spinned = neighbour->spinEdge(edgeToSpin, spinCentre, MAX_DEGREE,
					&emptyTabu);
		}
		else {
			spinned = neighbour->spinEdge(edgeToSpin, spinCentre, MAX_DEGREE,
					tabuList);
		}

		if (neighbour->isEdgeInvalid(spinned)) {
			tabuSpins.add(edgeToSpin);
			continue;
		}

		return true;
	}
	return false;
}

template <class T>
bool TabuSearch<T>::doubleSpinMaxDegree(TabuAdjMatrix<bool>* neighbour,
		grEdge edgeToDel, grEdge edgeToAdd, TabuList<bool>* tabuList,
		bool aspirationCrit) {

	neighbour->delEdge(edgeToDel);
	bool success;

	if (aspirationCrit) {
		TabuList<bool> emptyTabu;
		success = neighbour->doubleSpinEdge(edgeToAdd, MAX_DEGREE, emptyTabu);
	}
	else {
		ret = neighbour->doubleSpinEdge(edgeToAdd, MAX_DEGREE, tabuList);
	}

	neighbour->addEdge(edgeToDel);
	return success;
}

template <class T>
bool TabuSearch<T>::neighbourhoodStep(TabuAdjMatrix<bool>* neighbour,
		grEdge edgeToDel, TabuList<bool>* tabuList, bool aspirationCrit) {

	NeighbourStatus delStatus = this->predictDelActionStatus(neighbour,
			edgeToDel);
	grEdge edgeToAdd;

	switch (delStatus) {
		case del2mindeg:
			return this->swap(neighbour, edgeToDel, tabuList, aspirationCrit);
		case del1mindeg:
			return this->spin(neighbour, edgeToDel, tabuList, aspirationCrit);
	}

	//no problem with the edge to be deleted.
	//Select edge to add and verity if it has problems	
	TabuList<bool> tabuEdgesToAdd;

	if (!aspirationCrit) {
		for (size_t i = 0; i < tabuList->size(); i++)
			tabuEdgesToAdd.add(tabuList->at(i));
	}

	size_t possibilities = neighbour->maxNumEdges() -
		neighbour->getNumEdges() - tabuEdgesToAdd.size();
	grEdge edgeToAdd;
	NeighbourStatus addStatus;

	for (; possibilities > 0; possibilities--) {
		edgeToAdd = neighbour->selectRandomEdge(false, &tabuEdgesToAdd);
		addStatus = this->predictAddActionStatus(neighbour, edgeToDel,
				edgeToAdd, true);

		switch (addStatus) {
			case (add1maxdeg):
				if (!this->spinMaxDegree(neighbour, edgeToAdd,
							tabuList, aspirationCrit)) {
					tabuEdgesToAdd.add(edgeToAdd);
					continue;
				}
				break;
			case (add2maxdeg):
				if (!this->doubleSpinEdge(neighbour, edgeToDel, edgeToAdd,
							tabuList, aspirationCrit)) {
					tabuEdgesToAdd.add(edgeToAdd);
					continue;
				}
				break;
		}
		//default case. Add edge
		neighbour->delEdge(edgeToDel);
		neighbour->addEdge(edgeToAdd);
		return true;
	}

	return false;
}

template <class T>
void TabuSearch<T>::generateNeighbour(const TabuAdjMatrix<bool>* currSol,
		TabuList<bool>* tabuList, bool aspirationCrit) {
	//creates a copy of the current solution
	TabuAdjMatrix<bool>* neighbour = currSol->copy();
	//creates tabuList of edges to be deleted (edges that cannot be
	//removed without violating the aspiration criteria
	TabuList<bool> delTabuList;

	grEdge edgeToDel = neighbour->selectRandomEdge(&delTabuList);

	while (! this->neighbourhoodStep(neighbour, edgeToDel,
				tabuList, aspirationCrit)) {
		//delete neighbour; SHOULD NOT BE NECESSARY TODO: DEBUG
		delTabuList.add(edgeToDel);

		if (delTabuList.size() == currSol->getNumEdges()) {
			detele neighbour;
			return NULL;
		}

		//neighbour = currSol->copy(); SHOULD NOT BE NECESSARY TODO: DEBUG
		edgeToDel = neighbour->selectRandomEdge(&delTabuList);
	}

	return neighbour;
}
