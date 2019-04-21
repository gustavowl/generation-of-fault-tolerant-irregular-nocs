template <class T>
T TabuSearch<T>fitness(const GraphRepresentation<T>* tg
		const GraphRepresentation<T>* solution) {
	return tg->getEdgeValue(0, 0); 
}

template <class T>
static GraphRepresentation<T>* TabuSearch<T>generateValidStartGraph(
		const GraphRepresentation<T>* tg, unsigned int epsilon) {
	return tg;
}

template <class T>
GraphRepresentation<T>* TabuSearch<T>::start(
		const GraphRepresentation<T>* tg, unsigned int tabuListSize,
		unsigned int stopCriteria, unsigned int epsilon) {

	GraphRepresentation<T>* currSol = generateValidStartGraph(tg, episolon);
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
	return bestSol;
}
