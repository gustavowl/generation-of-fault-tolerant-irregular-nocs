template <class T>
void TabuSearch<T>::fitToEpsilon(TabuAdjMatrix<bool>* initSol) {

	size_t frstIndex, scndIndex;
	while (epsilon < initSol->getNumEdges()) {
		//removes edges
		//computes positions of the two nodes with largest degrees
		frstIndex = initSol->getNodeWithNthDegree(0, true);	
		scndIndex = initSol->getNodeWithNthDegree(1, true);	

		initSol->delEdge( grEdge{
				.orig=frstIndex, .dest=scndIndex} );
	}

	while (epsilon > initSol->getNumEdges()) {
		//adds edges
		//computes positions of the two nodes with smallest degrees
		frstIndex = initSol->getNodeWithNthDegree(0, false);	
		scndIndex = initSol->getNodeWithNthDegree(1, false);	

		initSol->delEdge( grEdge{
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
	grEdge edge;
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
		//	2.2 - create empty tabuList of target nodes
		TabuList<bool> tabuList = TabuList(initSol->getNumNodes());
		edge.orig = smallest;

		while (true) {
			//	2.3 - identify second node with smallest degree
			//		(not in tabuList)
			size_t rankPos = 0;
			size_t smlNeighbour;
			do {
				rankPos++;
				smlNeighbour = initSol->getNodeWithNthDegree(rankPos, false);
				edge.dest = smlNeighbour;
			} while(tabuList.isTabu(edge));
			//	2.4 - add edge between these two nodes
			if (initSol->edgeExists(edge)) {
				//edge exists, add target node to tabuList
				tabuList.add(edge);
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
				tabuList.add(edge);
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
	if ((2*epsilon) / tg->getNumNodes() < minDegree ||
			(2*epsilon) / tg->getNumNodes() > maxDegree) {

		return NULL;
	}

	TabuAdjMatrix<bool>* initSol = new TabuAdjMatrix<bool>(
			tg->getNumNodes(), true, true, tg->getNullEdgeValue());
	//unable to generate graph
	if (initSol->getNumNodes() != tg->getNumNodes())
		return NULL;

	//copies task graph, converting representation
	GraphConverter::convert(tg, initSol);

	fitToEpsilon(initSol);
	makeFeasible(initSol);

	return initSol;
}

template <class T>
T TabuSearch<T>::fitness(const TabuAdjMatrix<bool>* sol) {
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
					return fitnessLimit;

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

	TabuAdjMatrix<bool>* currSol = generateInitSol();
	if (currSol == NULL)
		return NULL;
	T currFit = fitness(currSol);

	std::vector<NeighbourhoodSearch::Neighbour> neighbours;
	std::vector<T> neighboursFit;
	
	//stores tabu edges. Namely, the edge chosen
	//to be deleted will remain unaddable for some iterations
	std::vector<grEdge> tabuList;
	tabuList.reserve(tabuListSize);
	//TODO: assert that tabuList.size() == 0
	size_t tabuIndex = 0; //used to simulate circular queue

	//creates a copy
	//TODO: set num edges (create copy const?)
	TabuAdjMatrix<bool>* bestSol = currSol->copy(); 
	T bestFit = currFit;
	size_t count = 0;
	size_t totalCount = 0;
	size_t selectedIndex;
	NeighbourhoodSearch::Neighbour chosenNeighbour;
	
	while(count < stopCriteria) {
		selectedIndex = 0;
		currSol->print();
		std::cout << "Fit: " << currFit << '\n';
		std::cout << count << '/' << stopCriteria << std::endl;

		//searches neighbourhood
		//searches first epsilon random neighbours
		//this is feasible since epsilon's upper bound is
		//2 * numNodes (check generateInitSol())
		for (size_t i = 0; i < epsilon; i++) {
			//generates random neighbourhood movements
			neighbours.push_back(NeighbourhoodSearch::generateNeighbour(
						currSol, tabuList, true));
			//computes neighbours' fitness
			neighboursFit.push_back(fitness(tg, neighbours[i].solution,
					valueLimit));
		}

		//searches for aspiration criterea
		bool aspirationCrit = false;
		for (size_t i = 0; i < neighboursFit.size(); i++) {
			if (!aspirationCrit) {
				if (neighboursFit[i] < bestFit) {
					selectedIndex = i;
					aspirationCrit = true;
				}
				continue;
			}
			//there may be multiple aspiration criterea
			//the best one is chosen
			if (neighboursFit[i] < neighboursFit[selectedIndex]) {
				selectedIndex = i;
			}
		}
		//if aspiration criterea not found,
		//search for the best solution not in tabuList
		if (!aspirationCrit) {
			while (!neighboursMov.empty()) {
				selectedIndex = 0;
				for (size_t i = 1; i < neighbours.size(); i++) {
					if (neighboursFit[i] < neighboursFit[selectedIndex])
						selectedIndex = i;
				}

				if (neighbours[selectedIndex].wasTabuEdgeAdded) {
					//tabu solution, search for next best neighbour
					deallocateNeighbour(&neighbours[selectedIndex]);
					neighbours.erase(neighbours.begin() +
							selectedIndex);
					neighboursFit.erase(neighboursFit.begin() +
							selectedIndex);
					continue;
				}
				break; //non tabu best neighbour found
			}

			//if all are tabu, generate non tabu neighbour
			if (neighbours.empty()) {
				neighbours.push_back(NeighbourhoodSearch::generateNeighbour(
							currSol, tabuList, false));
				neighboursFit.push_back(fitness(tg, neighbours[i].solution,
					valueLimit));
				selectedIndex = 0;
			}
		}

		//changes to best neighbour solution found
		delete currSol;
		currSol = neighbours[selectedIndex].solution;
		currFit = neighboursFit[selectedIndex];
		tabuList.add(neighbours[selectedIndex].deletedEdge);

		//deallocates remaining solutions
		neighbours.erase(neighbours.begin() + selectedIndex);
		for (size_t i = 0; i < neighbours.size(); i++)
			deallocateNeighbour(&neighbours[i]);
		neighbours.clear();
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
	TabuAdjMatrix<T>* ret = NULL;
	delete bestSol;

	//TODO: return solution set
	return ret;
}
