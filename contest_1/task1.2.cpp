/* Task :
   Дан невзвешенный неориентированный граф. Найдите цикл минимальной длины.

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

    int findMinCycle();

    ProblemSolver(const IGraph& _graph) : graph(_graph) {}

    ~ProblemSolver(){};

private:

    const IGraph& graph;

    int BFS();

    //  вспомогательная структура с данными о вершинах
    //  prev - предок
    //  depth - глубина с точки зрения БФС относительно стартовой вершины
    struct Point
    {
        int prev;

        int depth;

        Point(){}

        Point(int _prev, int _depth) :
            prev(_prev),
            depth(_depth) {}
    };
};


int ProblemSolver::BFS(){

    //  поддержка текущего минимального цикла
    int minCycle = graph.VerticesCount() + 1;

    //  запускаем обход в ширину от всех вершин, прерываемся, когда нашли первый цикл
    //  этот цикл будет минимальным от данной вершины (очевидно из принципа работы БФС)
    for(int i = 0; i < graph.VerticesCount(); ++i){

        //  информация о вершинах - факт посещения, предыдущая, глубина
        std::vector<Point> data(graph.VerticesCount());
        std::vector<bool> visited(graph.VerticesCount(), false);

        //  очередь БФС
        std::queue< int > vertexOrder;
        vertexOrder.push(i);
        data[i].depth = 0;

        bool cycleFound = false;

        visited[i] = true;
        int currentCycle = minCycle;

        while(!vertexOrder.empty())
        {
            int currentVertex = vertexOrder.front();
            vertexOrder.pop();

            std::vector<int> children;
            graph.GetNextVertices(currentVertex, children);

            for(auto v : children){
                //  если уже посещена - то это искомый цикл, если только это не
                //  повторное рассмотрение предка текущей вершины
                if(visited[v]){
                    if(v != data[currentVertex].prev){
                        currentCycle = data[currentVertex].depth + data[v].depth + 1;
                        cycleFound = true;
                        break;
                    }
                }

                //  если не посещена - закидываем ее в очередь
                if(!visited[v]){
                    visited[v] = true;
                    vertexOrder.push(v);
                    data[v].prev = currentVertex;
                    data[v].depth = data[currentVertex].depth + 1;
                }
            }

            if(currentCycle < minCycle){
                minCycle = currentCycle;
            }

            if(cycleFound){
                break;
            }
        }
    }
    //  контрольная проверка, что цикл вообще есть
    return (minCycle == graph.VerticesCount() + 1) ? -1 : minCycle;
}

int ProblemSolver::findMinCycle(){
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

    std::cout << problemSolver.findMinCycle();

    return 0;
}
