#include "graph.h"

int main(int argc, char **argv)
{
    try
    {
        graph g;
        g.read_data("infile.dat");

        // use depth first search to find connected components
        g.find_connected_components_v1("outfile01.dat");

        // use breadth first search to find connected components
        g.find_connected_components_v2("outfile02.dat");
        
        // find connected components and their cutpoints
        g.find_cutpoints("outfile.dat");

        graph g1;
        g1.read_data("infile1.dat");

        // use depth first search to find connected components
        g1.find_connected_components_v1("outfile11.dat");

        // use breadth first search to find connected components
        g1.find_connected_components_v2("outfile12.dat");

        // find connected components and their cutpoints
        g1.find_cutpoints("outfile1.dat");
    }
    catch (exception ex)
    {
        cerr << ex.what() << endl;
    }
    

    return 0;
}
