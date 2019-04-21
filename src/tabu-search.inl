template <class T>
void fitToEpsilon(AdjacencyMatrix<bool>* initSol, unsigned int epsilon) {
	while (epsilon < ret->getNumEdges()) {
		//removes edges
		unsigned int degrees[ret->getNumNodes()];
		for unsigned int i = 0; i < ret->getNumNodes(); i++)
			degrees[i] = ret->getNodeDegree(i);


	}
	while (epsilon > ret->getNumEdges()) {
		//adds edges
		
	}
	//TODO: while (epsilon > ret->getNumEdges()) addEdges()
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
