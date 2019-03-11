#include "../headers/ArcGraph.h"

#include <vector>

ArcGraph::ArcGraph(int _numVertices)
{
    numVertices = _numVertices;
}

ArcGraph::ArcGraph(const IGraph& other)
{
    numVertices = other.VerticesCount();

    for(int i = 0; i < numVertices; ++i){

        std::vector<int> children;
        other.GetNextVertices(i, children);

        for(int j = 0; j < children.size(); ++j){
            AddEdge(i, children[j]);
        }
    }
}

ArcGraph::~ArcGraph(){}

void ArcGraph::AddEdge(int from, int to)
{
    graph.push_back(std::make_pair(from, to));
}

int ArcGraph::VerticesCount() const
{
    return numVertices;
}

void ArcGraph::GetNextVertices(int vertex, std::vector<int>& vertices) const
{
    vertices.resize(0);
    for(int i = 0; i < graph.size(); ++i){
        if(graph[i].first == vertex){
            vertices.push_back(graph[i].second);
        }
    }
}

void ArcGraph::GetPrevVertices(int vertex, std::vector<int>& vertices) const
{
    vertices.resize(0);
    for(int i = 0; i < graph.size(); ++i){
        if(graph[i].second == vertex){
            vertices.push_back(graph[i].first);
        }
    }
}
