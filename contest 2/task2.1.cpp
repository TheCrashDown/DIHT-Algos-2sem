/* Task :
   Требуется отыскать самый короткий маршрут между городами. 
   Из города может выходить дорога, которая возвращается в этот же город.
   Требуемое время работы O((N + M)log N), где N – количество городов, 
   M – известных дорог между ними.
   N ≤ 10000, M ≤ 250000.
   Длина каждой дороги ≤ 10000.

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

    virtual void AddEdge(int from, int to, int weight) = 0;

    virtual int VerticesCount() const = 0;

    virtual void GetNextVertices(int vertex, std::vector< std::pair<int, int> >& vertices) const = 0;
    virtual void GetPrevVertices(int vertex, std::vector< std::pair<int, int> >& vertices) const = 0;

};

//===============================================================================//
//=====================скопированный файл ListGraph.h============================//

class ListGraph : public AdvancedIGraph
{

public:

    ListGraph(int _numVertices);

    ListGraph(const AdvancedIGraph& other);

    ~ListGraph() override;

    void AddEdge(int from, int to, int weight) override;

    int VerticesCount() const override;

    void GetNextVertices(int vertex, std::vector< std::pair<int, int> >& vertices) const override;
    void GetPrevVertices(int vertex, std::vector< std::pair<int, int> >& vertices) const override;


private:

    std::vector< std::vector< std::pair<int, int> > > graph;

    int numVertices;

};

//===============================================================================//
//====================скопированный файл ListGraph.cpp===========================//


ListGraph::ListGraph(int _numVertices)
{
    numVertices = _numVertices;
    graph.resize(numVertices);
}

ListGraph::ListGraph(const AdvancedIGraph& other)
{
    numVertices = other.VerticesCount();
    graph.resize(numVertices);

    for(int i = 0; i < numVertices; ++i){

        std::vector< std::pair<int, int> > children;
        other.GetNextVertices(i, children);

        for(int j = 0; j < children.size(); ++j){
            AddEdge(i, children[j].first, children[j].second);
        }
    }
}

ListGraph::~ListGraph(){}

void ListGraph::AddEdge(int from, int to, int weight)
{
    graph[from].push_back(std::make_pair(to, weight));
}

int ListGraph::VerticesCount() const
{
    return numVertices;
}

void ListGraph::GetNextVertices(int vertex, std::vector< std::pair<int, int> >& vertices) const
{
    vertices.resize(0);
    for(int i = 0; i < graph[vertex].size(); ++i){
        vertices.push_back(graph[vertex][i]);
    }
}

void ListGraph::GetPrevVertices(int vertex, std::vector< std::pair<int, int> >& vertices) const
{
    vertices.resize(0);
    for(int i = 0; i < graph.size(); ++i){
        for(auto point : graph[i]){
            if(point.first == vertex){
                vertices.push_back(std::make_pair(i, point.second));
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

    int findMinPath(int vFirst, int vSecond);

    ProblemSolver(const AdvancedIGraph& _graph) : graph(_graph) {}

    ~ProblemSolver(){};

private:

    const AdvancedIGraph& graph;

    int dijkstra(int vFirst, int vSecond);


    //  вспомогательная структура для приоритетной очереди 
    struct Point
    {
        int vertex;

        int length;

        Point(){}

        Point(int _vertex, int _length): vertex(_vertex), length(_length) {}
    };

    //  оператор сравнения для очереди с приоритетом, чтобы
    //  наверх поднимались элементы с минимальным расстоянием
    class PointCompare
    {
    public:
        bool operator() (const Point& a, const Point& b){
            return b.length < a.length;
        }
    };
};

int ProblemSolver::dijkstra(int vFirst, int vSecond)
{
    //  решение задачи с использованием алгоритма Дейкстры
    std::vector< int > dp(graph.VerticesCount(), INT_MAX);
    std::vector< bool > visited(graph.VerticesCount(), false);
    dp[vFirst] = 0;

    std::priority_queue< Point, std::vector< Point>, PointCompare > order;
    order.push(Point(vFirst, 0));

    while(!order.empty()){

        Point current = order.top();
        order.pop();

        if(current.length > dp[current.vertex]){
            continue;
        }

        //  находим и перебираем всех детей вершины
        std::vector< std::pair<int, int> > children;
        graph.GetNextVertices(current.vertex, children);

        for(auto v : children){
            int childVertex = v.first;
            int childLength = v.second;
 
            //  если расстояния можно обновить более оптимальными, делаем это 
            //  и закидываем их в очередь, чтобы обновить(если надо) их детей,
            //  т.к для них теперь возможно есть более оптимальный путь
            if(dp[current.vertex] + childLength < dp[childVertex]){
                dp[childVertex] = dp[current.vertex] + childLength;
                order.push(Point(childVertex, dp[childVertex]));
            }
        }
    }

    return dp[vSecond];
}

int ProblemSolver::findMinPath(int vFirst, int vSecond)
{
    return dijkstra(vFirst, vSecond);
}

int main()
{
    int n, m;
    std::cin >> n >> m;

    AdvancedIGraph* graph = new ListGraph(n);

    for(int i = 0; i < m; ++i){
        int from, to, weight;
        std::cin >> from >> to >> weight;
        graph->AddEdge(from, to, weight);
        graph->AddEdge(to, from, weight);
    }

    int from, to;

    std::cin >> from >> to;

    ProblemSolver problemSolver(*graph);

    std::cout << problemSolver.findMinPath(from, to);

    return 0;
}
