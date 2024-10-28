// Copyright (c) 2022 GoPro, Inc. All rights reserved.
// THIS SOURCE CODE IS THE PROPRIETARY INTELLECTUAL PROPERTY AND CONFIDENTIAL
// INFORMATION OF GOPRO, INC. AND IS PROTECTED UNDER U.S. AND INTERNATIONAL
// LAW. ANY USE OF THIS SOURCE CODE WITHOUT THE PRIOR WRITTEN AUTHORIZATION OF
// GOPRO IS STRICTLY PROHIBITED.

#include <limits>

#include "doctest/doctest.h"

#include "common/type/Graph.h"
#include "common/utility/Print.h"

using namespace common;

TEST_SUITE_BEGIN("graph");

TEST_CASE("construct")
{
    Graph<int,int> g;
    auto v1 = g.addVertex(7);
    auto v2 = g.addVertex(2);
    auto v3 = g.addVertex(3);
    g.addEdge(v1, v2, 100);    // add an edge connecting to vertices now
    auto e = g.addEdge(101);   // add an edge then connect it to vertices later
    e->start = v2;
    e->end = v3;
    v1->data = 1;              // update vertex data

    // common::println("{}", g);

    CHECK(g.vertices.size() == 3);
    CHECK(g.edges.size() == 2);
    CHECK(g.edgesIn(v1).size() == 0);
    CHECK(g.edgesIn(v2).size() == 1);
    CHECK(g.edgesIn(v3).size() == 1);
    CHECK(g.edgesOut(v1).size() == 1);
    CHECK(g.edgesOut(v2).size() == 1);
    CHECK(g.edgesOut(v3).size() == 0);
    CHECK(g.edges.front()->data == 100);
    CHECK(g.edges.back()->data == 101);
    CHECK(v1->data == 1);
    CHECK(v2->data == 2);
    CHECK(v3->data == 3);
}

TEST_CASE("construct_nullptr")
{
    Graph<int,int> g;
    auto v1 = g.addVertex(1);
    auto e1 = g.addEdge(v1, nullptr, 110);
    auto e2 = g.addEdge(nullptr, v1, 101);
    auto e3 = g.addEdge(nullptr, nullptr, 100);

    // common::println("{}", g);

    CHECK(g.vertices.size() == 1);
    CHECK(g.edges.size() == 3);

    g.removeEdge(e1);
    CHECK(g.vertices.size() == 1);
    CHECK(g.edges.size() == 2);

    g.removeEdge(e2);
    CHECK(g.vertices.size() == 1);
    CHECK(g.edges.size() == 1);

    g.removeEdge(e3);
    CHECK(g.vertices.size() == 1);
    CHECK(g.edges.size() == 0);
}

TEST_CASE("edges")
{
    Graph<int,int> g;
    auto v1 = g.addVertex(1);
    auto v2 = g.addVertex(2);
    auto v3 = g.addVertex(3);
    auto e1 = g.addEdge(v1, v2, 112);
    auto e2 = g.addEdge(v1, v3, 113);
    auto e3 = g.addEdge(v3, v1, 131);
    auto e4 = g.addEdge(v2, v3, 123);
    CHECK(g.edgesIn(v1).size() == 1);
    CHECK(g.edgesOut(v1).size() == 2);
    CHECK(g.edgesAll(v1).size() == 3);
    CHECK(g.edgesIn(v2).size() == 1);
    CHECK(g.edgesOut(v2).size() == 1);
    CHECK(g.edgesAll(v2).size() == 2);
    CHECK(g.edgesIn(v3).size() == 2);
    CHECK(g.edgesOut(v3).size() == 1);
    CHECK(g.edgesAll(v3).size() == 3);
}

TEST_CASE("remove")
{
    Graph<int,int> g;
    auto v1 = g.addVertex(1);
    auto v2 = g.addVertex(2);
    auto v3 = g.addVertex(3);
    auto e1 = g.addEdge(v1, v2, 112);
    auto e2 = g.addEdge(v1, v3, 113);
    auto e3 = g.addEdge(v3, v1, 131);
    auto e4 = g.addEdge(v2, v3, 123);
    auto e5 = g.addEdge(v2, v2, 122);

    // common::println("{}", g);

    // removing an edge removes from the global list as well as the vertex list
    CHECK(g.edges.size() == 5);
    CHECK(g.edgesOut(v2).size() == 2);
    g.removeEdge(e4);
    CHECK(g.edges.size() == 4);
    CHECK(g.edgesOut(v2).size() == 1);

    // removing a vertex removes all edges starting or ending on that vertex
    CHECK(g.vertices.size() == 3);
    CHECK(g.edges.size() == 4);
    g.removeVertex(v1);
    CHECK(g.vertices.size() == 2);
    CHECK(g.edges.size() == 1);

    // remove an edge that doesn't exist
    CHECK(g.edges.size() == 1);
    g.removeEdge({});
    CHECK(g.edges.size() == 1);

    // remove an edge that was already removed
    CHECK(g.edges.size() == 1);
    g.removeEdge(e4);
    CHECK(g.edges.size() == 1);

    // remove a vertex that doesn't exist
    CHECK(g.vertices.size() == 2);
    g.removeVertex({});
    CHECK(g.vertices.size() == 2);

    // remove a vetex that was already remoed
    CHECK(g.vertices.size() == 2);
    g.removeVertex(v1);
    CHECK(g.vertices.size() == 2);
}

TEST_SUITE_END();
