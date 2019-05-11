/* Task:
   Дан неориентированный связный граф. Требуется найти вес минимального остовного дерева в этом графе. 
   Решить с помощью алгортима Прима.

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

    int findMinSpanningTree();

    ProblemSolver(const AdvancedIGraph& _graph) : graph(_graph) {}

    ~ProblemSolver(){};

private:

    const AdvancedIGraph& graph;

    int Prim();

    struct Point
    {
        int vertex;

        int priority;

        Point(){}

        Point(int _vertex, int _priority): vertex(_vertex), priority(_priority) {}
    };

    //  компаратор для очереди с приоритетом
    class PointCompare
    {
    public:
        bool operator() (const Point& a, const Point& b){
            return b.priority < a.priority;
        }
    };
};


int ProblemSolver::findMinSpanningTree(){
    return Prim();
}

int ProblemSolver::Prim(){
    std::vector< bool > visited(graph.VerticesCount(), false);
    std::vector< int > dp(graph.VerticesCount(), INT_MAX);

    std::priority_queue< Point, std::vector< Point >, PointCompare > order;
    order.push(Point(0, 0));

    int pathLength = 0;

    while(!order.empty()){
        Point current = order.top();
        order.pop();
        if(visited[current.vertex]){
            continue;
        }
        visited[current.vertex] = true;

        pathLength += current.priority;

        std::vector< std::pair< int, int > > children;
        graph.GetNextVertices(current.vertex, children);

        for(auto child : children){
            int childVertex = child.first;
            int childWeight = child.second;

            if(dp[childVertex] > childWeight){

                dp[childVertex] = childWeight;

                if(!visited[childVertex]){
                    order.push(Point(childVertex, childWeight));
                }
            }
        }
    }

    return pathLength;
}



int main()
{
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);

    int n, m;
    std::cin >> n >> m;

    AdvancedIGraph* graph = new ListGraph(n);

    for(int i = 0; i < m; ++i){
        int from, to, weight;
        std::cin >> from >> to >> weight;
        graph->AddEdge(from - 1, to - 1, weight);
        graph->AddEdge(to - 1, from - 1, weight);
    }

    ProblemSolver problemSolver(*graph);

    std::cout << problemSolver.findMinSpanningTree();

    return 0;
}
