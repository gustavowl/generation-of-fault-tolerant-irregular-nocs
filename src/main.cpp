#include "include/file-manager.h"
#include <iostream>

int main(int argc, char *argv[]) {
	std::cout << "Hello World" << std::endl;
	std::cout << std::to_string(FileManager::readFile(
				"task-graphs/adj-lists/descriptions_seed-73_multi-start-node_weight-range-1-100_dimension-4x4_in-out-degrees-2-2.adjl",
				FileManager::FileType::adj_list)) << std::endl;
	return 0;
}
