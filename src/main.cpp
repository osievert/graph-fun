#include <string>

#include "Graph.h"

using namespace container;

int main()
{
    auto i = 12;
    printf("%d\n", i);

    Graph<int,int> g;
    auto v1 = g.addVertex(7);
    auto v2 = g.addVertex(2);
    auto v3 = g.addVertex(3);
    printf("%d\n", g.vertices.front()->data);

    g.addEdge(v1, v2, 100);    // add an edge connecting two vertices

    auto e = g.addEdge(101);   // add an edge then connect it to vertices later
    e->start = v2;
    e->end = v3;

    v1->data = 1;              // update vertex data

    printf("%d\n", g.vertices.front()->data);
    return 0;
}

