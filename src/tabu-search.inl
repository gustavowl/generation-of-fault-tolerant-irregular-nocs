template <class T>
void TabuSearch<T>::fitToEpsilon(AdjacencyMatrix<bool>* initSol,
		size_t epsilon) {
	//saves nodes degrees
	size_t size = initSol->getNumNodes();
	size_t degrees[initSol->getNumNodes()];
	for (size_t i = 0; i < initSol->getNumNodes(); i++)
		degrees[i] = initSol->getNodeDegree(i);

	while (epsilon < initSol->getNumEdges()) {
		//removes edges
		//computes positions of the two nodes with largest degrees
		size_t largest = 0;
		size_t largest2 = 0;

		for (size_t i = 1; i < size; i++) {
			if (degrees[i] > degrees[largest]) {
				largest2 = largest;
				largest = i;
				continue;
			}
			if (degrees[i] > degrees[largest2])
				largest2 = i;
		}

		initSol->delEdge(largest, largest2);
		degrees[largest]--;
		degrees[largest2]--;
	}

	while (epsilon > initSol->getNumEdges()) {
		//adds edges
		//computes positions of the two nodes with smallest degrees
		size_t smallest = 0;
		size_t smallest2 = 0;

		for (size_t i = 1; i < size; i++) {
			if (degrees[i] < degrees[smallest]) {
				smallest2 = smallest;
				smallest = i;
				continue;
			}
			if (degrees[i] < degrees[smallest2])
				smallest2 = i;
		}

		initSol->addEdge(smallest, smallest2, true);
		degrees[smallest]++;
		degrees[smallest2]++;
	}
}

template <class T>
bool TabuSearch<T>::isFeasible(AdjacencyMatrix<bool>* initSol) {
	size_t size = initSol->getNumNodes();
	size_t degree = 0;
	for (size_t i = 0; i < size; i++) {
		degree = initSol->getNodeDegree(i);
		if (degree < 2 || degree > 4)
			return false;
	}
	return true;
}

template <class T>
void TabuSearch<T>::makeFeasible(AdjacencyMatrix<bool>* initSol) {
	//sets initial variables
	size_t size = initSol->getNumNodes();
	size_t degrees[size];
	for (size_t i = 0; i < size; i++)
		degrees[i] = initSol->getNodeDegree(i);
	std::vector<size_t> neighbours;

	//while (not feasible)
	while (!isFeasible(initSol)) {
		//1 - remove edge
		//	1.1 - identify node with largest degree
		size_t largest = 0;
		for (size_t i = 1; i < size; i++) {
			if (degrees[i] > degrees[largest])
				largest = i;
		}
		//	1.2 - identify its neighbour with largest degree
		neighbours = initSol->getNeighbours(largest);
		size_t lrgNeighbour = neighbours[0];
		for (size_t i = 1; i < neighbours.size(); i++) {
			if (degrees[neighbours[i]] > degrees[lrgNeighbour])
				lrgNeighbour = neighbours[i];
		}
		//	1.3 - remove edge
		initSol->delEdge(largest, lrgNeighbour);
		degrees[largest]--;
		degrees[lrgNeighbour]--;
		
		//2 - add edge
		//	2.1 - identify node with smallest degree
		size_t smallest = 0;
		for (size_t i = 1; i < size; i++) {
			if (degrees[i] < degrees[smallest])
				smallest = i;
		}

		//	2.2 - create empty tabuList of target nodes
		bool tabuList[size];
		for (size_t i = 0; i < size; i++)
			tabuList[i] = false;
		tabuList[smallest] = true;

		while (true) {
			//	2.3 - identify second node with smallest degree
			//		(not in tabuList)
			size_t smlNeighbour = 0; //first node not in tabu
			for (size_t i = 0; i < size; i++) {
				if (!tabuList[i]) {
					smlNeighbour = i;
					break;
				}
			}
			//second node not in tabu
			for (size_t i = smlNeighbour + 1; i < size; i++) {
				if (!tabuList[i] && degrees[i] < degrees[smlNeighbour])
					smlNeighbour = i;
			}
			//	2.4 - add edge
			if (initSol->edgeExists(smallest, smlNeighbour)) {
				//edge exists, add target node to tabuList
				tabuList[smlNeighbour] = true;
				continue;
			}
			initSol->addEdge(smallest, smlNeighbour, true);
			degrees[smallest]++;
			degrees[smlNeighbour]++;

			//3 - check if graph is disconnect
			//	3.1 - Dijkstra
			size_t numHops = Dijkstra<bool>::dijkstra(
					initSol, largest, lrgNeighbour, true,
					false).hops;
			//	3.2 - if disconnected
			if (numHops == HOP_INF) {
				//	3.2.1 - remove edge
				initSol->delEdge(smallest, smlNeighbour);
				degrees[smallest]--;
				degrees[smlNeighbour]--;
				//	3.2.2 - add target node to a TabuList
				tabuList[smlNeighbour] = true;
				//	3.2.3 - go back to step 2.3
				continue;
			}

			break;
		}
	}
}

template <class T>
AdjacencyMatrix<bool>* TabuSearch<T>::generateInitSol(
		const GraphRepresentation<T>* tg, size_t epsilon) {
	//checks wheter if epsilon is valid or not.
	//i.e. if it is possible to generate a graph such that
	//for all nodes, degree(node) in [2, 4]
	//REFER TO: degree sum formula / handshaking lemma
	if ((2*epsilon) / tg->getNumNodes() < 2 ||
			(2*epsilon) / tg->getNumNodes() > 4) {

		return NULL;
	}

	AdjacencyMatrix<bool>* initSol = new AdjacencyMatrix<bool>(
			tg->getNumNodes(), true, true, tg->getNullEdgeValue());
	//unable to generate graph
	if (initSol->getNumNodes() != tg->getNumNodes())
		return NULL;

	//copies task graph, converting representation
	GraphConverter::convert(tg, initSol);

	fitToEpsilon(initSol, epsilon);
	makeFeasible(initSol);

	return initSol;
}

template <class T>
T TabuSearch<T>::fitness(const GraphRepresentation<T>* tg,
		const AdjacencyMatrix<bool>* sol) {
	//computes QAP function:
	//for all edge(i, j) in tg
	//\sum min_hops(node_i, node_j) * weightOf(edge(i, j))
	T sum;
	bool firstFound = false;

	//Computes QAP for each edge in the task graph
	for (size_t i = 0; i < tg->getNumNodes(); i++) {
		for (size_t j = 0; j < tg->getNumNodes(); j++) {
			if (tg->edgeExists(i, j)) {

				size_t numHops = Dijkstra<bool>::dijkstra(
						sol, i, j, true, false).hops;

				T qap = numHops * tg->getEdgeValue(i, j);

				if (firstFound) {
					sum += qap;
					continue;
				}
				sum = qap;
				firstFound = true;
			}
		}
	}

	return sum; 
}

template <class T>
bool TabuSearch<T>::areEdgesEqual(size_t* edge1, size_t* edge2) {
	if ((edge1[0] == edge2[0] && edge1[1] == edge2[1]) ||
			(edge1[0] == edge2[1] && edge1[1] == edge2[0])) {
		return true;
	}
	return false;
}

template <class T>
bool TabuSearch<T>::isInTabuList(const std::vector<size_t*>* tabuList,
		Movement mov) {
	//searches tabuList
	size_t tabuEdge[2];
	for (size_t i = 0; i < tabuList->size(); i++) {
		tabuEdge[0] = tabuList->at(i)[0];
		tabuEdge[1] = tabuList->at(i)[1];
		
		//checks if any edge in movement is is tabuList
		if (areEdgesEqual(tabuEdge, mov.edgeAdded)) {
			return true;
		}
	}

	return false;
}

template <class T>
void TabuSearch<T>::addToTabuList(std::vector<size_t*>* tabuList,
		size_t* tabuIndex, Movement mov) {
	if (tabuList->size() == tabuList->capacity()) {
		//cycle
		tabuList->at(*tabuIndex)[0] = mov.edgeDeltd[0];
		tabuList->at(*tabuIndex)[1] = mov.edgeDeltd[1];
		*tabuIndex = (*tabuIndex + 1) % tabuList->size();

		return;
	}
	//tabuList is not full, no need to cycle
	size_t* newEdge = new size_t[2];
	newEdge[0] = mov.edgeDeltd[0];
	newEdge[1] = mov.edgeDeltd[1];
	tabuList->push_back(newEdge);
}

template <class T>
typename TabuSearch<T>::Movement TabuSearch<T>::getRandomNeighbour(
		const AdjacencyMatrix<bool>* currSol, size_t epsilon,
		const std::vector<size_t*>* tabuList, bool aspirationCrit) {

	size_t numNodes = currSol->getNumNodes();
	size_t* edgeToDel = new size_t[2];
   	size_t*	edgeToAdd = new size_t[2];
	size_t delCount, addCount, delRandom, addRandom;
	//no loops are allowed and the matrices are symmetric.
	//Thus, there are only (nodes*2 - nodes)/2 unique edges.
	//This result can be obtained through arithmetic progression
	size_t uniqueEdges = (numNodes * numNodes - numNodes) / 2;

	do {
		//search process
		delRandom = rand() % (epsilon);
		addRandom = rand() % (uniqueEdges - epsilon);
		delCount = addCount = 0;
		bool edgeExists, stop = false;

		//searches triangular matrix
		for (size_t i = 1; i < numNodes && !stop; i++) {
			for (size_t j = 0; j < i && !stop; j++) {
				edgeExists = currSol->edgeExists(i, j);
				if (!edgeExists && addCount <= addRandom) {
					if (addCount == addRandom) {
						edgeToAdd[0] = i;
						edgeToAdd[1] = j;
						stop = delCount > delRandom;
					}
					addCount++;
				}
				else if (edgeExists && delCount <= delRandom) {
					if (delCount == delRandom) {
						edgeToDel[0] = i;
						edgeToDel[1] = j;
						stop = addCount > addRandom;
					}
					delCount++;
				}
			}
		}

		if (!aspirationCrit) {
			if (! isInTabuList(tabuList, Movement {
						edgeToDel, edgeToAdd}) ) {
				
				aspirationCrit = true; //stop
			}
		}

	} while (!aspirationCrit);

//	std::cout << "delCount/delRandom" << delCount << '/' << delRandom << '\n';
//	std::cout << "addCount/addRandom" << addCount << '/' << addRandom << '\n';	
//	std::cout << "edgeToDel " << edgeToDel[0] << ' ' << edgeToDel[1];
//	std::cout << "\nedgeToAdd " << edgeToAdd[0] << ' ' << edgeToAdd[1] <<
//		'\n' << std::endl;
	return Movement {edgeToDel, edgeToAdd};
}

template <class T>
void TabuSearch<T>::makeMovement(AdjacencyMatrix<bool>* currSol,
		typename TabuSearch<T>::Movement mov, bool undo) {
	if (undo) {
		//currSol->delEdge(mov.edgeAdded[0], mov.edgeAdded[1]);
		//currSol->addEdge(mov.edgeDeltd[0], mov.edgeDeltd[1]);
		return;
	}

	//currSol->delEdge(mov.edgeDeltd[0], mov.edgeDeltd[1]);
	//currSol->addEdge(mov.edgeAdded[0], mov.edgeAdded[1]);
}

template <class T>
void TabuSearch<T>::deallocateMovement(Movement* mov) {
	delete[] mov->edgeDeltd;
	delete[] mov->edgeAdded;
}

template <class T>
void TabuSearch<T>::deallocateTabuList(std::vector<size_t*>* tabuList) {
	for (size_t i = 0; i < tabuList->size(); i++)
		delete[] tabuList->at(i);
}

template <class T>
AdjacencyMatrix<T>* TabuSearch<T>::start(
		const GraphRepresentation<T>* tg, size_t tabuListSize,
		size_t stopCriteria, size_t epsilon) {

	//no loops are allowed and the matrices are symmetric.
	//Thus, there are only (nodes*2 - nodes)/2 unique edges.
	//This result can be obtained through arithmetic progression
	if (tabuListSize >= (tg->getNumNodes() * tg->getNumNodes() -
				tg->getNumNodes()) / 2)
		return NULL; //this would cause an infinite loop

	AdjacencyMatrix<bool>* currSol = generateInitSol(tg, epsilon);
	if (currSol == NULL)
		return NULL;
	T currFit = fitness(tg, currSol);
	Movement currMov;


	std::vector<Movement> neighboursMov;
	std::vector<T> neighboursFit;
	
	//stores tabu edges
	//two edges are add/removed per movement (iteration)
	std::vector<size_t*> tabuList;
	tabuList.reserve(tabuListSize);
	//TODO: assert that tabuList.size() == 0
	size_t tabuIndex = 0; //used to simulate circular queue

	//creates a copy
	//TODO: set num edges (create copy const?)
	GraphRepresentation<bool>* bestSol = currSol->copy(); 
	T bestFit = currFit;
	size_t count = 0;
	size_t totalCount = 0;
	
	while(count < stopCriteria) {
		currSol->print();
		std::cout << count << '/' << stopCriteria << std::endl;

		//searches neighbourhood
		//searches first epsilon random neighbours
		//this is feasible since epsilon's upper bound is
		//2 * numNodes (check generateInitSol())
		for (size_t i = 0; i < epsilon; i++) {
			//generates random neighbourhood movements
			neighboursMov.push_back(getRandomNeighbour(currSol,
						epsilon, &tabuList));
			//computes neighbours' fitness
			makeMovement(currSol, neighboursMov[i]);
			neighboursFit.push_back(fitness(tg, currSol));
			makeMovement(currSol, neighboursMov[i], true);
		}

		//searches for aspiration criterea
		bool aspirationCrit = false;
		for (size_t i = 0; i < neighboursFit.size(); i++) {
			if (!aspirationCrit) {
				if (neighboursFit[i] < bestFit) {
					currFit = neighboursFit[i];
					currMov = neighboursMov[i];
					aspirationCrit = true;
				}
				continue;
			}
			//there may be multiple aspiration criterea
			//the best one is chosen
			if (neighboursFit[i] < currFit) {
				currFit = neighboursFit[i];
				currMov = neighboursMov[i];
			}
		}
		//if aspiration criterea not found,
		//search for the best solution not in tabuList
		if (!aspirationCrit) {
			size_t bestIndex;
			while (!neighboursMov.empty()) {
				bestIndex = 0;
				for (size_t i = 1; i < neighboursMov.size(); i++) {
					if (neighboursFit[i] < neighboursFit[bestIndex])
						bestIndex = i;
				}

				currMov = neighboursMov[bestIndex];
				currFit = neighboursFit[bestIndex];
			//	std::cout << currMov.edgeDeltd[0] << ' ' << currMov.edgeDeltd[1] << '\n';
			//	std::cout << currMov.edgeAdded[0] << ' ' << currMov.edgeAdded[1] << '\n';
			//	std::cin.ignore();

				if (isInTabuList(&tabuList, currMov)) {
					//tabu solution, search for next best neighbour
					deallocateMovement(&neighboursMov[bestIndex]);
					neighboursMov.erase(neighboursMov.begin() + bestIndex);
					neighboursFit.erase(neighboursFit.begin() + bestIndex);
					continue;
				}
				break; //non tabu best neighbour found
			}

			//if all are tabu, generate non tabu neighbour
			if (neighboursMov.empty()) {
				currMov = getRandomNeighbour(currSol, epsilon, &tabuList, false);
				//computes neighbours' fitness
				makeMovement(currSol, currMov);
				currFit = (fitness(tg, currSol));
				makeMovement(currSol, currMov, true);
			}
		}

		//changes to best neighbour solution found
		makeMovement(currSol, currMov);
		addToTabuList(&tabuList, &tabuIndex, currMov);

		if (neighboursMov.empty())
			deallocateMovement(&currMov);
		for (size_t i = 0; i < neighboursMov.size(); i++)
			deallocateMovement(&neighboursMov[i]);
		neighboursMov.clear();
		neighboursFit.clear();

		if (currFit < bestFit) {
			count = 0;
			delete bestSol;
			bestSol = currSol->copy();
			bestFit = currFit;
			continue;
		}
		count++;
		totalCount++;
	}

	delete currSol;
	//TODO: create new matrix from bestSol.
	//	Compute QAP for each edge, then return
	bestSol->print();
	std::cout << "Total iterations: " << totalCount << std::endl;
	AdjacencyMatrix<T>* ret = NULL;
	delete bestSol;

	deallocateTabuList(&tabuList);

	//TODO: return solution set
	return ret;
}
