template <class T>
void fitToEpsilon(AdjacencyMatrix<bool>* initSol, unsigned int epsilon) {
	//saves nodes degrees
	unsigned int size = initSol->getNumNodes();
	unsigned int degrees[initSol->getNumNodes()];
	for unsigned int i = 0; i < initSol->getNumNodes(); i++)
		degrees[i] = initSol->getNodeDegree(i);

	while (epsilon < initSol->getNumEdges()) {
		//removes edges
		//computes positions of the two nodes with largest degrees
		unsigned int largest = 0;
		unsigned int largest2 = 0;

		for (unsigned int i = 1; i < size; i++) {
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
		unsigned int smallest = 0;
		unsigned int smallest2 = 0;

		for (unsigned int i = 1; i < size; i++) {
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
void makeFeasible(AdjacencyMatrix<bool>* initSol) {
}

template <class T>
AdjacencyMatrix<bool>* TabuSearch<T>generateInitSol(
		const GraphRepresentation<T>* tg, unsigned int epsilon) {
	//checks wheter if epsilon is valid or not.
	//i.e. if it is possible to generate a graph such that
	//for all nodes, degree(node) in [2, 4]
	//REFER TO: degree sum formula / handshaking lemma
	if ((2*epsilon) / tg->getNumNodes() < 2 ||
			(2*epsilon) / tg->getNumNodes() > 4) {

		return NULL;
	}

	AdjacencyMatrix<bool>* ret = new AdjacencyMatrix(tg->getNumNodes(),
			true, true, tg->getNullEdgeValue());
	//unable to generate graph
	if (ret->getNumNodes != tg->getNumNodes())
		return NULL;

	//copies task graph, converting representation
	GraphConverter::convert(tg, &ret);

	//TODO: fitToEpsilon
	//TODO: make feasible

	return ret;
}

template <class T>
T TabuSearch<T>fitness(const GraphRepresentation<T>* tg
		const GraphRepresentation<T>* solution) {
	return tg->getEdgeValue(0, 0); 
}

template <class T>
AdjacencyMatrix<T>* TabuSearch<T>::start(
		const GraphRepresentation<T>* tg, unsigned int tabuListSize,
		unsigned int stopCriteria, unsigned int epsilon) {

	AdjacencyMatrix<bool>* currSol = generateValidStartGraph(tg, epsilon);
	if (currSol == NULL)
		return NULL;

	T currFit = fitness(tg, currSol);
	//creates a copy
	GraphRepresentation<T>* bestSol; //TODO: set num edges (create copy const?)
	GraphConverter::convert(currSol, bestSol);
	T bestFit = currFit;
	unsigned int count = 0;

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
			bestSol = currSol;
			bestFir = currFit;
			continue;
		}
		count++;
	}

	delete currSol;
	//TODO: create new matrix. Compute QAP for each edge, then return
	return bestSol;
}
