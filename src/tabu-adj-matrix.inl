template <class T>
void TabuAdjMatrix<T>::nodeIdSwap(grEdge* edge) const {
	if (edge->orig < edge->dest) {
		size_t change = edge->orig;
		edge->orig = edge->dest;
		edge->dest = change;
	}
}

template <class T>
void TabuAdjMatrix<T>::setInvalid() {
	//Set graph as zero-order/invalid.
	if (this->adjm != NULL) {
		//then there are some arrays allocated
		for (size_t i = 0; i < this->numNodes; i++) {

			if (this->adjm[i] != NULL)
				delete[] this->adjm[i];

		}
		delete[] this-> adjm;
	}

	if (this->degrees != NULL)
		delete[] this->degrees;

	this->numNodes = 0;
	this->numEdges = 0;
}

template <class T>
bool TabuAdjMatrix<T>::isEdgeInvalid(grEdge edge,
		bool checkValue)  const {

	bool ret = edge.orig == edge.dest ||
			edge.orig >= this->numNodes ||
			edge.dest >= this->numNodes;

	if (checkValue)
		return ret || edge.value == this->nullEdgeValue;

	return ret;
}

template <class T>
grEdge TabuAdjMatrix<T>::generateInvalidEdge() {
	return grEdge {
		.orig = this->numNodes,
		.dest = this->numNodes,
		.value = this->nullEdgeValue
	};
}

template <class T>
size_t TabuAdjMatrix<T>::maxNumEdges() {
	//since the matrix is triangular and has no
	//main diagonal, it has (n^2 - n)/2 possible
	//edges. This formula can be easily obtained
	//via arithmetic progression.
	return (this->numNodes*this->numNodes - this->numNodes) / 2;
}

//TODO: verify if space to  be allocated is free.
//Otherwise, process may be killed.
template <class T>
TabuAdjMatrix<T>::TabuAdjMatrix(size_t numNodes, T nullEdgeValue) {

	if (numNodes <= 1)
		return;

	this->adjm = new (std::nothrow) T* [numNodes];
	if (this->adjm == NULL) { //unable to allocate mem space
		setInvalid();
		return;
	}
	//removes garbage content
	for (size_t i = 0; i < numNodes; i++)
		this->adjm[i] = NULL;

	this->degrees = new (std::nothrow) size_t [numNodes];
	if (this->degrees == NULL) {
		setInvalid();
		return;
	}
	//removes garbage content
	for (size_t i = 0; i < numNodes; i++)
		this->degrees[i] = 0;

	this->numNodes = numNodes;
	this->nullEdgeValue = nullEdgeValue;

	//resizes adjm columns to triangular matrix w/ no
	//main diagonal
	for (size_t i = 1; i < numNodes; i++) {
		//allocates space
		this->adjm[i] = new (std::nothrow) T [i];

		if (this->adjm[i] == NULL) {
			//failed to allocate memory. Reset
			setInvalid();
			return;
		}
		//sets all values to nullEdgeValue
		for (size_t j = 0; j < i; j++)
			this->adjm[i][j] = this->nullEdgeValue;
	}
}

template <class T>
TabuAdjMatrix<T>::~TabuAdjMatrix() {
	setInvalid();
}

template <class T>
void TabuAdjMatrix<T>::addEdge(grEdge edge) {
	//check if arguments are valid
	if (isEdgeInvalid(edge))
		return;

	nodeIdSwap(&edge);

	if (this->edgeExists(edge)) {
		return;
	}

	adjm[edge.orig][edge.dest] = edge.value;

	degrees[edge.orig]++;
	degrees[edge.dest]++;

	this->numEdges++;
}

template <class T>
void TabuAdjMatrix<T>::delEdge(grEdge edge){
	//check if arguments are valid
	if (isEdgeInvalid(edge, false))
		return;

	nodeIdSwap(&edge);

	if (!this->edgeExists(edge)) {
		return;
	}

	adjm[edge.orig][edge.dest] = this->nullEdgeValue;

	degrees[edge.orig]--;
	degrees[edge.dest]--;

	this->numEdges--;
}

template <class T>
bool TabuAdjMatrix<T>::edgeExists(grEdge edge) const {

	if (isEdgeInvalid(edge, false))
		return false;

	nodeIdSwap(&edge);

	return adjm[edge.orig][edge.dest] != this->nullEdgeValue;
}

template <class T>
T TabuAdjMatrix<T>::getEdgeValue(grEdge edge) const {

	if (isEdgeInvalid(edge, false))
		return this->nullEdgeValue;

	nodeIdSwap(&edge);

	return adjm[edge.orig][edge.dest];
}

template <class T>
size_t TabuAdjMatrix<T>::getNodeDegree(size_t node) const {
	if (node >= this->numNodes)
		return 0;
	return degrees[node];
}

template <class T>
TabuAdjMatrix<T>* TabuAdjMatrix<T>::copy() const {
	//calls constructor to reserve mem space and initial instantiation
	TabuAdjMatrix<T>* ret = new TabuAdjMatrix(this->numNodes,
			this->nullEdgeValue); //TODO (std::nothrow) and check

	//checks if matrix if valid
	if (ret->numNodes == 0)
		return NULL;

	grEdge edge;
	//copies edges
	for (size_t i = 1; i < ret->numNodes; i++) {
		edge.orig = i;

		for(size_t j = 0; j < i; j++) {
			edge.dest = j;

			if (this->edgeExists(edge)) {
				edge.value = this->getEdgeValue(edge);
				ret->addEdge(edge);
			}
		}
	}

	return ret;
}

template <class T>
std::string TabuAdjMatrix<T>::toStr() const {
	//initializes empty strings
	std::string ret = "";

	//populates content
	//head
	ret += this->numNodes;
	ret += ',' + this->numEdges;
	//populates edges
	grEdge edge;
	for (size_t i = 0; i < this->numNodes - 1; i++) {
		for (size_t j = i + 1; j < this->numNodes; j++) {
			edge.orig = j;
			edge.dest = i;
			if (edgeExists(edge)) {
				ret += '\n' + edge.dest + ',' + edge.orig + ',' +
					getEdgeValue(edge);
			}
		}
	}

	return ret;
}

template <class T>
bool TabuAdjMatrix<T>::areEdgesEqual(grEdge edge1, grEdge edge2) {
	nodeIdSwap(&edge1);
	nodeIdSwap(&edge2);

	return edge1.equalsTo(edge2);
}

template <class T>
grEdge TabuAdjMatrix<T>::selectRandomEdge(TabuList<T>* tabuList,
		bool existent) {
	
	grEdge e;

	if ( tabuList == NULL ) {
		return this->generateInvalidEdge();
	}

	//count the possible edges to be chosen
	size_t inLimits = 0;
	for (e.orig = 1; e.orig < this->numNodes; e.orig++) {
		for (e.dest = 0; e.dest < e.orig; e.dest++) {
			if ( ((existent && this->edgeExists(e)) ||
					(!existent && !this->edgeExists(e))) &&
					!tabuList->isTabu(e) ) {
				inLimits++;
			}

		}
	}	

	if (inLimits == 0)
		return this->generateInvalidEdge();
	size_t randomVal = rng() % inLimits;

	size_t count = 0;
	for (e.orig = 1; e.orig < this->numNodes; e.orig++) {

		for (e.dest = 0; e.dest < e.orig; e.dest++) {

			if (tabuList->isTabu(e))
				continue;

			if ( (existent && this->edgeExists(e)) ||
					(!existent && !this->edgeExists(e))) {

				if (count == randomVal) {
					e.value = this->getEdgeValue(e);
					return e;
				}

				count++;
			}
		}
	}

	return this->generateInvalidEdge();
}

template <class T>
grEdge TabuAdjMatrix<T>::selectRandomEdge(size_t incidentNode,
		TabuList<T>* tabuList, bool existent) {
	size_t maxDegree = degrees[0];
	for (size_t i = 1; i < this->numNodes; i++) {
		if (degrees[i] > maxDegree)
			maxDegree = degrees[i];
	}
	
	return this->selectRandomEdge(incidentNode, maxDegree + 1,
			tabuList, existent);
}

template <class T>
grEdge TabuAdjMatrix<T>::selectRandomEdge(size_t incidentNode,
		size_t upperDestDeg, TabuList<T>* tabuList, bool existent) {
	grEdge e;
	//counts nodes with degree < upperDestDeg
	size_t inLimits = 0;

	if (incidentNode >= this->numNodes || tabuList == NULL)
		return this->generateInvalidEdge();

	for (size_t i = 0; i < this->numNodes; i++) {

		if (i == incidentNode || degrees[i] >= upperDestDeg)
			continue;
		e.orig = incidentNode;
		e.dest = i;
		this->nodeIdSwap(&e);

		if ( ((existent && this->edgeExists(e)) ||
				(!existent && !this->edgeExists(e))) &&
				!tabuList->isTabu(e) ) {

			inLimits++;
		}
	}

	if (inLimits == 0)
		return this->generateInvalidEdge();

	size_t randomVal = rng() % inLimits;
	size_t count = 0;

	for (size_t i = 0; i < this->numNodes; i++) {

		if (i == incidentNode || degrees[i] >= upperDestDeg)
			continue;

		e.orig = incidentNode;
		e.dest = i;
		this->nodeIdSwap(&e);

		if (tabuList->isTabu(e))
			continue;

		if ( (existent && this->edgeExists(e)) ||
				(!existent && !this->edgeExists(e)) ) {

			if (count == randomVal) {
				e.value = this->getEdgeValue(e);
				return e;
			}

			count++;
		}
	}

	return this->generateInvalidEdge();
}

template <class T>
void TabuAdjMatrix<T>::swapEdgesNodes(grEdge* edge1, grEdge* edge2,
		TabuList<T>* tabuList) {
	//there are two possible swap for edges (1, 2), (3, 4)
	//	(1, 4), (3, 2) or (1, 3), (2, 4)
	
	grEdge swapEdge1 = this->generateInvalidEdge();
	grEdge swapEdge2 = swapEdge1;

	int scenario = rng() % 2;
	int possibleScenariosCount = 0;
	size_t swapNode;

	if (!edgeExists(*edge1) || !edgeExists(*edge2) || tabuList == NULL) {
		possibleScenariosCount = 3;
	}


	for (; possibleScenariosCount < 2; possibleScenariosCount++) {

		swapEdge1 = *edge1;
		swapEdge2 = *edge2;
		
		if (scenario == 0) {
			//(1, 2), (3, 4) -> (1, 4), (3, 2)
			swapNode = swapEdge1.dest;
			swapEdge1.dest = swapEdge2.dest;
			swapEdge2.dest = swapNode;
		}
		else {
			//(1, 2), (3, 4) -> (1, 3), (2, 4)
			swapNode = swapEdge1.dest;
			swapEdge1.dest = swapEdge2.orig;
			swapEdge2.orig = swapNode;
		}

		scenario = (scenario + 1) % 2;

		//check if swap is possible
		if (isEdgeInvalid(swapEdge1, false) ||
				isEdgeInvalid(swapEdge2, false)) {
			//this condition is necessary because swapping
			//edges (0, 1), and (0, 2) may generate edge
			//(0, 0), which is invalid and would not be
			//detected by edgeExists()
			continue;
		}
		if (edgeExists(swapEdge1) || edgeExists(swapEdge2)) {
			//one of the swapped edges already exists.
			//Thus, the graph cannot be changed
			continue;
		}

		//valid swap. Check if edges are in tabuList.
		//Otherwise, remove edges, choose weights randomly,
		//and add new swapped edges.
		this->nodeIdSwap(&swapEdge1); //standard representation
		this->nodeIdSwap(&swapEdge2); //standard representation
		if (tabuList->isTabu(swapEdge1) || tabuList->isTabu(swapEdge2))
			continue;

		if (rng() % 2 == 0) {
			swapEdge1.value = getEdgeValue(*edge1); 
			swapEdge2.value = getEdgeValue(*edge2); 
		}
		else {
			swapEdge1.value = getEdgeValue(*edge2); 
			swapEdge2.value = getEdgeValue(*edge1);
		}

		this->delEdge(*edge1);
		this->delEdge(*edge2);

		this->addEdge(swapEdge1);
		this->addEdge(swapEdge2);

		*edge1 = swapEdge1;
		*edge2 = swapEdge2;
		return;
	}
	
	//set edges invalid
	*edge1 = this->generateInvalidEdge();
	*edge2 = *edge1;
}

template <class T>
grEdge TabuAdjMatrix<T>::spinEdge(grEdge edge, size_t fixedNode) {

	if (!edgeExists(edge) || ( edge.orig != fixedNode &&
				edge.dest != fixedNode) ) {
		return this->generateInvalidEdge();
	}

	grEdge spinned = selectRandomEdge(fixedNode, false);

	if (this->isEdgeInvalid(spinned, false)) {
		//SHOULD BE NEVER REACHED
		return this->generateInvalidEdge();
	}

	spinned.value = edge.value;
	this->delEdge(edge);
	this->addEdge(spinned);

	return spinned;
}

template <class T>
grEdge TabuAdjMatrix<T>::spinEdge(grEdge edge, size_t fixedNode,
		size_t upperDestDeg, TabuList<T>* tabuList) {

	if (!edgeExists(edge) || ( edge.orig != fixedNode &&
				edge.dest != fixedNode) || tabuList == NULL) {
		return this->generateInvalidEdge();
	}

	grEdge spinned = selectRandomEdge(fixedNode, upperDestDeg,
			tabuList, false);

	if (this->isEdgeInvalid(spinned, false))
		return this->generateInvalidEdge();

	spinned.value = edge.value;
	this->delEdge(edge);
	this->addEdge(spinned);

	return spinned;
}

template <class T>
grEdge* TabuAdjMatrix<T>::doubleSpinEdge(grEdge deltdEdge, grEdge targets,
		size_t upperDestDeg, TabuList<T>* tabuList) {

	if (targets.orig >= this->numNodes || targets.dest >= this->numNodes)
		return NULL;

	TabuList<T> tabuOrigEdges;
	TabuList<T> tabuSpinnedOrig;
	for (size_t i = 0; i < tabuList->size(); i++)
		tabuSpinnedOrig.add(tabuList->at(i));
	tabuSpinnedOrig.add(deltdEdge);

	while (true) {
		grEdge origEdge = this->selectRandomEdge(targets.orig,
				&tabuOrigEdges, true);

		if (this->isEdgeInvalid(origEdge))
			break;

		//first edge to spin selected. Spin it
		size_t fixedNode = (origEdge.orig == targets.orig) ?
			origEdge.dest : origEdge.orig;
		grEdge spinnedOrig = this->spinEdge(origEdge, fixedNode,
			upperDestDeg, &tabuSpinnedOrig);
		if (this->isEdgeInvalid(spinnedOrig)) {
			tabuOrigEdges.add(origEdge);
			continue;
		}

		//select second edge to spin
		TabuList<T> tabuDestEdges;

		TabuList<T> tabuSpinnedDest;
		for (size_t i = 0; i < tabuList->size(); i++)
			tabuSpinnedDest.add(tabuList->at(i));
		tabuSpinnedDest.add(deltdEdge);
		tabuSpinnedDest.add(origEdge);

		while(true) {
			grEdge destEdge = this->selectRandomEdge(targets.dest,
					&tabuDestEdges, true);

			if (this->isEdgeInvalid(destEdge))
				break;

			fixedNode = (destEdge.dest == targets.dest) ?
				destEdge.orig : destEdge.dest;
			grEdge spinnedDest = this->spinEdge(destEdge, fixedNode,
				upperDestDeg, &tabuSpinnedDest);
			if (this->isEdgeInvalid(spinnedDest)) {
				tabuDestEdges.add(destEdge);
				continue;
			}

			//verify if movement is tabu
			std::vector<grEdge> mv;
			mv.push_back(spinnedOrig);
			mv.push_back(spinnedDest);
			if (tabuList->isTabu(mv)) {
				//undoes movement
				this->delEdge(spinnedDest);
				this->addEdge(destEdge);
				tabuSpinnedDest.add(spinnedDest);
				continue;
			}

			//second edge successfully spinned
			//allocate space to return deleted and added edges
			grEdge* ret = new grEdge[4];
			ret[0] = origEdge;
			ret[1] = destEdge;
			ret[2] = spinnedOrig;
			ret[3] = spinnedDest;
			return ret;
		}

		//failed. Undo
		this->delEdge(spinnedOrig);
		this->addEdge(origEdge);
		tabuSpinnedOrig.add(spinnedOrig);
		tabuOrigEdges.add(origEdge);
	}

	//no possible spin found.return false;
	return NULL;
}

template <class T>
size_t TabuAdjMatrix<T>::getNodeWithNthDegreeFromList(
		std::vector<size_t> nodes, size_t rankPos, bool largest) {
	if (rankPos >= nodes.size())
		return this->numNodes;

	size_t node; //selected node
	size_t degree;

	rankPos++;

	for (; rankPos > 0; rankPos--) {
		size_t index = 0; //index of selected node in nodes
		node = nodes[index];
		degree = degrees[node];

		for (size_t i = 1; i < nodes.size(); i++) {
			if ( (largest && degrees[nodes[i]] > degree) ||
				   (!largest && degrees[nodes[i]] < degree) ) {
				index = i;
				node = nodes[index];
				degree = degrees[node];
			}	
		}
		//removes node to search for other positions
		nodes.erase(nodes.begin() + index);
	}
	return node;
}

template <class T>
size_t TabuAdjMatrix<T>::getNodeWithNthDegree(size_t rankPos,
		bool largest) {

	if (rankPos >= this->numNodes)
		return this->numNodes;


	std::vector<size_t> nodes;
	nodes.reserve(this->numNodes);
	for (size_t i = 0; i < this->numNodes; i++)
		nodes.push_back(i);

	return this->getNodeWithNthDegreeFromList(nodes, rankPos, largest);
}

template <class T>
size_t TabuAdjMatrix<T>::getNeighbourWithNthDegree(size_t rankPos,
		size_t incidentNode, bool largest) {
	if (incidentNode >= this->numNodes)
		return this->numNodes;
	
	return this->getNodeWithNthDegreeFromList(
			this->getNeighbours(incidentNode), rankPos, largest);
}

template <class T>
size_t TabuAdjMatrix<T>::toInt64() {
	size_t maxEdges = this->maxNumEdges();
	if (maxEdges > 64)
		return 0;

	size_t ret = 0;
	grEdge edge;
	for (edge.orig = 1; edge.orig < this->numNodes; edge.orig++) {
		for (edge.dest = 0; edge.dest < edge.orig; edge.dest++) {
			ret = ret * 2 + this->edgeExists(edge);
		}
	}
	return ret;
}
