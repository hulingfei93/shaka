#include "graph.h"

graph::graph()
{
	_graph = 0;
}

graph::~graph()
{
	if (_graph != 0)
		clear();
}

void graph::clear()
{
	delete[] _graph;
}

void graph::read_data(const char *input_file_path)
{
	ifstream input_file(input_file_path);

	if (input_file.fail())
	{
		string err_msg = "error: The input file " + string(input_file_path) + " opening failed";
		throw exception(err_msg.c_str());
	}

	input_file >> nodes >> edges;

	if (_graph != 0)
		clear();

	_graph = new SList<int>[nodes];

	int edge_end_node_1, edge_end_node_2;

	for (int i = 0; i < edges; ++i)
	{
		input_file >> edge_end_node_1 >> edge_end_node_2;
		if (edge_end_node_1 < 0 
			|| edge_end_node_1 >= nodes
			|| edge_end_node_2 < 0
			|| edge_end_node_2 >=nodes)
		{
			string err_msg = "edge data failure.";
			throw exception(err_msg.c_str());
		}

		_graph[edge_end_node_1].AddLast(edge_end_node_2);
		_graph[edge_end_node_2].AddLast(edge_end_node_1);
	}

	input_file.close();
}

void graph::explore_from_breadth_first_search(int s, bool S[])
{
	Queue<int> _S;
	SList<int>::Iterator *vec_iter = new SList<int>::Iterator[nodes];

	_S.EnQueue(s);

	// mark all edges "unused"
	for (int i = 0; i < nodes; ++i)
	{
		vec_iter[i] = _graph[i].Begin();
	}

	while (!_S.IsEmpty())
	{
		// choose some node v in _S
		int v = _S.Front();
		
		// there is no "unused" edge out of v
		if (vec_iter[v] == _graph[v].End())
		{
			// delete v from _S
			_S.DeQueue();
		}
		else // there is a "unused" edge out of v
		{
			// let (v, w) be the next unused edge out of v
			int w = *(vec_iter[v]);
			++(vec_iter[v]);
			// w is not in S
			if (S[w] == false)
			{
				// add w to S
				S[w] = true;
				// add w to _S
				_S.EnQueue(w);
			}
		}
	}
	delete[] vec_iter;
}

void graph::explore_from_depth_first_search(int s, bool S[])
{
	Stack<int> _S;
	SList<int>::Iterator *vec_iter = new SList<int>::Iterator[nodes];

	_S.Push(s);

	// mark all edges "unused"
	for (int i = 0; i < nodes; ++i)
	{
		vec_iter[i] = _graph[i].Begin();
	}

	while (!_S.IsEmpty())
	{
		// choose some node v in _S
		int v = _S.Top();

		// there is no "unused" edge out of v
		if (vec_iter[v] == _graph[v].End())
		{
			// delete v from _S
			_S.Pop();
		}
		else // there is a "unused" edge out of v
		{
			// let (v, w) be the next unused edge out of v
			int w = *(vec_iter[v]);
			++(vec_iter[v]);
			// w is not in S
			if (S[w] == false)
			{
				// add w to S
				S[w] = true;
				// add w to _S
				_S.Push(w);
			}
		}
	}

	delete[] vec_iter;
}

void graph::find_connected_components_v1(const char *output_file_path)
{
	SList<SList<int> > connected_components;
	// use depth first search to find connected component
	find_connected_components_imp(&graph::explore_from_depth_first_search, connected_components);
	// save result to file
	output_connected_components(output_file_path, connected_components);
}

void graph::find_connected_components_v2(const char *output_file_path)
{
	SList<SList<int> > connected_components;
	// use breadth first search to find connected component
	find_connected_components_imp(&graph::explore_from_breadth_first_search, connected_components);
	// save result to file
	output_connected_components(output_file_path, connected_components);
}


void graph::find_connected_components_imp(search_fn_ptr search_fn, SList<SList<int> > &connected_components)
{
	bool *vec_nodes = new bool[nodes];
	bool *S = new bool[nodes];
	for (int i = 0; i < nodes; ++i)
	{
		vec_nodes[i] = false;
	}

	bool is_all_visited = false;
	int s = 0;

	int num = 1;
	while (!is_all_visited)
	{
		for (int i = 0; i < nodes; ++i)
		{
			S[i] = false;
		}
		S[s] = true;

		(this->*search_fn)(s, S);
				
		// merge with vec_nodes and store results to connected_components
		SList<int> connected_component;
		for (int i = 0; i < nodes; ++i)
		{
			if (S[i] == true)
			{
				if (vec_nodes[i] != S[i])
				{
					vec_nodes[i] = S[i];
				}
				connected_component.AddLast(i);
			}
		}
		connected_components.AddLast(connected_component);

		// check if all nodes have been visited
		is_all_visited = true;
		for (int i = 0; i < nodes; ++i)
		{
			if (vec_nodes[i] == false)
			{
				is_all_visited = false;
				s = i;
				break;
			}
		}
	}

	delete[] vec_nodes;
	delete[] S;
}

void graph::output_connected_components(const char *output_file_path, SList<SList<int> > &connected_components)
{
	ofstream output_file(output_file_path);

	int num = 1;
	while (!connected_components.IsEmpty())
	{
		output_file << "component " << num++ << " = (";
		bool is_first_node = true;
		SList<int> connected_component = connected_components.ShowFirst();
		connected_components.RemoveFirst();
		while (!connected_component.IsEmpty())
		{
			if (is_first_node)
			{
				output_file << connected_component.ShowFirst();
				is_first_node = false;
			}
			else
			{
				output_file << "," << connected_component.ShowFirst();
			}
			connected_component.RemoveFirst();
		}
		output_file << ")" << endl;
		
	}

	output_file.close();
}

void graph::find_cutpoints(const char *output_file_path)
{
	SList<SList<int> > connected_components;
	// find connected components
	find_connected_components_imp(&graph::explore_from_depth_first_search, connected_components);

	find_cutpoints_imp(output_file_path, connected_components);
}

void graph::find_cutpoints_imp(const char *output_file_path, SList<SList<int> > &connected_components)
{
	ofstream output_file(output_file_path);

	// store the edge type. -1: back edge, 0: no edge, 1: tree edge, 
	int **matrix = new int *[nodes]; 
	for (int i = 0; i < nodes; ++i)
	{
		matrix[i] = new int[nodes];
	}

	// mark whether or not the nodes have been outputed as the cutpoints
	bool *cutpoints = new bool[nodes];
	
	
	int num = 1;
	// for each connected component, identify and output the cutpoints
	while (!connected_components.IsEmpty())
	{
		for (int i = 0; i < nodes; ++i)
		{
			for (int j = 0; j < nodes; ++j)
				matrix[i][j] = 0;
		}
		for (int i = 0; i < nodes; ++i)
		{
			cutpoints[i] = false;
		}

		SList<int> connected_component = connected_components.ShowFirst();
		connected_components.RemoveFirst();

		// start node
		int s = connected_component.ShowFirst();

		// output connected component
		output_file << "component " << num++ << " = (";
		bool is_first_node = true;
		while (!connected_component.IsEmpty())
		{
			if (is_first_node)
			{
				output_file << connected_component.ShowFirst();
				is_first_node = false;
			}
			else
			{
				output_file << "," << connected_component.ShowFirst();
			}
			connected_component.RemoveFirst();
		}
		output_file << ")" << '\t';

	//step_1:
		Stack<int> _S;
		_S.Push(s);
		int *dfn = new int[nodes];
		int *low = new int[nodes];
		for (int i = 0; i < nodes; ++i)
			dfn[i] = 0;
		int order = 0;
		int v = s;
		int u, p;

		// mark all edges "unused"
		SList<int>::Iterator *vec_iter = new SList<int>::Iterator[nodes];
		for (int i = 0; i < nodes; ++i)
			vec_iter[i] = _graph[i].Begin();

	step_2:
		++order;
		dfn[v] = order;
		low[v] = order;

	step_3:
		// if v has no unused edges, go to step_5
		if (vec_iter[v] == _graph[v].End())
		{
			goto step_5;
		}

	//step_4:
		// choose the next unused edge e = {v, u}
		u = *(vec_iter[v]);
		// mark e "used"
		++(vec_iter[v]);
		if (dfn[u] != 0) // edge e is a back edge
		{
			low[v] = (low[v] <= dfn[u] ? low[v] : dfn[u]);
			matrix[v][u] = -1;
			goto step_3;
		}
		else // edge is a tree edge
		{
			_S.Push(u);
			matrix[v][u] = 1;
			v = u;
			goto step_2;
		}

	step_5:
		if (v == s)
		{
			// compute count of tree edges directed out of it
			int tree_edges_count = 0;
			for (int j = 0; j < nodes; ++j)
			{
				if (matrix[s][j] == 1)
					++tree_edges_count;
			}
			// if tree_edges_count >= 2, s is a cutpoint
			if (tree_edges_count >= 2)
			{
				cutpoints[s] = true;
			}
			goto step_7; // start find the cutpoints for the next connected component
		}

	//step_6:
		_S.Pop();
		p = _S.Top();
		if (p != s)
		{
			low[p] = (low[p] <= low[v] ? low[p] : low[v]);
			if (low[v] >= dfn[p])
			{
				cutpoints[p] = true;
			}
		}
		v = p;
		goto step_3;


	step_7:
		// output cutpoints
		output_file << "cutpoints: ";
		for (int i = 0; i < nodes; ++i)
		{
			if (cutpoints[i] == true)
				output_file << i << '\t';
		}
		output_file << endl;

		//clean
		delete[] dfn;
		delete[] low;
		delete[] vec_iter;
	}

	// clean
	for (int i = 0; i < nodes; ++i)
	{
		delete[] matrix[i];
	}
	delete[] matrix;
	output_file.close();
}