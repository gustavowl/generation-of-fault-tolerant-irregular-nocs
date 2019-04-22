template <class T>
typename Dijkstra<T>::Node Dijkstra<T>::dijkstra (
		GraphRepresentation<T>* graph,
		size_t orig, size_t dest,
		T weightInf, bool isWeighted) {

	//sets Dijkstra's vars
	bool wasVisited[graph->getNumNodes()]; //prevents cycling
	//empty set (S)
	Node nodes[graph->getNumNodes()];
	//initializes priority queue w/ origin node
	std::vector<Node> minPriority (graph->getNumNodes());
	for (size_t i = 0; i < graph->getNumNodes(); i++) {
		minPriority[i] = Node { i, weightInf, HOP_INF };
		nodes[i] = Node { i, weightInf, HOP_INF };
		wasVisited[i] = false;
	}
	minPriority[orig] = Node { orig, 0, 0 };

	//custom vars
	Node selectedNode;
	std::vector<size_t> neighbours;

	while(! minPriority.empty()) {
		selectedNode = extractMin(&minPriority, isWeighted);
		nodes[selectedNode.nodeId] = selectedNode;
		wasVisited[selectedNode.nodeId] = true;

		//relaxation
		neighbours = graph->getNeighbours(selectedNode.nodeId);
		while (! neighbours.empty()) {
			size_t neighbour = neighbours.back();
			neighbours.pop_back();
			//prevents cycling:
			//check if node was previously visited
			if (!wasVisited[neighbour]) {
				if (isWeighted) {
					relaxWeighted(&selectedNode, &nodes[neighbour],
							graph->getEdgeValue(selectedNode.nodeId, neighbour));
				}
				else {
					relaxUnweighted(&selectedNode, &nodes[neighbour]);
				}

				//updates minPriority queue
				for (size_t i = 0; i < minPriority.size(); i++) {
					if (minPriority[i].nodeId == neighbour) {
						minPriority[i] = nodes[neighbour];
						break;
					}
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
typename Dijkstra<T>::Node Dijkstra<T>::extractMin(std::vector<Node>* minPriority,
		bool isWeighted) {
	size_t index = 0;
	size_t minHops = minPriority->at(0).hops;
	T minWeight = minPriority->at(0).weightSum;

	//searches min
	for (size_t i = 1; i < minPriority->size(); i++) {
		if (isWeighted) {
			if (minPriority->at(i).weightSum < minWeight) {
				minWeight = minPriority->at(i).weightSum;
				index = i;
			}
			continue;
		}
		//not weighted
		if (minPriority->at(i).hops < minHops) {
			minHops = minPriority->at(i).hops;
			index = i;
		}
	}

	//extracts min
	Node ret = minPriority->at(index);
	minPriority->erase(minPriority->begin() + index);
	return ret;
}

template <class T>
void Dijkstra<T>::relaxWeighted(Node* orig, Node* dest, T weight) {
	if (dest->weightSum > orig->weightSum + weight) {
		dest->weightSum = orig->weightSum + weight;
		dest->hops = orig->hops + 1;
	}
}

template <class T>
void Dijkstra<T>::relaxUnweighted(Node* orig, Node* dest) {
	if (dest->hops > orig->hops + 1)
		dest->hops = orig->hops + 1;
}
