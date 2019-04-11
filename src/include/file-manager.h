#ifndef __FILE_MANAGER_H__
#define __FILE_MANAGER_H__

#include <fstream>

class FileManager {
	public:
		/* Attempts to read file in filepath location.
		 * If reading fails, returns false
		 * (e.g. the file does not exist)
		 * and true otherwise. 
		 * */
		static bool readFile(std::string filepath);
};

#endif
