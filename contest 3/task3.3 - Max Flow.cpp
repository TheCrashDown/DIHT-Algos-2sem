/* Task:
   Задан ориентированный граф, каждое ребро которого обладает целочисленной пропускной способностью. 
   Найдите максимальный поток из вершины с номером 1 в вершину с номером n.

   Решить с помощью алгоритма Эдмондса-Карпа

   developed by Crashdown

*/
#include <iostream>
#include <vector>
#include <queue>
#include <memory>
#include <climits>


//===============================================================================//
//===================скопированный файл AdvancedIGraph.h=========================//

class AdvancedIGraph
{
public:

    virtual ~AdvancedIGraph() {};

    virtual void setEdge(int from, int to, int weight) = 0;
    virtual int getEdge(int from, int to) const = 0;

    virtual int VerticesCount() const = 0;

    virtual void GetNextVertices(int vertex, std::vector< std::pair<int, int> >& vertices) const = 0;
    virtual void GetPrevVertices(int vertex, std::vector< std::pair<int, int> >& vertices) const = 0;

};

//===============================================================================//
//=====================скопированный файл MatrixGraph.h============================//

class MatrixGraph : public AdvancedIGraph
{

public:

    MatrixGraph(int _numVertices);

    MatrixGraph(const AdvancedIGraph& other);

    ~MatrixGraph() override;

    void setEdge(int from, int to, int weight) override;
    int getEdge(int from, int to) const override;


    int VerticesCount() const override;

    void GetNextVertices(int vertex, std::vector< std::pair<int, int> >& vertices) const override;
    void GetPrevVertices(int vertex, std::vector< std::pair<int, int> >& vertices) const override;


private:

    std::vector< std::vector< int > > graph;

    int numVertices;

};

//===============================================================================//
//====================скопированный файл MatrixGraph.cpp===========================//


MatrixGraph::MatrixGraph(int _numVertices)
{
    numVertices = _numVertices;
    graph.resize(numVertices);
    for (int i = 0; i < numVertices; ++i)
    {
        graph[i].resize(numVertices, 0);
    }
}


MatrixGraph::~MatrixGraph(){}

MatrixGraph::MatrixGraph(const AdvancedIGraph& other)
{
    numVertices = other.VerticesCount();
    graph.resize(numVertices);
    for (int i = 0; i < numVertices; ++i)
    {
        graph[i].resize(numVertices, 0);
    }

    for(int i = 0; i < numVertices; ++i){

        std::vector< std::pair<int, int> > children;
        other.GetNextVertices(i, children);

        for(int j = 0; j < children.size(); ++j){
            setEdge(i, children[j].first, children[j].second);
        }
    }
}

void MatrixGraph::setEdge(int from, int to, int weight)
{
    graph[from][to] = weight;
}

int MatrixGraph::getEdge(int from, int to) const
{
    return graph[from][to];
}

int MatrixGraph::VerticesCount() const
{
    return numVertices;
}

void MatrixGraph::GetNextVertices(int vertex, std::vector< std::pair<int, int> >& vertices) const
{
    vertices.resize(0);
    for(int i = 0; i < numVertices; ++i){
        if(graph[vertex][i] != 0){
            vertices.push_back(std::make_pair(i, graph[vertex][i]));
        }
    }
}

void MatrixGraph::GetPrevVertices(int vertex, std::vector< std::pair<int, int> >& vertices) const
{
    vertices.resize(0);
    for(int i = 0; i < numVertices; ++i){
        if(graph[i][vertex] != 0){
            vertices.push_back(std::make_pair(i, graph[i][vertex]));
        }
    }
}


//==========================конец скопированной части============================//
//===============================================================================//



//===============================================================================//
//===========модуль, решающий задачу на переданном в конструкторе графе==========//

class ProblemSolver
{
public:

    int findMaxFlow(int from, int to);

    ProblemSolver(const AdvancedIGraph& _graph){
        graph = new MatrixGraph(_graph);
    }

    ~ProblemSolver(){};

private:

    AdvancedIGraph* graph;

    int EdmondsKarp(int from, int to);

    int BFS(int from, int to);

};

int ProblemSolver::findMaxFlow(int from, int to){
    return EdmondsKarp(from, to);
}

int ProblemSolver::BFS(int from, int to){
    std::queue< int > order;
    std::vector< bool > visited(graph->VerticesCount(), false);
    std::vector< int > parents(graph->VerticesCount(), -1);
    order.push(from);
    parents[from] = -2;

    //  ищем путь
    while(!order.empty()){
        int current = order.front();
        order.pop();
        bool found = false;

        visited[current] = true;

        std::vector< std::pair< int, int > > children;
        graph->GetNextVertices(current, children);

        for(auto child : children){
            int childVertex = child.first;
            int childCapacity = child.second;

            if(!visited[childVertex] && childCapacity != 0){
                order.push(childVertex);
                parents[childVertex] = current;
                visited[childVertex] = true;
            }

            if(childVertex == to){
                found = true;
                break;
            }
        }
        if(found){
            break;
        }
    }

    //  путь не найден
    if(parents[to] == -1){
        return -1;
    }

    //  обработка пути : поиск минимума
    int pos = to;
    int currMin = INT_MAX;
    while(true){
        if(parents[pos] == -2){
            break;
        }
        int capacity = graph->getEdge(parents[pos], pos);
        if(capacity < currMin){
            currMin = capacity;
        }
        pos = parents[pos];
    }

    //  обработка пути : изменение ребер пути
    pos = to;
    while(true){
        if(parents[pos] == -2){
            break;
        }
        graph->setEdge(pos, parents[pos], graph->getEdge(pos, parents[pos]) + currMin);
        graph->setEdge(parents[pos], pos, graph->getEdge(parents[pos], pos) - currMin);
        pos = parents[pos];
    }

    return currMin;
}

int ProblemSolver::EdmondsKarp(int from, int to){

    int maxFlow = 0;

    while(true){
        //  пока есть путь, обрабатываем его и добовляем к ответу
        int currFlow = BFS(from, to);
        if(currFlow < 0){
            break;
        }

        maxFlow += currFlow;
    }

    return maxFlow;
}



int main()
{
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);

    int n, m;
    std::cin >> n >> m;

    AdvancedIGraph* graph = new MatrixGraph(n);

    for(int i = 0; i < m; ++i){
        int from, to, weight;
        std::cin >> from >> to >> weight;
        graph->setEdge(from - 1, to - 1, weight);
    }

    ProblemSolver problemSolver(*graph);

    std::cout << problemSolver.findMaxFlow(0, n - 1);

    return 0;
}

