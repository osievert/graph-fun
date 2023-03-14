#include <string>
#include <utility>

#include "Graph.h"

template <typename V, typename E>
std::list<typename Graph<V,E>::Vertex> search(const Graph<V,E>& g)
{
    std::list<typename Graph<V,E>::Vertex> result;

    while (result.size() != g.vertices.size())
    {
        for (const auto& vertex : g.vertices)
        {
            // check if the vertices pointed to by 'vertex' is already output list
            auto outEdges = g.edgesOut(vertex);
            auto addToResult = true;

            for (const auto& edge : outEdges)
            {
                // if the vertex pointed to by edge->end is not in our result, we can't add this vertex
                auto found = std::find(result.begin(), result.end(), edge->end);

                if (found == result.end())
                    addToResult = false;
            }

            if (addToResult)
            {
                result.push_back(vertex);
            }
        }
    }

    return result;
}

int main()
{
    // build the graph we drew on paper
    Graph<std::string,int> g;
    auto vR = g.addVertex("root");
    auto v1 = g.addVertex("1");
    auto v2 = g.addVertex("2");
    auto v3 = g.addVertex("3");
    auto v4 = g.addVertex("4");
    auto v5 = g.addVertex("5");

    g.addEdge(vR, v1, 1);
    g.addEdge(vR, v2, 2);
    g.addEdge(v1, v3, 3);
    g.addEdge(v1, v4, 4);
    g.addEdge(v2, v3, 5);
    g.addEdge(v2, v5, 6);

    printf("first vertex: %s\n", g.vertices.front()->data.c_str());

    auto vertices = search(g);

    for (const auto& vertex : vertices)
    {
        printf("%s\n",vertex->data.c_str());
    }

    return 0;
}
