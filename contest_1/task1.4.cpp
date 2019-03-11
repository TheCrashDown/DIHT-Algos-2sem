/* Task :
   Дан невзвешенный неориентированный граф. Определить, является ли он двудольным.
   Требуемая сложность O(V+E).

   developed by Crashdown

*/
#include <iostream>
#include <vector>
#include <queue>
#include <memory>


//===============================================================================//
//=======================скопированный файл IGraph.h=============================//

class IGraph
{
public:

    virtual ~IGraph() {};

    virtual void AddEdge(int from, int to) = 0;

    virtual int VerticesCount() const = 0;

    virtual void GetNextVertices(int vertex, std::vector<int>& vertices) const = 0;
    virtual void GetPrevVertices(int vertex, std::vector<int>& vertices) const = 0;

};

//===============================================================================//
//=====================скопированный файл ListGraph.h============================//

class ListGraph : public IGraph
{
public:

    ListGraph(int _numVertices);

    ListGraph(const IGraph& other);

    ~ListGraph() override;

    void AddEdge(int from, int to) override;

    int VerticesCount() const override;

    void GetNextVertices(int vertex, std::vector<int>& vertices) const override;
    void GetPrevVertices(int vertex, std::vector<int>& vertices) const override;

private:

    std::vector< std::vector<int> > graph;

    int numVertices;
};

//===============================================================================//
//====================скопированный файл ListGraph.cpp===========================//


ListGraph::ListGraph(int _numVertices)
{
    numVertices = _numVertices;
    graph.resize(numVertices);
}

ListGraph::ListGraph(const IGraph& other)
{
    numVertices = other.VerticesCount();
    graph.resize(numVertices);

    for(int i = 0; i < numVertices; ++i){

        std::vector<int> children;
        other.GetNextVertices(i, children);

        for(int j = 0; j < children.size(); ++j){
            AddEdge(i, children[j]);
        }
    }
}

ListGraph::~ListGraph(){}

void ListGraph::AddEdge(int from, int to)
{
    for(int i = 0; i < graph[from].size(); ++i){
        if(graph[from][i] == to){
            return;
        }
    }
    graph[from].push_back(to);
}

int ListGraph::VerticesCount() const
{
    return numVertices;
}

void ListGraph::GetNextVertices(int vertex, std::vector<int>& vertices) const
{
    vertices.resize(0);
    for(int i = 0; i < graph[vertex].size(); ++i){
        vertices.push_back(graph[vertex][i]);
    }
}

void ListGraph::GetPrevVertices(int vertex, std::vector<int>& vertices) const
{
    vertices.resize(0);
    for(int i = 0; i < graph.size(); ++i){
        for(int v : graph[i]){
            if(v == vertex){
                vertices.push_back(i);
            }
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

    bool isBipartite();

    ProblemSolver(const IGraph& _graph) : graph(_graph) {}

    ~ProblemSolver(){};

private:

    const IGraph& graph;

    bool BFS();
};


bool ProblemSolver::BFS()
{
    std::vector<bool> visited(graph.VerticesCount(), false);
    std::vector<bool> color(graph.VerticesCount(), false);

    //  идея алгоритма - покраска в 2 цвета
    //  идем БФСом по всем вершинам(цикл нужен, чтобы пройти все компоненты связанности)
    //  красим первую в первый цвет, ее детей во второй, их детей опять в первый и тд.
    //  если на каком-то этапе встречаем противоречие, по типу хотим покраисть в первый цвет,
    //  а вершина уже покрашена во второй, то граф ьне двудольный, т.к. его вершины не удалось
    //  разбить на 2 группы, так чтобы ребра шли только из одной группы в другую
    for(int i = 0; i < graph.VerticesCount(); ++i){
        if(visited[i]){
            continue;
        }

        //  очередь БФС
        std::queue<int> vertexOrder;
        vertexOrder.push(i);
        visited[i] = true;

        while(!vertexOrder.empty())
        {
            int current = vertexOrder.front();
            vertexOrder.pop();

            std::vector<int> children;
            graph.GetNextVertices(current, children);

            for(auto v : children){
                //  если не посещена - красим и кидаем в очередь
                if(!visited[v]){
                    color[v] = !color[current];
                    vertexOrder.push(v);
                } else {
                //  если посещена - смотрим есть ли противоречие
                    if(color[v] == color[current]){
                        return false;
                    }
                }
                visited[v] = true;
            }
        }
    }
    return true;
}

bool ProblemSolver::isBipartite(){
    return BFS();
}

int main()
{
    int vertexCount, edgesCount;
    std::cin >> vertexCount >> edgesCount;

    std::shared_ptr<IGraph> graph = std::make_shared<ListGraph>(ListGraph(vertexCount));

    for(int i = 0; i < edgesCount; ++i){
        int from, to;
        std::cin >> from >> to;
        graph->AddEdge(from, to);
        graph->AddEdge(to, from);
    }

    ProblemSolver problemSolver(*graph);

    if(problemSolver.isBipartite()){
        std::cout << "YES";
    } else {
        std::cout << "NO";
    }
}
