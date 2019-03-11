#ifndef ARCGRAPH_H
#define ARCGRAPH_H

#include "IGraph.h"

#include <vector>
#include <utility>


class ArcGraph : public IGraph
{
public:

    ArcGraph(int _numVertices);

    ArcGraph(const IGraph& other);

    ~ArcGraph() override;

    void AddEdge(int from, int to) override;

    int VerticesCount() const override;

    void GetNextVertices(int vertex, std::vector<int>& vertices) const override;
    void GetPrevVertices(int vertex, std::vector<int>& vertices) const override;

private:

    std::vector< std::pair<int, int> > graph;

    int numVertices;
};

#endif
