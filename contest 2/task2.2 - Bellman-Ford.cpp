/* Task :
   Необходимо написать торгового советника для поиска арбитража.
   Арбитраж - это торговля по цепочке различных валют в надежде заработать на
   небольших различиях в коэффициентах. Например, есть следующие курсы валют:
   GBP/USD: 0.67
   RUB/GBP: 78.66
   USD/RUB: 0.02
   Имея 1$ и совершив цикл USD->GBP->RUB->USD, получим 1.054$. Таким образом заработав 5.4

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

    virtual void AddEdge(int from, int to, double weight) = 0;

    virtual int VerticesCount() const = 0;

    virtual void GetNextVertices(int vertex, std::vector< std::pair<int, double> >& vertices) const = 0;
    virtual void GetPrevVertices(int vertex, std::vector< std::pair<int, double> >& vertices) const = 0;

};

//===============================================================================//
//=====================скопированный файл ListGraph.h============================//

class ListGraph : public AdvancedIGraph
{

public:

    ListGraph(int _numVertices);

    ListGraph(const AdvancedIGraph& other);

    ~ListGraph() override;

    void AddEdge(int from, int to, double weight) override;

    int VerticesCount() const override;

    void GetNextVertices(int vertex, std::vector< std::pair<int, double> >& vertices) const override;
    void GetPrevVertices(int vertex, std::vector< std::pair<int, double> >& vertices) const override;


private:

    std::vector< std::vector< std::pair<int, double> > > graph;

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

        std::vector< std::pair<int, double> > children;
        other.GetNextVertices(i, children);

        for(int j = 0; j < children.size(); ++j){
            AddEdge(i, children[j].first, children[j].second);
        }
    }
}

ListGraph::~ListGraph(){}

void ListGraph::AddEdge(int from, int to, double weight)
{
    graph[from].push_back(std::make_pair(to, weight));
}

int ListGraph::VerticesCount() const
{
    return numVertices;
}

void ListGraph::GetNextVertices(int vertex, std::vector< std::pair<int, double> >& vertices) const
{
    vertices.resize(0);
    for(int i = 0; i < graph[vertex].size(); ++i){
        vertices.push_back(graph[vertex][i]);
    }
}

void ListGraph::GetPrevVertices(int vertex, std::vector< std::pair<int, double> >& vertices) const
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

    bool arbitrageExist();

    ProblemSolver(const AdvancedIGraph& _graph) : graph(_graph) {}

    ~ProblemSolver(){};

private:

    const AdvancedIGraph& graph;

    bool bellmanFord(int vStart, std::vector< bool >& visited);

    struct Point
    {
        int vertex;

        int length;

        Point(){}

        Point(int _vertex, int _length): vertex(_vertex), length(_length) {}
    };
};

bool ProblemSolver::arbitrageExist(){
    //  запускаем алгоритм Форда-Беллмана от каждой еще не посещенной
    //  вершины - посещать повторно не имеет смысла, т.к. если бы в них
    //  были арбитражи, они были бы найдены на предыдущих итерациях.
    //  запуск от всех непосещенных требуется, чтобы учесть все КСС, и при
    //  этом не нарушает асимптотику - если у нас осталась непосещенная КСС, то
    //  это значит, что она не обрабатывалась ранее, и таким образом оценка сложности
    //  алгоритма Форда-Беллмана для всего графа сохраняется, она просто применена по очереди
    //  ко всем КСС
    std::vector< bool > visited(graph.VerticesCount(), false);

    for(int i = 0; i < graph.VerticesCount(); ++i){
        if(!visited[i]){
            if(bellmanFord(i, visited)){
                return true;
            }
        }
    }
    return false;
}

bool ProblemSolver::bellmanFord(int vStart, std::vector< bool >& visited){

    //  поскольку мы работаем с коэфициентами арбитража, а не с реальными весами
    //  ребер, то сделаем следующие модификации:
    //      1. начальное значение - не INF, а 0, на него проверяем отдельно
    //      2. не складываем пути, а перемножаем коэффициенты
    //      3. нейтральное значение по умножению (значение на стартовой вершине) - 1.0
    //      (аналогично в классическом алгоритме для сложения оно было равным 0)
    std::vector< double > dp(graph.VerticesCount(), -1.0);

    dp[vStart] = 1.0;
    visited[vStart] = true;


    //  делаем n - 1 итерацию по улучшению веса. алгоритм гарантирует, что
    //  в отсутствии отрицательных циклов этого хватит, чтобы
    //  полностью соптимизировать все веса
    for(int i = 0; i < graph.VerticesCount() - 1; ++i){
        for(int vertex = 0; vertex < graph.VerticesCount(); ++vertex){
            std::vector< std::pair< int, double > > children;
            graph.GetPrevVertices(vertex, children);
            for(auto u : children){
                int childVertex = u.first;
                double childWeight = u.second;
                if(dp[childVertex] != -1.0 && dp[vertex] < dp[childVertex] * childWeight){
                    //int flag = dp[vertex];
                    dp[vertex] = dp[childVertex] * childWeight;
                    //flag = dp[vertex];
                    visited[vertex] = true;
                }
            }
        }
    }

    //  пробуем сделать еще 1 итерацию. если получилось сделать еще одно улучшение -
    //  значит, есть отрицательный цикл, т.к. за n - 1 итерацию оптимальное решение не было найдено
    for(int vertex = 0; vertex < graph.VerticesCount(); ++vertex){
        std::vector< std::pair< int, double > > children;
        graph.GetPrevVertices(vertex, children);
        for(auto u : children){
            int childVertex = u.first;
            double childWeight = u.second;
            if(dp[childVertex] != -1.0 && dp[vertex] < dp[childVertex] * childWeight){
                return true;
            }
        }
    }
    return false;
}



int main()
{
    int n;
    std::cin >> n;

    AdvancedIGraph* graph = new ListGraph(n);

    for(int i = 0; i < n; ++i){
        for(int j = 0; j < n; ++j){
            if(i == j){
                continue;
            }
            double tmp;
            std::cin >> tmp;
            if(tmp != -1.0){
                graph->AddEdge(i, j, tmp);
            }
        }
    }

    ProblemSolver problemSolver(*graph);

    if(problemSolver.arbitrageExist()){
        std::cout << "YES";
    } else {
        std::cout << "NO";
    }

    return 0;
}
