#ifndef __STATISTICS_H__
#define __STATISTICS_H__

template <class T>
class Statistics {
private:
	T minWeight;
	T maxWeight;
	double meanWeight;
	double stdDev;
	T fitness; //alias to weightSum

public:
	T getMinWeight() const;
	T getMaxWeight() const;
	double getMeanWeight() const;
	double getStdDev() const;
	T getFitness();

	Statistics();
	~Statistics();

	void computeGraphStats(TabuAdjMatrix<T>* graph);

};

#include "../statistics.inl"

#endif
