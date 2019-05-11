#include "../headers/MSTCreator.h"

#include <climits>
#include <vector>
#include <queue>

MSTCreator::MSTCreator(const AdvancedIGraph& _graph) : graph(_graph) {}

MSTCreator::~MSTCreator(){}

struct MSTCreator::Point
{
    int vertex;

    int priority;

    int parent;

    Point(){}

    Point(int _vertex, int _priority, int _parent): vertex(_vertex),
                                                    priority(_priority),
                                                    parent(_parent) {}

};

class MSTCreator::PointCompare
{
public:
    bool operator() (const Point& a, const Point& b){
        return b.priority < a.priority;
    }
};


int MSTCreator::findMinSpanningTree(std::vector< std::pair<int, int> >& path){
    path.resize(0);
    return Prim(path);
}

//  алгоритм из первой задачи, который теперь еще и восстанавливает путь
int MSTCreator::Prim(std::vector< std::pair<int, int> >& path){
    std::vector< bool > visited(graph.VerticesCount(), false);

    std::vector< int > dp(graph.VerticesCount(), INT_MAX);

    std::priority_queue< Point, std::vector< Point >, PointCompare > order;
    order.push(Point(0, 0, -2));

    int pathLength = 0;

    while(!order.empty()){
        Point current = order.top();
        order.pop();
        if(!visited[current.vertex]){

            visited[current.vertex] = true;

            pathLength += current.priority;

            if(current.parent >= 0){
                path.push_back(std::make_pair(current.vertex, current.parent));
            }

            std::vector< std::pair< int, double > > children;
            graph.GetNextVertices(current.vertex, children);

            for(auto child : children){
                int childVertex = child.first;
                int childWeight = child.second;

                if(dp[childVertex] > childWeight){

                    dp[childVertex] = childWeight;

                    if(!visited[childVertex]){
                        order.push(Point(childVertex, childWeight, current.vertex));
                    }
                }
            }
        }
    }


    return pathLength;
}
