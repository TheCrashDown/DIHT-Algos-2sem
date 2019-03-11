/* Task :
   Дан невзвешенный неориентированный граф. В графе может быть несколько
   кратчайших путей между какими-то вершинами. Найдите количество
   различных кратчайших путей между заданными вершинами.
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

    int getNumPaths(int vFirst, int vSecond);

    ProblemSolver(const IGraph& _graph) : graph(_graph) {}

    ~ProblemSolver(){};

private:

    const IGraph& graph;

    int BFS(int vFirst, int vSecond);

    //  вспомогательная структура с данными о вершинах
    //  numPaths - количество путей до вершины минимальной длины
    //  depth - глубина с точки зрения БФС относительно первой вершины
    struct Point
    {
        int numPaths = -1;

        int depth = 0;

        Point(){}

        Point(int _numPaths, int _depth) :
            numPaths(_numPaths),
            depth(_depth) {}
    };
};


//  идея алгоритма - динамика по врешинам
//  обходим их БФСом, т.е. ближайшие к стартовой обходятся раньше
//  дальше при прохождении следующих количество кратчайших путей до них
//  складываем из количеств этих самых путей для ее вершин-предков
int ProblemSolver::BFS(int vFirst, int vSecond)
{
    std::vector< Point > data(graph.VerticesCount());

    //  очередь БФС
    std::queue< int > vertexOrder;
    vertexOrder.push(vFirst);

    data[vFirst].numPaths = 1;
    data[vFirst].depth = 0;

    while(!vertexOrder.empty())
    {
        int currentVertex = vertexOrder.front();
        vertexOrder.pop();

        std::vector<int> children;
        graph.GetNextVertices(currentVertex, children);

        for(auto v : children){
            //  если еще не посещена - добавляем ее в очередь и записываем ей пути
            if(data[v].numPaths == -1){
                vertexOrder.push(v);
                data[v].numPaths = data[currentVertex].numPaths;
                data[v].depth = data[currentVertex].depth + 1;
                continue;
            }
            //  если уже посещена, то добавляем ей новые найденные пути, если они кратчайшие
            if(data[v].depth == data[currentVertex].depth + 1){
                data[v].numPaths += data[currentVertex].numPaths;
            }
        }
    }
    return data[vSecond].numPaths;
}

int ProblemSolver::getNumPaths(int vFirst, int vSecond){
    int ans = BFS(vFirst, vSecond);
    if(vFirst == vSecond){
        return 0;
    }
    if(ans == -1){
        return 0;
    }
    return ans;
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

    int vFirst, vSecond;

    std::cin >> vFirst >> vSecond;

    ProblemSolver problemSolver(*graph);

    std::cout << problemSolver.getNumPaths(vFirst, vSecond);

    return 0;
}
