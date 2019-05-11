#ifndef BRUTFORCE
#define BRUTFORCE

#include "AdvancedIGraph.h"

#include <vector>
#include <climits>


class BrutForce
{
public:

    BrutForce(const AdvancedIGraph& _graph);

    ~BrutForce();

    double findMinPath();


private:

    const AdvancedIGraph& graph;

    std::vector< int > path;

    double currMin = INT_MAX;

    void pathfinder(int depth);

    double getPathLength();
};


#endif