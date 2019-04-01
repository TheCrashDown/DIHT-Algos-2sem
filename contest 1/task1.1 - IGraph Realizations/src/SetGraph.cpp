#include "../headers/SetGraph.h"

#include <vector>

SetGraph::SetGraph(int _numVertices)
{
    numVertices = _numVertices;
    graph.resize(numVertices);
    reverseGraph.resize(numVertices);
}

SetGraph::SetGraph(const IGraph& other)
{
    numVertices = other.VerticesCount();
    graph.resize(numVertices);
    reverseGraph.resize(numVertices);

    for(int i = 0; i < numVertices; ++i){

        std::vector<int> children;
        other.GetNextVertices(i, children);

        for(int j = 0; j < children.size(); ++j){
            AddEdge(i, children[j]);
        }
    }
}

SetGraph::~SetGraph(){}

void SetGraph::AddEdge(int from, int to)
{
    graph[from].insert(to);
    reverseGraph[to].insert(from);
}

int SetGraph::VerticesCount() const
{
    return numVertices;
}

void SetGraph::GetNextVertices(int vertex, std::vector<int>& vertices) const
{
    vertices.resize(0);
    for(auto i : graph[vertex]){
        vertices.push_back(i);
    }
}

void SetGraph::GetPrevVertices(int vertex, std::vector<int>& vertices) const
{
    vertices.resize(0);
    for(auto i : reverseGraph[vertex]){
        vertices.push_back(i);
    }
}
