#ifndef LISTGRAPH_H
#define LISTGRAPH_H

#include "IGraph.h"

#include <vector>


class ListGraph : public IGraph
{
public:

    ListGraph(int _numVertices);

    ListGraph(const IGraph& other);

    ~ListGraph() override;

    void AddEdge(int from, int to) override;

    int VerticesCount() const override;

    void GetNextVertices(int vertex, std::vector<int>& vertices) const override;
    void GetPrevVertices(int vertex, std::vector<int>& vertices) const override;

private:

    std::vector< std::vector<int> > graph;

    int numVertices;
};

#endif