#include "../headers/ListGraph.h"

#include <vector>

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
