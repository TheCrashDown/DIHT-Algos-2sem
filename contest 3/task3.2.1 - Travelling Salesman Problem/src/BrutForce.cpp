#include "../headers/BrutForce.h"


BrutForce::BrutForce(const AdvancedIGraph& _graph) : graph(_graph){
    path.resize(0);

    //  начинаем с 1, т.к. будем все пути по дефолту начинать с 0 вершины
    for(int i = 1; i < graph.VerticesCount(); ++i)
    {
        path.push_back(i);
    }
}


BrutForce::~BrutForce(){}


double BrutForce::findMinPath(){

    pathfinder(1);

    return currMin;
}

//  рекурсивная функция для получения всех путей и online проверки
//  на минимальность
void BrutForce::pathfinder(int depth){
    //  условие выхода
    if(depth == path.size()){
        currMin = std::min(currMin, getPathLength());
        return;
    }

    for(int i = 0; i <= depth; ++i)
    {
        //  поочередно подставляем на место depth
        //  возможные оставшиеся вершины
        std::swap(path[i], path[depth]);

        pathfinder(depth + 1);

        std::swap(path[i], path[depth]);
    }
}

double BrutForce::getPathLength(){
    //  в пути есть такие ребра :
    //  (напоминаю, что в path нулевая не хранится,
    //  необходимый костыль чтобы бесконечного перебора не было)))))
    //  от 0 до path[0]
    //  от path[i] до path[i+1] для каждого i от 0 до n-1
    //  от 0 до path[n-1]
    double tmp = 0.0;
    for (int i = 0; i < graph.VerticesCount() - 2; ++i)
    {
        tmp += graph.getEdge(path[i], path[i + 1]);
    }
    tmp += graph.getEdge(0, path[0]);
    tmp += graph.getEdge(0, path[graph.VerticesCount() - 2]);

    return tmp;
}
