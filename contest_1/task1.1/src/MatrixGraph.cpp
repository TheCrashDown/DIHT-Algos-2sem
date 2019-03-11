#include "../headers/MatrixGraph.h"

#include <vector>

MatrixGraph::MatrixGraph(int _numVertices)
{
    numVertices = _numVertices;
    graph.resize(numVertices);

    for(int i = 0; i < numVertices; ++i){
        graph[i].resize(numVertices, false);
    }
}

MatrixGraph::MatrixGraph(const IGraph& other){

    numVertices = other.VerticesCount();
    graph.resize(numVertices);

    for(int i = 0; i < numVertices; ++i){
        graph[i].resize(numVertices, false);
    }

    for(int i = 0; i < numVertices; ++i){

        std::vector<int> children;
        other.GetNextVertices(i, children);

        for(int j = 0; j < children.size(); ++j){
            AddEdge(i, children[j]);
        }
    }

}

MatrixGraph::~MatrixGraph(){}

void MatrixGraph::AddEdge(int from, int to)
{
    graph[from][to] = true;
}

int MatrixGraph::VerticesCount() const
{
    return numVertices;
}

void MatrixGraph::GetNextVertices(int vertex, std::vector<int>& vertices) const
{
    vertices.resize(0);
    for(int i = 0; i < numVertices; ++i){
        if(graph[vertex][i] == true){
            vertices.push_back(i);
        }
    }

}

void MatrixGraph::GetPrevVertices(int vertex, std::vector<int>& vertices) const
{
    vertices.resize(0);
    for(int i = 0; i < numVertices; ++i){
        if(graph[i][vertex] == true){
            vertices.push_back(i);
        }
    }
}
