#ifndef MATRIXGRAPH_H
#define MATRIXGRAPH_H

#include "IGraph.h"

#include <vector>

class MatrixGraph : public IGraph
{
public:

    MatrixGraph(int _numVertices);
    MatrixGraph(const IGraph& other);

    ~MatrixGraph() override;

    void AddEdge(int from, int to) override;

    int VerticesCount() const override;

    void GetNextVertices(int vertex, std::vector<int>& vertices) const override;
    void GetPrevVertices(int vertex, std::vector<int>& vertices) const override;

private:

    std::vector< std::vector<bool> > graph;
    std::vector< std::vector<bool> > reverseGraph;

    int numVertices;
};

#endif
