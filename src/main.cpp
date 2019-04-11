#include "include/file-manager.h"
#include <iostream>

int main(int argc, char *argv[]) {
	std::cout << "Hello World" << std::endl;
	std::cout << std::to_string(FileManager::readFile("test.txt")) << std::endl;
	return 0;
}
