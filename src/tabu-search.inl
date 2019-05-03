template <class T>
void TabuSearch<T>::fitToEpsilon(TabuAdjMatrix<bool>* initSol) {

	size_t frstIndex, scndIndex;
	while (epsilon < initSol->getNumEdges()) {
		//removes edges
		//computes positions of the two nodes with largest degrees
		frstIndex = initSol->getNodeWithNthDegree(0, true);	
		scndIndex = initSol->getNeighbourWithNthDegree(0,
				frstIndex, true);	

		initSol->delEdge( boolEdge{
				.orig=frstIndex, .dest=scndIndex} );
	}

	while (epsilon > initSol->getNumEdges()) {
		//adds edges
		//computes positions of the two nodes with smallest degrees
		frstIndex = initSol->getNodeWithNthDegree(0, false);	
		scndIndex = initSol->getNodeWithNthDegree(1, false);	

		initSol->addEdge( boolEdge{
				.orig=frstIndex, .dest=scndIndex,
				.value = !initSol->getNullEdgeValue()} );
	}
}

template <class T>
bool TabuSearch<T>::isFeasible(TabuAdjMatrix<bool>* sol) {
	size_t size = sol->getNumNodes();
	size_t degree = 0;
	for (size_t i = 0; i < size; i++) {
		degree = sol->getNodeDegree(i);
		if (degree < minDegree || degree > maxDegree)
			return false;
	}
	return true;
}

template <class T>
void TabuSearch<T>::makeFeasible(TabuAdjMatrix<bool>* initSol) {
	//sets initial variables
	boolEdge edge;
	edge.value = initSol->getNullEdgeValue();

	//while (not feasible)
	while (!isFeasible(initSol)) {
		//1 - remove edge
		//	1.1 - identify node with largest degree
		size_t largest = initSol->getNodeWithNthDegree(0, true);
		//	1.2 - identify its neighbour with largest degree
		size_t lrgNeighbour = initSol->getNeighbourWithNthDegree(
				0, largest, true);
		//1.3 - remove edge
		edge.orig = largest;
		edge.dest = lrgNeighbour;
		initSol->delEdge(edge);
		
		//2 - add edge
		//	2.1 - identify node with smallest degree
		size_t smallest = initSol->getNodeWithNthDegree(0, false);
		//	2.2 - create empty tl of target nodes
		TabuList<bool> tl = TabuList<bool>(initSol->getNumNodes());
		edge.orig = smallest;

		while (true) {
			//	2.3 - identify second node with smallest degree
			//		(not in tl)
			size_t rankPos = 0;
			size_t smlNeighbour;
			do {
				rankPos++;
				smlNeighbour = initSol->getNodeWithNthDegree(rankPos,
						false);
				edge.dest = smlNeighbour;
			} while(tl.isTabu(edge));
			//	2.4 - add edge between these two nodes
			if (initSol->edgeExists(edge)) {
				//edge exists, add target node to tl
				tl.add(edge);
				continue;
			}
			edge.value = !edge.value;
			initSol->addEdge(edge);
			edge.value = !edge.value;

			//3 - check if graph is disconnected
			//	3.1 - Dijkstra
			size_t numHops = Dijkstra<bool>::dijkstra(
					initSol, largest, lrgNeighbour, true,
					false).hops;
			//	3.2 - if disconnected
			if (numHops == HOP_INF) {
				//	3.2.1 - remove edge
				initSol->delEdge(edge);
				//	3.2.2 - add target node to a TabuList
				tl.add(edge);
				//	3.2.3 - go back to step 2.3
				continue;
			}

			break;
		}
	}
}

template <class T>
TabuAdjMatrix<bool>* TabuSearch<T>::generateInitSol() {
	//checks wheter if epsilon is valid or not.
	//i.e. if it is possible to generate a graph such that
	//for all nodes, degree(node) in [minDegree, maxDegree]
	//REFER TO: degree sum formula / handshaking lemma
	if ((2*epsilon) / taskGraph->getNumNodes() < minDegree ||
			(2*epsilon) / taskGraph->getNumNodes() > maxDegree) {

		return NULL;
	}

	TabuAdjMatrix<bool>* initSol = new TabuAdjMatrix<bool>(
			taskGraph->getNumNodes(), taskGraph->getNullEdgeValue());
	//unable to generate graph
	if (initSol->getNumNodes() != taskGraph->getNumNodes())
		return NULL;

	//copies task graph, converting representation
	GraphConverter::convert(taskGraph, initSol);

	fitToEpsilon(initSol);
	makeFeasible(initSol);

	return initSol;
}

template <class T>
T TabuSearch<T>::fitness(const TabuAdjMatrix<bool>* sol) {
	//computes QAP function:
	//for all edge(i, j) in taskGraph
	//\sum min_hops(node_i, node_j) * weightOf(edge(i, j))
	T sum;
	bool firstFound = false;

	grEdge edge;
	//Computes QAP for each edge in the task graph
	for (edge.orig = 0; edge.orig < taskGraph->getNumNodes();
			edge.orig++) {
		for (edge.dest = 0; edge.dest < taskGraph->getNumNodes();
				edge.dest++) {
			if (taskGraph->edgeExists(edge)) {

				size_t numHops = Dijkstra<bool>::dijkstra(
						sol, edge.orig, edge.dest, true, false).hops;

				if (numHops == HOP_INF)
					return fitnessLimit;

				T qap = numHops * taskGraph->getEdgeValue(edge);

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
TabuSearch<T>::TabuSearch() {
	this->taskGraph = NULL;
	this->tabuList = NULL;
}

template <class T>
TabuSearch<T>::~TabuSearch() {
	if (this->taskGraph != NULL)
		delete this->taskGraph;
	if (this->tabuList != NULL)
		delete this->tabuList;
}

template <class T>
void TabuSearch<T>::setDegreeLimits(size_t minDegree, size_t maxDegree) {
	if (maxDegree < minDegree)
		return;

	this->minDegree = minDegree;
	this->maxDegree = maxDegree;
	neighSearch.setDegreeLimits(minDegree, maxDegree);
}

template <class T>
void TabuSearch<T>::setTaskGraph(const GraphRepresentation<T>* tg) {
	if (this->taskGraph != NULL)
		delete this->taskGraph;

	this->taskGraph = tg->copy();
}

template <class T>
void TabuSearch<T>::setTabuListSize(size_t tabuListSize) {
	if (this->tabuList != NULL)
		delete tabuList;

	this->tabuList = new TabuList<bool>(tabuListSize, true);
}

template <class T>
void TabuSearch<T>::setStopCriteria(size_t stopCriteria) {
	this->stopCriteria = stopCriteria;
}

template <class T>
void TabuSearch<T>::setFitnessLimit(T fitnessLimit) {
	this->fitnessLimit = fitnessLimit;
}

template <class T>
void TabuSearch<T>::setEpsilon(size_t epsilon) {
	this->epsilon = epsilon;
}

template <class T>
size_t TabuSearch<T>::selectBestNeighbour(std::vector<T>* neighboursFit) {

	size_t size = neighboursFit->size();
	size_t selectedIndex = 0;

	for (size_t i = 1; i < size; i++) {
		if (neighboursFit->at(i) < neighboursFit->at(selectedIndex))
			selectedIndex = i;
	}

	return selectedIndex;
}

template <class T>
size_t TabuSearch<T>::searchAspirationCriteria(
		std::vector<T>* neighboursFit, T bestFit) {

	size_t selectedIndex = selectBestNeighbour(neighboursFit);

	if (neighboursFit->at(selectedIndex) < bestFit)
		return selectedIndex;
	return neighboursFit->size();
}

template <class T>
void TabuSearch<T>::removeTabuNeighbours(
		std::vector<NeighbourhoodSearch::Neighbour>* neighbours,
		std::vector<T>* neighboursFit) {

	for (size_t i = neighbours->size(); i > 0; i--) {
		if (neighbours->at(i - 1).isTabu) {
			neighSearch.deallocateNeighbour(
					&(neighbours->at(i - 1)));
			neighbours->erase(neighbours->begin() + i - 1);
			neighboursFit->erase(neighboursFit->begin() + i - 1);
		}
	}
}

template <class T>
TabuAdjMatrix<bool>* TabuSearch<T>::start() {

	//TODO: print/save seed

	TabuAdjMatrix<bool>* currSol = generateInitSol();
	if (currSol == NULL)
		return NULL;
	T currFit = fitness(currSol);

	std::vector<NeighbourhoodSearch::Neighbour> neighbours;
	std::vector<T> neighboursFit;

	//creates a copy
	//TODO: set num edges (create copy const?)
	TabuAdjMatrix<bool>* bestSol = currSol->copy(); 
	T bestFit = currFit;
	size_t count = 0;
	size_t totalCount = 0;
	size_t selectedIndex;
	bool aspirationCrit;
	
	while(count < stopCriteria) {
		selectedIndex = 0;
		currSol->print();
		//std::cout << currSol->toInt64() << std::endl;
		std::cout << "Aspiration criteria: " << aspirationCrit << '\n';
		std::cout << "Fit: " << currFit << '\n';
		std::cout << count << '/' << stopCriteria << std::endl;

		//searches neighbourhood
		//searches first epsilon random neighbours
		//this is feasible since epsilon's upper bound is
		//2 * numNodes (check generateInitSol())
		for (size_t i = 0; i < epsilon; i++) {
			//generates random neighbourhood movements
			neighbours.push_back(neighSearch.generateNeighbour(
						currSol, tabuList, true));
			assert(neighbours[i].sol != NULL);
			//computes neighbours' fitness
			neighboursFit.push_back(fitness(neighbours[i].sol));
		}

		//searches for aspiration criterea
		selectedIndex = searchAspirationCriteria(&neighboursFit, bestFit);
		aspirationCrit = selectedIndex != neighboursFit.size();
		//if aspiration criterea not found,
		//search for the best solution not in tabuList
		if (!aspirationCrit) {
			removeTabuNeighbours(&neighbours, &neighboursFit);

			if (!neighbours.empty())
				selectedIndex = selectBestNeighbour(&neighboursFit);
			else {
				//if all are tabu, generate non tabu neighbour
				//TODO generate multiple non-tabu neighbours
				//and choose best one
				neighbours.push_back(
						neighSearch.generateNeighbour(currSol,
							tabuList, false));
				if (neighbours[0].sol == NULL) {
					currSol->print();
					std::cout << "Unable to generate non-tabu neighbour" <<
						"\nReturning best solution found..." << std::endl;
					break;
				}
				neighboursFit.push_back(fitness(neighbours[0].sol));
				selectedIndex = 0;
				assert(neighbours[0].sol != NULL);
				assert(!neighbours[0].isTabu);
			}
		}

		//changes to best neighbour solution found
		delete currSol;
		currSol = neighbours[selectedIndex].sol;
		currFit = neighboursFit[selectedIndex];
		tabuList->add(neighbours[selectedIndex].deltdEdges);

		assert(currSol->getNumEdges() == epsilon);
		if (!aspirationCrit)
			assert(!neighbours[selectedIndex].isTabu);

		//deallocates remaining solutions
		neighbours.erase(neighbours.begin() + selectedIndex);
		for (size_t i = 0; i < neighbours.size(); i++)
			neighSearch.deallocateNeighbour(&neighbours[i]);
		neighbours.clear();
		neighboursFit.clear();

		totalCount++;
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
	std::cout << "\n\nBEST SOLUTION\n";
	bestSol->print();
	std::cout << "Fitness: " << bestFit;
	std::cout << "\nTotal iterations: " << totalCount << std::endl;

	//TODO: return solution set
	return bestSol;
}

template <class T>
void TabuSearch<T>::assert(bool value) {
	if (value)
		return;
	//print status
	std::cout << "**************************************" <<
		"\n*********ASSERT EXCEPTION*************\n" <<
		"**************************************\n";
	std::cout << "TABU LIST\n";
	std::vector<boolEdge> tabuItem;
	for (size_t i = 0; i < tabuList->size(); i++) {
		tabuItem = tabuList->at(i);
		std::cout << '{';
		size_t j = 0;
		for (; j < tabuItem.size() - 1; j++) {
			std::cout << tabuItem[j].orig << " -> " << tabuItem[j].dest <<
				" = " << tabuItem[j].value << ", ";
		}
		std::cout << tabuItem[j].orig << " -> " << tabuItem[j].dest <<
			" = " << tabuItem[j].value << ", ";
		std::cout << "}\n";
	}
	std::cout << "**************************************" <<
		"\n*********ASSERT EXCEPTION*************\n" <<
		"**************************************" << std::endl;
}
