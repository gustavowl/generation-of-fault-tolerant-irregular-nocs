#ifndef __TABU_SEARCH__
#define __TABU_SEARCH__

#include "graph-representation.h"
#include "graph-converter.h"
#include "dijkstra.h"

template <class T>
class TabuSearch {
private:
	//TODO: change name?
	//Computes QAP
	static T fitness(const GraphRepresentation<T>* tg);

	//removes edges until |E| = epsilon
	static GraphRepresentation<T>* generateValidStartGraph(
			const GraphRepresentation<T>* tg, unsigned int epsilon);

public:
	// stopCriteria in number of iterations with no improvement
	static GraphRepresentation<T>* start(const GraphRepresentation<T>* tg,
			unsigned int tabuListSize, unsigned int stopCriteria,
			unsigned int epsilon);
};

#include "../tabu-search.inl"

#endif
