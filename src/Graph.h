// Copyright (c) 2022 GoPro, Inc. All rights reserved.
// THIS SOURCE CODE IS THE PROPRIETARY INTELLECTUAL PROPERTY AND CONFIDENTIAL
// INFORMATION OF GOPRO, INC. AND IS PROTECTED UNDER U.S. AND INTERNATIONAL
// LAW. ANY USE OF THIS SOURCE CODE WITHOUT THE PRIOR WRITTEN AUTHORIZATION OF
// GOPRO IS STRICTLY PROHIBITED.

#pragma once

#include <functional>
#include <list>
#include <memory>
#include <string>
#include <vector>

// A graph data structure.
//
// Every graph data structure is study in compromise. This implementation:
// - is mutabile, using std::list, which means that it will not be as performant
//   as you could make an immutable arena graph using std::vector.
// - optimizes for global operations by storing vertices in a single
//   lifetime-owning list, and by storing edges in a single lifetime-owning
//   list.

// API:
// - The user provides their own data types via the V (vertex) and E (edge) template types.
// - The graph places these data inside VertexElement and EdgeElement structs, which add
//   relationships to create the graph:
//   - every edge knows which vertices it involves (start and end)
// - The graph is built using std::list<std::shared_ptr<T>>. These shared pointers
//   are visible to the user as Graph::Vertex and Graph::Edge. Because of this, users
//   need to know these types must be dereferenced to access the VertexElement and
//   EdgeElement types, both of which own the user data in "data" members. This ends up
//   being a bit of a word salad. Here is an example of how to get at the user data:
//
//     struct Foo { int a = 42; };
//     struct Bar { int a = 42; };
//     Graph<Foo,Bar>::Vertex v = g.addVertex(Foo {3});
//     int vertexValue = v->data.a; // value: 3
//     Graph<Foo,int>::Edge e = g.addEdge(v, {}, Bar {4});
//     int edgeValue = e->data.a; // value: 4

template <typename V, typename E>
struct Graph
{
    struct EdgeElement;
    struct VertexElement;

    using Vertex = std::shared_ptr<VertexElement>;
    using Edge = std::shared_ptr<EdgeElement>;

    struct EdgeElement
    {
        Vertex start;
        Vertex end;
        E data;

        EdgeElement() = delete;
        EdgeElement(Vertex start, Vertex end, E edgeData) : start(start), end(end), data(edgeData) {}

        E&       operator*()       { return data; }
        const E& operator*() const { return data; }

        std::string string() const { return ""; }
    };

    struct VertexElement
    {
        V data;

        VertexElement() = delete;
        VertexElement(V vertexData) : data(vertexData) {}

        V&       operator*()       { return data; }
        const V& operator*() const { return data; }

        std::string string() const { return ""; }
    };

    std::list<Vertex> vertices;
    std::list<Edge> edges;

    Vertex addVertex(V vertexData);
    void   removeVertex(Vertex vertex);

    Edge   addEdge(E edgeData);
    Edge   addEdge(Vertex start, Vertex end, E edgeData);
    void   removeEdge(Edge edge);

    std::vector<Edge> edgesIn(const Vertex vertex) const;
    std::vector<Edge> edgesOut(const Vertex vertex) const;
    std::vector<Edge> edgesAll(const Vertex vertex) const;

    std::string string() const;
    bool empty() const;
};


template <typename V, typename E>
auto Graph<V,E>::addVertex(V vertexData) -> Vertex
{
    auto vertex = std::make_shared<VertexElement>(vertexData);
    vertices.emplace_back(vertex);
    return vertex;
}

template <typename V, typename E>
void Graph<V,E>::removeVertex(Vertex vertex)
{
    auto v = std::find(vertices.begin(), vertices.end(), vertex);
    if (v == vertices.end()) return;

    edges.remove_if([vertex](const Edge& edge) { return edge->end == vertex || edge->start == vertex; });
    vertices.erase(v);
}

template <typename V, typename E>
auto Graph<V,E>::addEdge(E edgeData) -> Edge
{
    auto edge = std::make_shared<EdgeElement>(nullptr, nullptr, edgeData);
    edges.push_back(edge);
    return edge;
}

template <typename V, typename E>
auto Graph<V,E>::addEdge(Vertex start, Vertex end, E edgeData) -> Edge
{
    auto edge = std::make_shared<EdgeElement>(start, end, edgeData);
    edges.push_back(edge);
    return edge;
}

template <typename V, typename E>
void Graph<V,E>::removeEdge(Edge edge)
{
    edges.remove_if([edge](const Edge& e) { return e == edge; });
}

template <typename V, typename E>
auto Graph<V,E>::edgesIn(const Vertex vertex) const -> std::vector<Edge>
{
    std::vector<Edge> output;
    std::copy_if(edges.begin(), edges.end(), std::back_inserter(output), [vertex](const Edge& edge) {
        return edge != nullptr && edge->end == vertex;
    });
    return output;
}

template <typename V, typename E>
auto Graph<V,E>::edgesOut(const Vertex vertex) const -> std::vector<Edge>
{
    std::vector<Edge> output;
    std::copy_if(edges.begin(), edges.end(), std::back_inserter(output), [vertex](const Edge& edge) {
        return edge != nullptr && edge->start == vertex;
    });
    return output;
}

template <typename V, typename E>
auto Graph<V,E>::edgesAll(const Vertex vertex) const -> std::vector<Edge>
{
    std::vector<Edge> output;
    std::copy_if(edges.begin(), edges.end(), std::back_inserter(output), [vertex](const Edge& edge) {
        return edge != nullptr && (edge->start == vertex || edge->end == vertex);
    });
    return output;
}

template <typename V, typename E>
bool Graph<V,E>::empty() const
{
    return vertices.empty() && edges.empty();
};

template <typename V, typename E>
std::string Graph<V,E>::string() const
{
    return {}; //common::format("vertices:{}\nedges:{}", vertices, edges);
}
