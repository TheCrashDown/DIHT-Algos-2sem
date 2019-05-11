#ifndef MSTCREATOR
#define MSTCREATOR

#include "AdvancedIGraph.h"
#include <vector>

class MSTCreator
{
public:

    int findMinSpanningTree(std::vector< std::pair<int, int> >& path);

    MSTCreator(const AdvancedIGraph& _graph);

    ~MSTCreator();

private:

    const AdvancedIGraph& graph;

    int Prim(std::vector< std::pair<int, int> >& path);

    struct Point;

    //  компаратор для очереди с приоритетом
    class PointCompare;
};


#endif
