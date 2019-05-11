/* Task: 
   В одном из отделов крупной организации работает n человек. Как практически все сотрудники этой организации, 
   они любят пить чай в перерывах между работой. При этом они достаточно дисциплинированы и делают в день ровно 
   один перерыв, во время которого пьют чай. Для того, чтобы этот перерыв был максимально приятным, каждый из сотрудников 
   этого отдела обязательно пьет чай одного из своих любимых сортов. В разные дни сотрудник может пить чай разных сортов. 
   Для удобства пронумеруем сорта чая числами от 1 до m. Недавно сотрудники отдела купили себе большой набор чайных 
   пакетиков, который содержит a1 пакетиков чая сорта номер 1, a2 пакетиков чая сорта номер 2, ..., am пакетиков 
   чая сорта номер m. Теперь они хотят знать, на какое максимальное число дней им может хватить купленного набора так, 
   чтобы в каждый из дней каждому из сотрудников доставался пакетик чая одного из его любимых сортов. 
   Каждый сотрудник отдела пьет в день ровно одну чашку чая, которую заваривает из одного пакетика. 
   При этом пакетики чая не завариваются повторно. 
   
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


void readAndMakeGraph(int n, int m, AdvancedIGraph*& graph, int& summ_tea){
    //  Формирование графа:
    //  
    //  0               - фиктивный исток
    //  1..n            - сотрудники
    //  n + 1..n + m    - сорты чая
    //  m + n + 1       - фиктивный сток
    
    for(int i = 0; i < m; ++i)
    {
        int tmp = 0;
        std::cin >> tmp;

        summ_tea += tmp;

        //  ребра от чаев к стоку с пропускной способностью = кол-ву таких пакетиков
        graph->setEdge(n + 1 + i, m + n + 1, tmp);
    }


    for(int i = 0; i < n; ++i){
        int k = 0;
        std::cin >> k;

        for(int j = 0; j < k; ++j){
            int tmp = 0;
            std::cin >> tmp;

            //  ребра от сотрудников к чаям которые они любят
            graph->setEdge(i + 1, n + tmp, INT_MAX);
        }
    }
}



int main()
{
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);

    int n, m;
    std::cin >> n >> m;

    AdvancedIGraph* graph = new MatrixGraph(n + m + 2);

    int summ_tea = 0;

    readAndMakeGraph(n, m, graph, summ_tea);


    //  бинарный поиск по дням
    //  левая граница - 0
    //  правая граница - summ_tea - суммарное число пакетиков в начале
    int left = 0;
    int right = summ_tea + 1;

    int days = (left + right) / 2;


    while(true){

        days = (left + right) / 2;

        for(int i = 1; i <= n; ++i){
            //  ребра от истока к сотрудникам с пропускной способностью = текущее 
            //  предположение количества дней
            graph->setEdge(0, i, days);
        }

        ProblemSolver problemSolver(*graph);

        int flow = problemSolver.findMaxFlow(0, n + m + 1);

        //  условие возможности данного количества дней - выходной поток равен входному
        if(flow == days * n){
            left = days;
        } else {
            right = days;
        }

        //  условие выхода из цикла - оптимальное решение перестало обновляться
        if(days == (left + right) / 2){
            break;
        }

    }

    std::cout << days;

    return 0;
}
