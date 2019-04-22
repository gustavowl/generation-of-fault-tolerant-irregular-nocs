#ifndef __FILE_MANAGER_H__
#define __FILE_MANAGER_H__

#include <fstream>
#include "graph-representation.h"
#include "adjacency-list.h"
#include "adjacency-matrix.h"

// This class is used for manipulating files
// (Operations such as read and write).
// This class manipulates column-separated-files
class FileManager {

public:
	/* There are two representation types for topologies:
	 * adjacency list, and adjacency matrix.
	 */
	enum FileType { adj_list, adj_matrix };

	/* Attempts to read file in filepath location.
	 * If reading fails, returns false
	 * (e.g. the file does not exist)
	 * and true otherwise. 
	 * */
	static GraphRepresentation<size_t>* readFile(std::string filepath,
			FileType type, char separator=stdSeparator, char comment=stdComment);

private:
	static const char stdComment = '#';
	static const char stdSeparator = ',';

	static AdjacencyList<size_t>* readAdjList(std::ifstream* file,
			char separator, char comment);
	static AdjacencyMatrix<size_t>* readAdjMatrix(std::ifstream* file,
			char separator, char comment);
	
	/* Parses a csv line.
	 * Lines beginning with `comment` are ignored.
	 * 
	 * The `values` parameter will contain the separated values
	 * after the function finishes its computations.
	 * An array of ints is expected.
	 * The size is not assert, thus an error may occur if the
	 * csv file is not properly defined.
	 *
	 * The function returns the length of `result`.
	 */
	static size_t parseLine(std::string* line, int* values,
			char separator, char comment);
};

#endif
