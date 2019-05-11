#ifndef MATRIXGRAPH
#define MATRIXGRAPH

#include "AdvancedIGraph.h"

#include <vector>
#include <climits>

class MatrixGraph : public AdvancedIGraph
{

public:

    MatrixGraph(int _numVertices);

    MatrixGraph(const AdvancedIGraph& other);

    ~MatrixGraph() override;

    void setEdge(int from, int to, double weight) override;
    double getEdge(int from, int to) const override;


    int VerticesCount() const override;

    void GetNextVertices(int vertex, std::vector< std::pair<int, double> >& vertices) const override;
    void GetPrevVertices(int vertex, std::vector< std::pair<int, double> >& vertices) const override;


private:

    std::vector< std::vector< double > > graph;

    int numVertices;

};



#endif