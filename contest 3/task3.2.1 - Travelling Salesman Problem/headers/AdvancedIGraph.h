#ifndef ADVANCEDIGRAPH
#define ADVANCEDIGRAPH

#include <vector>

class AdvancedIGraph
{
public:

    virtual ~AdvancedIGraph() {};

    virtual void setEdge(int from, int to, double weight) = 0;

    virtual double getEdge(int from, int to) const = 0;

    virtual int VerticesCount() const = 0;

    virtual void GetNextVertices(int vertex, std::vector< std::pair<int, double> >& vertices) const = 0;
    virtual void GetPrevVertices(int vertex, std::vector< std::pair<int, double> >& vertices) const = 0;

};

#endif
