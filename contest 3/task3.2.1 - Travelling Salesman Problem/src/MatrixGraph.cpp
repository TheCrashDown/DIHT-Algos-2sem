#include "../headers/MatrixGraph.h"

#include <vector>

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

        std::vector< std::pair<int, double> > children;
        other.GetNextVertices(i, children);

        for(int j = 0; j < children.size(); ++j){
            setEdge(i, children[j].first, children[j].second);
        }
    }
}

void MatrixGraph::setEdge(int from, int to, double weight)
{
    graph[from][to] = weight;
}

double MatrixGraph::getEdge(int from, int to) const
{
    return graph[from][to];
}

int MatrixGraph::VerticesCount() const
{
    return numVertices;
}

void MatrixGraph::GetNextVertices(int vertex, std::vector< std::pair<int, double> >& vertices) const
{
    vertices.resize(0);
    for(int i = 0; i < numVertices; ++i){
        if(graph[vertex][i] != 0){
            vertices.push_back(std::make_pair(i, graph[vertex][i]));
        }
    }
}

void MatrixGraph::GetPrevVertices(int vertex, std::vector< std::pair<int, double> >& vertices) const
{
    vertices.resize(0);
    for(int i = 0; i < numVertices; ++i){
        if(graph[i][vertex] != 0){
            vertices.push_back(std::make_pair(i, graph[i][vertex]));
        }
    }
}
