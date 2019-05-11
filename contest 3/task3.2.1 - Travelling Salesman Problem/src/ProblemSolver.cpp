#include "../headers/ProblemSolver.h"
#include "../headers/MSTCreator.h"
#include "../headers/MatrixGraph.h"



ProblemSolver::ProblemSolver(const AdvancedIGraph& _graph) : graph(_graph) {}


ProblemSolver::~ProblemSolver(){}


double ProblemSolver::findMinPath(){
    return MSTSearch();
}

//  получаем мин остов из класса MSTCreator(скопированный файл из первой задачи
//  с дополнительной функцией восстановлениея пути), обходим его DFSом, чтобы 
//  сформировать путь коммивояжера, и считаем его длину
double ProblemSolver::MSTSearch(){
    std::vector< std::pair<int, int> > mst_pairs;

    MSTCreator mstCreator(graph);
    mstCreator.findMinSpanningTree(mst_pairs);

    AdvancedIGraph* mst = new MatrixGraph(graph.VerticesCount());

    for(auto p : mst_pairs){
        mst->setEdge(p.first, p.second, graph.getEdge(p.first, p.second));
        mst->setEdge(p.second, p.first, graph.getEdge(p.first, p.second));
    }

    std::vector< bool > visited(graph.VerticesCount(), false);
    std::vector< int > path;
    visited[0] = true;


    DFS(visited, path, 0);

    //  подсчет длины пути, аналогично тому что использован в реализации
    //  класса BrutForce, подробное описание функции можно прочитать там
    double tmp = 0.0;
    for (int i = 0; i < graph.VerticesCount() - 2; ++i)
    {
        tmp += graph.getEdge(path[i], path[i + 1]);
    }
    tmp += graph.getEdge(0, path[0]);
    tmp += graph.getEdge(0, path[graph.VerticesCount() - 2]);

    return tmp;

}

//  простой обход, который пропускает уже посешенные вершины и формирует путь для коммивояжера из остова  
void ProblemSolver::DFS(std::vector< bool >& visited, std::vector< int >& path,  int vertex){

    std::vector< std::pair<int, double> > children;
    graph.GetNextVertices(vertex, children);

    for(auto child : children){
        if(!visited[child.first]){
            visited[child.first] = true;
            path.push_back(child.first);
            DFS(visited, path, child.first);
        }
    }
}
