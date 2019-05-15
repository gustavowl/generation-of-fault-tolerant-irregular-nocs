//TODO: clean includes
#include <iostream>
#include "include/file-manager.h"
#include "include/graph-converter.h"
#include "include/graph-representation.h"
#include "include/tabu-adj-matrix.h"
#include "include/tabu-search.h"
#include "include/benchmark.h"
#include <sys/time.h>

std::string getTime() {
	timeval curTime;
	gettimeofday(&curTime, NULL);
	int milli = curTime.tv_usec / 1000;

	char buffer [80];
	strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", localtime(&curTime.tv_sec));

	char currentTime[84] = "";
	sprintf(currentTime, "%s:%d", buffer, milli);
	return currentTime;
}

int main(int argc, char *argv[]) {
	if (argc != 7) {
		std::cout << "Invalid number of arguments! Six expected:\n" <<
			"\t1 - Path to .adjl file;\n" <<
			"\t2 - Value separator (',' is the standard);\n" <<
			"\t3 - Number of edges in the output graph (epsilon);\n" <<
			"\t4 - Tabu list size;\n" <<
			"\t5 - Number of iterations with no improvements " <<
			"(stop criterion)\n" <<
		   	"\t6 - Output filename" << std::endl;
		return -1;
	}

	//std::cout << "\n";
	//for (int i = 0; i < 7; i++)
	//	std::cout << argv[i] << ' ';
	//std::cout << std::endl;

	std::string currTime = getTime();
	std::cout << currTime << "\tReading \"" << argv[1] << "\" file... " <<
		std::endl;

	GraphRepresentation<size_t>* gr = FileManager::readFile(
			argv[1], FileManager::FileType::adj_list, argv[2][0]);

	if (gr == NULL) {
		std::cout << "Failed to read \"" << argv[0] <<
			"\" file using separator '" << argv[1][0] <<
			"'." << std::endl;
		return -1;
	}

	TabuAdjMatrix<bool> adjm = TabuAdjMatrix<bool>(
			gr->getNumNodes(), false);
	GraphConverter::convert(gr, &adjm);
	
	/*std::cout << "Graph read from file:\n" <<
		"==============================" << std::endl;
	gr->print();
	std::cout << "==============================" << std::endl;
	std::cout << "Converted Graph:\n" <<
		"==============================" << std::endl;
	adjm.print();

	std::cout << "==============================" << std::endl;
	std::cout << gr->getNumEdges() << ' ' << adjm.getNumEdges() <<
		std::endl;*/

	TabuSearch<size_t> ts;
	ts.setTaskGraph(gr);
	ts.setEpsilon(std::stoi(argv[3]));
	ts.setTabuListSize(std::stoi(argv[4]));
	ts.setStopCriteria(std::stoi(argv[5]));
	ts.setFitnessLimit(std::numeric_limits<size_t>::max());
	ts.setDegreeLimits(2, 4);

	currTime = getTime();
	std::cout << currTime << "\tBegins Tabu Search... " << std::endl;
	TabuAdjMatrix<bool>* res = ts.start();
	
	Benchmark<size_t> bmk;
	bmk.setTaskGraph(gr);
	bmk.setTopology(res);
	bmk.setWeightInf(HOP_INF);
	bmk.setTabuStatsFilename("tabu-stats.csv");
	bmk.setUsedTabuArgs(argv[1], argv[6], std::stoi(argv[3]),
			std::stoi(argv[4]), std::stoi(argv[5]),
			ts.getPerformedIterations());

	currTime = getTime();	
	std::cout << currTime <<
		"\tBegins resulted topology benchmarking... " <<
		std::endl;
	bmk.start();

	delete gr;

	if (res == NULL) {
		std::cout << "Error running Tabu Search.\nProbably invalid " <<
			"epsilon or tabu list size values" << std::endl;
		return -1;
	}

	delete res;

	return 0;
}
