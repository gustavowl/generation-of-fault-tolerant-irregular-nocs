template <class T>
typename TabuSearch<T>::NeighbourStatus TabuSearch<T>::predictActionStatus(
		const TabuAdjMatrix<bool>* graph, grEdge edge, bool add) {
	NeighbourStatus status = dflt;

	if (add) {
		//add action
		if (graph->getNodeDegree(edge.orig) == MAX_DEGREE && 
				graph->getNodeDegree(edge.dest) == MAX_DEGREE) {
			status = add2deg4;
		}
		
		else if (graph->getNodeDegree(edge.orig) == MAX_DEGREE ||
				graph->getNodeDegree(edge.dest) == MAX_DEGREE) {
			status = add1deg4;
		}
	}
	else {
		//delete action
		if (graph->getNodeDegree(edge.orig) == MIN_DEGREE && 
				graph->getNodeDegree(edge.dest) == MIN_DEGREE) {
			status = del2deg2;
		}
		
		else if (graph->getNodeDegree(edge.orig) == MIN_DEGREE ||
				graph->getNodeDegree(edge.dest) == MIN_DEGREE) {
			status = del1deg2;
		}
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
	NeighbourStatus delStatus = predictActionStatus(neighbour, edgeToDel,
			false);
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
typename TabuSearch<T>::Movement TabuSearch<T>::randomNeighbourhoodStep(
		const TabuAdjMatrix<bool>* currSol,
		const std::vector<size_t*>* tabuList, bool aspirationCrit) {
	size_t* edgeToDel = NULL, edgeToAdd = NULL;
	while (edgeToAdd == NULL) {
		if (edgeToDel != NULL)
			delete[] edgeToDel;

		edgeToDel = selectEdgeToDel(neighbour);
		edgeToAdd = selectEdgeToAdd(neighbour, edgeToDel, tabuList,
				aspirationCrit);
	}

	return Movement{edgeToDel, edgeToAdd};
}

template <class T>
void TabuSearch<T>::generateNeighbour(const TabuAdjMatrix<bool>* currSol,
		std::vector<size_t>* tabuList, bool aspirationCrit) {
	TabuAdjMatrix<bool>* neighbour = currSol->copy();
	grEdge edgeToDel = neighbour->selectRandomEdge();
	while (true) {
	}

	//diferent feasibleness procedures are triggered depending on the movement
	NeighbourStatus delStatus = predictActionStatus(neighbour, edgeToDel);

	if (delStatus == del2deg2) {
		swapEdgesNodes(neighbour, mov, tabuList, aspirationCrit);
		return false;
		//the mov.edgeToAdd was previously choosen accordingly to
		//delStatus in method selectNeighbourhoodStep() and selectEdgeToAdd().
		//There is no need to verify the remaining delStatus possibilities
	}

	switch (addStatus) {
		case add1deg4:
			spinEdge(neighbour, mov.edgeToAdd, tabuList, aspirationCrit);
			break;
		case add2deg4:
			spinEdges(neighbour, mov.edgeToAdd, tabuList, aspirationCrit);
		default:
			break; //mov will generate a feasible solution
	}


	delete neighbour;
}
