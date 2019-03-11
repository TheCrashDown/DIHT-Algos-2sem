#ifndef SETGRAPH_H
#define SETGRAPH_H

#include "IGraph.h"

#include <vector>
#include <unordered_set>

class SetGraph : public IGraph
{
public:

    SetGraph(int _numVertices);

    SetGraph(const IGraph& other);

    ~SetGraph() override;

    void AddEdge(int from, int to) override;

    int VerticesCount() const override;

    void GetNextVertices(int vertex, std::vector<int>& vertices) const override;
    void GetPrevVertices(int vertex, std::vector<int>& vertices) const override;

private:

    std::vector< std::unordered_set<int> > graph;
    std::vector< std::unordered_set<int> > reverseGraph;

    int numVertices;
};

#endif
