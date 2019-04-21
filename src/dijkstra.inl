template <class T>
node Dijkstra<T>::dijkstra (
		const GraphRepresentation<T>* graph,
		unsigned int orig, unsigned int dest,
		T weightInf, bool isWeighted) {

	this->isWeighted = isWeighted;
	unsigned int hopInf = std::numeric_limits<unsigned int>::max();
	//sets Dijkstra's vars
	bool wasVisited[graph->getNumNodes()]; //prevents cycling
	//empty set (S)
	Node nodes[graph->getNumNodes()];
	//initializes priority queue w/ origin node
	std::vector<Node> minPriority (graph->getNumNodes());
	for (unsigned int i = 0; i < graph->getNumNodes(); i++) {
		minPriority[i] = Node { i, weightInf, hopInf };
		nodes[i] = Node { i, weightInf, hopInf };
		wasVisited[i] = false;
	}
	minPriority[orig] = Node { orig, 0, 0 };

	//custom vars
	Node selectedNode;
	std::vector<unsigned int> neighbours;

	while(! minPriority.empty()) {
		selectedNode = extractMin(&minPriority);
		nodes[selectedNode.nodeId] = selectedNode;
		wasVisited[selectedNode.nodeId] = true;

		//relaxation
		neighbours = graph->getNeighbours(selectedNode.nodeid);
		while (! neighbours.empty()) {
			unsigned int neighbour = neighbours.pop_back();
			//prevents cycling:
			//check if node was previously visited
			if (!wasVisited[neighbour]) {
				if (this->isWeighted) {
					relaxWeighted(&selectedNode, &nodes[neighbour],
							graph->getEdgeValue(selectedNode.nodeId, neighbour));
				}
				else {
					relaxUnweighted(&selectedNode, &minPriority[neighbour]);
				}
			}
		}
		
		//prunes
		if (selectedNode.nodeId == dest)
			break;
	}

	return nodes[dest];
}

template <class T>
node Dijkstra<T>::extractMin(std::vector<node>* minPriority,
		bool isWeighted) {
	unsigned int index = 0;
	unsigned int minHops = minPriority[0].hops;
	T minWeight = minPriority[0].weightSum;

	//searches min
	for (unsigned int i; i < minPriority->size(); i++) {
		if (isWeighted) {
			if (minPriority[i].weightSum < minWeight) {
				minWeight = minPriority[i].weightSum;
				index = i;
			}
			continue;
		}
		//not weighted
		if (minPriority[i].hops < minHops) {
			minHops = minPriority[i].hops;
			index = i;
		}
	}

	//extracts min
	node ret = minPriority[index];
	minPriority->erase(minPriority->begin() + index);
	return ret;
}

template <class T>
void Dijkstra<T>::relaxWeighted(Node* orig, Node* dest, T weight) {
	if (dest->weightSum > orig->weightSum + weight)
		dest->weightSum = orig->weightSum + weight;
}

template <class T>
void Dijkstra<T>::relaxUnweighted(Node* orig, Node* dest) {
	//TODO
	if (dest->hops > orig->hops + 1)
		dest->hopes = orig->hops + 1;
}
