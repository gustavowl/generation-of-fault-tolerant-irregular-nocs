template <class T>
void TabuSearch<T>::fitToEpsilon(AdjacencyMatrix<bool>* initSol) {
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
bool TabuSearch<T>::isFeasible(AdjacencyMatrix<bool>* sol) {
	size_t size = sol->getNumNodes();
	size_t degree = 0;
	for (size_t i = 0; i < size; i++) {
		degree = sol->getNodeDegree(i);
		if (degree < MIN_DEGREE || degree > MAX_DEGREE)
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
AdjacencyMatrix<bool>* TabuSearch<T>::generateInitSol() {
	//checks wheter if epsilon is valid or not.
	//i.e. if it is possible to generate a graph such that
	//for all nodes, degree(node) in [2, 4]
	//REFER TO: degree sum formula / handshaking lemma
	if ((2*epsilon) / tg->getNumNodes() < MIN_DEGREE ||
			(2*epsilon) / tg->getNumNodes() > MAX_DEGREE) {

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
T TabuSearch<T>::fitness(const AdjacencyMatrix<bool>* sol) {
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

				if (numHops == HOP_INF)
					return valueLimit;

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
void TabuSearch<T>::makeMovement(AdjacencyMatrix<bool>* graph, Movement mov,
		bool undo) {
	if (undo) {
		graph->delEdge(mov.edgeToAdd[0], mov.edgeToAdd[1]);
		graph->addEdge(mov.edgeToDel[0], mov.edgeToDel[1],
				! graph->getNullEdgeValue());
		return;
	}
	graph->delEdge(mov.edgeToDel[0], mov.edgeToDel[1]);
	graph->addEdge(mov.edgeToAdd[0], mov.edgeToAdd[1],
			! graph->getNullEdgeValue());
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
TabuSearch<T>::TabuSearch() {
	this->taskGraph = NULL;
}

template <class T>
TabuSearch<T>::~TabuSearch() {
	if (this->taskGraph != NULL)
		delete this->taskGraph;
}

template <class T>
void TabuSearch<T>::setDegreeLimits(size_t minDegree, size_t maxDegree) {
	if (maxDegree < minDegree)
		return;

	this->minDegree = minDegree;
	this->maxDegree = maxDegree;
}

template <class T>
void setTaskGraph(const GraphRepresentation<T>* taksGraph) {
	if (this->taskGraph != NULL)
		delete this->taskGraph;

	this->taskGraph = taskGraph->copy();
}

template <class T>
void setTabuListSize(size_t tabuListSize) {
	this->tabuList = TabuList<T>::TabuList(tabuListSize);
}

template <class T>
void setStopCriteria(size_t stopCriteria) {
	this->stopCriteria = stopCriteria;
}

template <class T>
void setFitnessLimit(T fitnessLimit) {
	this->fitnessLimit = fitnessLimit;
}

template <class T>
void setEpsilon(size_t epsilon) {
	this->epsilon = epsilon;
}

template <class T>
TabuAdjMatrix<T>* TabuSearch<T>::start() {

	//TODO: print/save seed

	//no loops are allowed and the matrices are symmetric.
	//Thus, there are only (nodes*2 - nodes)/2 unique edges.
	//This result can be obtained through arithmetic progression
	if (tabuListSize >= (tg->getNumNodes() * tg->getNumNodes() -
				tg->getNumNodes()) / 2)
		return NULL; //this would cause an infinite loop

	AdjacencyMatrix<bool>* currSol = generateInitSol(tg, epsilon);
	if (currSol == NULL)
		return NULL;
	T currFit = fitness(tg, currSol, valueLimit);
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
		std::cout << "Fit: " << currFit << '\n';
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
			neighboursFit.push_back(fitness(tg, currSol, valueLimit));
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
					neighboursMov.erase(neighboursMov.begin() +
							bestIndex);
					neighboursFit.erase(neighboursFit.begin() +
							bestIndex);
					continue;
				}
				break; //non tabu best neighbour found
			}

			//if all are tabu, generate non tabu neighbour
			if (neighboursMov.empty()) {
				currMov = getRandomNeighbour(currSol, epsilon,
						&tabuList, false);
				//computes neighbours' fitness
				makeMovement(currSol, currMov);
				currFit = fitness(tg, currSol, valueLimit);
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
	std::cout << "\n\nBEST SOLUTION\n";
	bestSol->print();
	std::cout << "Fitness: " << bestFit;
	std::cout << "\nTotal iterations: " << totalCount << std::endl;
	AdjacencyMatrix<T>* ret = NULL;
	delete bestSol;

	deallocateTabuList(&tabuList);

	//TODO: return solution set
	return ret;
}
