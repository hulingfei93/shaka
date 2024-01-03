#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <stdlib.h>
#include <fstream>
#include "SList.h"
#include "Queue.h"
#include "Stack.h"

using namespace std;

class graph
{
public:
	typedef void(graph::*search_fn_ptr)(int, bool[]);

	graph();
	~graph();
	void read_data(const char *input_file_path);
	
	void find_connected_components_v1(const char *output_file_path);
	void find_connected_components_v2(const char *output_file_path);
	void find_cutpoints(const char *output_file_path);
private:
	void explore_from_breadth_first_search(int s, bool S[]);
	void explore_from_depth_first_search(int s, bool S[]);
	void find_connected_components_imp(search_fn_ptr, SList<SList<int> > &connected_components);
	void output_connected_components(const char *output_file_path, SList<SList<int> > &connected_components);
	void find_cutpoints_imp(const char *output_file_path, SList<SList<int> > &connected_components);
	void clear();

	SList<int> *_graph;
	int nodes;
	int edges;
	
};

#endif