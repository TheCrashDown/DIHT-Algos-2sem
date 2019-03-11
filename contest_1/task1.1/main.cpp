#include "headers/ListGraph.h"
#include "headers/MatrixGraph.h"
#include "headers/SetGraph.h"
#include "headers/ArcGraph.h"

#include <iostream>
#include <vector>


int main()
{
    IGraph* graph = new ListGraph(6);

    //ListGraph graph(6);

    graph->AddEdge(1,2);
    graph->AddEdge(1,4);
    graph->AddEdge(4,2);

    //IGraph* graph(graph1);


    std::cout << graph->VerticesCount() << "\n";

    std::vector<int> q;

    graph->GetNextVertices(1, q);

    for(int i = 0; i < q.size(); ++i){
        std::cout << q[i] << " ";
    }

    graph->GetPrevVertices(2, q);

    for(int i = 0; i < q.size(); ++i){
        std::cout << q[i] << " ";
    }


    return 0;
}
