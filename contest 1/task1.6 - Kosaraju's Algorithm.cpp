/* Task :
   Дан ориентированный граф. Определите, какое минимальное число ребер
   необходимо добавить, чтобы граф стал сильносвязным. В графе возможны петли.

   developed by Crashdown

*/
#include <iostream>
#include <vector>
#include <stack>
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

    int additionToSCC();

private:

    const IGraph& graph;

    std::shared_ptr<IGraph> graphCondence;

    void DFSStraight(int vertex, std::vector< bool >& visited, std::stack< int >& vertexOrder);

    void DFSReversed(int vertex, std::vector< bool >& visited, std::vector< int >& component, int componentNumber);

    void DFSCondenceBuild(int vertex, std::vector< bool >& visited, std::vector< int >& component);

    void DFSCountSCC(int vertex, std::vector< bool >& visited);

};

//  обход в прямом порядке в алгоритме Косарайю
void ProblemSolver::DFSStraight(int vertex, std::vector< bool >& visited, std::stack< int >& vertexOrder)
{
    visited[vertex] = true;

    std::vector< int > children;
    graph.GetNextVertices(vertex, children);

    for(int v : children){
        if(!visited[v]){
            DFSStraight(v, visited, vertexOrder);
        }
    }
    vertexOrder.push(vertex);
}

//  обход в обратном порядке в алгоритме Косарайю
void ProblemSolver::DFSReversed(int vertex, std::vector< bool >& visited, std::vector< int >& component, int componentNumber)
{
    visited[vertex] = true;

    std::vector< int > parents;
    graph.GetPrevVertices(vertex, parents);

    component[vertex] = componentNumber;

    for(int v : parents){
        if(!visited[v]){
            DFSReversed(v, visited, component, componentNumber);
        }
    }
}

//  построение графа конденсации, имея найденными все КСС
void ProblemSolver::DFSCondenceBuild(int vertex, std::vector< bool >& visited, std::vector< int >& component)
{
    visited[vertex] = true;

    std::vector< int > childern;
    graph.GetNextVertices(vertex, childern);

    for(int v : childern){
        if(component[vertex] != component[v]){
            graphCondence->AddEdge(component[vertex], component[v]);
        }
        if(!visited[v]){
            DFSCondenceBuild(v, visited, component);
        }
    }
}


int ProblemSolver::additionToSCC()
{
    //  алгоритм Косарайю: обходим граф в глубину, запоминаем порядок выхода
    //  в стек vertexOrder, потом обходим граф в обратном направлении в порядке
    //  заданным vertexOrder, все вершины которые запоминаются на очередном этапе - КСС
    std::vector< bool > visited(graph.VerticesCount(), false);
    std::stack< int > vertexOrder;

    for(int i = 0; i < graph.VerticesCount(); ++i){
        if(!visited[i]){
            DFSStraight(i, visited, vertexOrder);
        }
    }

    //  в массиве component поддерживаем в какой компоненте находится
    //  очередная вершина графа
    std::vector< int > component(graph.VerticesCount());
    visited.resize(0);
    visited.resize(graph.VerticesCount(), false);

    //  поддержка количества КСС в исходном графе (= вершин в конденсе)
    int componentNumber = 0;

    while(!vertexOrder.empty()){
        int v = vertexOrder.top();
        vertexOrder.pop();

        if(!visited[v]){
            DFSReversed(v, visited, component, componentNumber);
            componentNumber++;
        }
    }

    //  досрочный выход, если в конденсе всего одна вершина -> исходный граф сильносвязан
    if(componentNumber == 1){
        return 0;
    }


    //  постороение графа конденсации обходом в глубину:
    //  смотрим какие ребра есть в исходном и добавляем ребро между
    //  соответствующими компонентами в конденсе
    graphCondence = std::make_shared<ListGraph>(ListGraph(componentNumber));

    visited.resize(0);
    visited.resize(graph.VerticesCount(), false);

    for(int v = 0; v < graph.VerticesCount(); ++v){
        if(!visited[v]){
            DFSCondenceBuild(v, visited, component);
        }
    }


    //  после построения конденса будем искать ответ на задачу:
    //  посчитаем количества вершин, которые:
    //      не имеют предка - outgoing
    //      не имеют детей - recieving
    //  ответом на задачу будет максимум из этих величин - для каждой
    //  такой вершины сюръективно добавляем ребро в вершины другого типа. при этом граф станосится
    //  сильносвязанным, а меньшим количеством ребер обойтись нельзя, т.к. останутся вершины
    //  со стрелками только в одну сторону
    int outgoingVerticies = 0;
    int recievingVerticies = 0;

    for(int i = 0; i < graphCondence->VerticesCount(); ++i){
        std::vector< int > v;

        bool hasChild = false;
        bool hasParent = false;

        graphCondence->GetNextVertices(i, v);
        if(v.size() != 0){
            hasChild = true;
        }
        graphCondence->GetPrevVertices(i, v);
        if(v.size() != 0){
            hasParent = true;
        }

        if(hasChild && !hasParent){
            outgoingVerticies++;
        }
        if(!hasChild && hasParent){
            recievingVerticies++;
        }
        if(!hasChild && !hasParent){
            outgoingVerticies++;
            recievingVerticies++;
        }
    }

    return std::max(outgoingVerticies, recievingVerticies);
}



int main()
{
    int vertexCount, edgesCount;
    std::cin >> vertexCount >> edgesCount;

    std::shared_ptr<IGraph> graph = std::make_shared<ListGraph>(ListGraph(vertexCount));

    for(int i = 0; i < edgesCount; ++i){
        int from, to;
        std::cin >> from >> to;
        graph->AddEdge(from - 1, to - 1);
    }

    ProblemSolver problemSolver(*graph);

    std::cout << problemSolver.additionToSCC();

    return 0;
}
