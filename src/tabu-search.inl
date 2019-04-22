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
	return tg->getEdgeValue(0, 0); 
}

template <class T>
AdjacencyMatrix<T>* TabuSearch<T>::start(
		const GraphRepresentation<T>* tg, size_t tabuListSize,
		size_t stopCriteria, size_t epsilon) {

	AdjacencyMatrix<bool>* currSol = generateInitSol(tg, epsilon);
	if (currSol == NULL)
		return NULL;

	T currFit = fitness(tg, currSol);
	//creates a copy
	//TODO: set num edges (create copy const?)
	GraphRepresentation<bool>* bestSol = currSol->copy(); 
	T bestFit = currFit;
	size_t count = 0;

	while(count < stopCriteria) {

		//while (not tabu) {
		//	random movement() //TODO: decide movements: swap?
		//	compute fitness
		//	if isBetter() //TODO: verify this critera. Compute fitness is expensive
		//		break;
		//	else if isTabu()
		//		traceback;
		//	else
		//		break;
		//}
		//add movement to tabu list()
		//TODO: when to compute solution feasibleness?
		//	too early may make TS not to convert into optimal
		//	too late may make TS may found optimal, but the
		//	closest feasible solutio*=ns are worse than others
		//	previously found

		if (currFit < bestFit) {
			count = 0;
			delete bestSol;
			bestSol = currSol->copy();
			bestFit = currFit;
			continue;
		}
		count++;
	}

	delete currSol;
	//TODO: create new matrix from bestSol.
	//	Compute QAP for each edge, then return
	AdjacencyMatrix<T>* ret = NULL;
	return ret;
}
