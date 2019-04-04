/* Task :
   Полный ориентированный взвешенный граф задан матрицей смежности. 
   Постройте матрицу кратчайших путей между его вершинами. 
   Гарантируется, что в графе нет циклов отрицательного веса.

   developed by Crashdown

*/
#include <iostream>
#include <vector>

void floyd(int n, std::vector< std::vector<int> >& graph)
{
    //  алгоритм Флойда для данной матрицы смежности
    for(int k = 0; k < n; ++k){
        for(int i = 0; i < n; ++i){
            for(int j = 0; j < n; ++j){
                graph[i][j] = std::min(graph[i][j], graph[i][k] + graph[k][j]);
            }
        }
    }
}


int main()
{
    int n = 0;
    std::cin >> n;
    std::vector< std::vector<int> > graph(n);

    for(int i = 0; i < n; ++i){
        graph[i].resize(n);
    }

    for(int i = 0; i < n; ++i){
        for(int j = 0; j < n; ++j){
            std::cin >> graph[i][j];
        }
    }

    floyd(n, graph);

    for(int i = 0; i < n; ++i){
        for(int j = 0; j < n; ++j){
            std::cout << graph[i][j] << " ";
        }
        std::cout << "\n";
    }

    return 0;
}
