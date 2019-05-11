#ifndef PROBLEMSOLVER
#define PROBLEMSOLVER

#include "AdvancedIGraph.h"
#include <vector>


class ProblemSolver
{
public:

    double findMinPath();

    ProblemSolver(const AdvancedIGraph& _graph);

    ~ProblemSolver();

    
private:   

    const AdvancedIGraph& graph;


    void DFS(std::vector< bool >& visited, std::vector< int >& path,  int vertex);

    double MSTSearch();

};




#endif