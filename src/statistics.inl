template <class T>
T Statistics<T>::getMinWeight() const {
	return this->minWeight;
}

template <class T>
T getMaxWeight() const{
	return this->maxWeight;
}

template <class T>
double getMeanWeight() const{
	return this->meanWeight;
}

template <class T>
double getStdDev() const{
	return this->stdDev;
}

template <class T>
T getFitness(){
	return this->fitness;
}

template <class T>
void computeGraphStats(TabuAdjMatrix<T>* graph){
	//do magic
}
