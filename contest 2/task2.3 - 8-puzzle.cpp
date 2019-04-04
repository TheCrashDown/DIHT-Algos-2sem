/* Task :
   «Восьминашки» – упрощенный вариант известной головоломки «Пятнашки». 
   Восемь костяшек, пронумерованных от 1 до 8, расставлены по ячейкам игровой доски 3 на 3, 
   одна ячейка при этом остается пустой. За один ход разрешается передвинуть одну из костяшек, 
   расположенных рядом с пустой ячейкой, на свободное место. Цель игры – для заданной начальной конфигурации 
   игровой доски за минимальное число ходов получить выигрышную конфигурацию (пустая ячейка обозначена нулем):

   1 2 3
   4 5 6
   7 8 0

   developed by Crashdown

*/
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <set>
#include <map>


class ProblemSolver
{
public:
    ProblemSolver(int _data);
    ~ProblemSolver() {}

    int findMinPath(std::vector< char >& path);

private:

    //  состояние хранится как шестнадцатиричное число, каждая цифра которого обозначает 
    //  значение в соответствующей ячейке
    long long data = 0;
    int posSpace = 0;

    const long long AIM_STATE = 0x123456780;

    //  маски для быстрого изменения / получения i-й цифры шестнадцатиричного числа
    const unsigned long long masksGet[9] = {0xF00000000, 0x0F0000000, 0x00F000000, 0x000F00000,
                                            0x0000F0000, 0x00000F000, 0x000000F00, 0x0000000F0,
                                            0x00000000F};

    const unsigned long long masksSet[9] = {0x0FFFFFFFF, 0xF0FFFFFFF, 0xFF0FFFFFF, 0xFFF0FFFFF,
                                            0xFFFF0FFFF, 0xFFFFF0FFF, 0xFFFFFF0FF, 0xFFFFFFF0F,
                                            0xFFFFFFFF0};


    void getChildren(long long state, std::vector< std::pair< long long, char > >& children);

    int getPosSpace(long long state);

    void moveUp(long long& state);
    void moveDown(long long& state);
    void moveLeft(long long& state);
    void moveRight(long long& state);

    void setState(long long& state, int pos, int value);
    int getState(long long state, int pos);

    bool possible();

    int AStar(std::vector< char >& path);

    int heuristic(long long state);

    struct Point
    {
        long long state;

        int priority;

        int vNum;

        Point(long long _state, int _priority, int _vNum) :
                state(_state), priority(_priority),
                vNum(_vNum){}

    };

    class PointCompare
    {
    public:
        bool operator() (const Point& a, const Point& b){
            return b.priority < a.priority;
        }
    };
};


ProblemSolver::ProblemSolver(int _data){
    int tmp = _data;
    for(int i = 8; i >= 0; --i){
        if(tmp % 10 == 0){
            posSpace = i;
        }
        setState(data, i, tmp % 10);
        tmp /= 10;
    }
}

int ProblemSolver::getPosSpace(long long state){
    //  метод получает текущую позицию пустой ячейки
    for(int i = 0; i < 9; ++i){
        if(getState(state, i) == 0){
            return i;
        }
    }
}

void ProblemSolver::getChildren(long long state, std::vector< std::pair< long long, char > >& children){
    //  метод для состояния state получает все состояния, в которые можно из него попасть за 1 движение
    int flag = getPosSpace(state);
    if(getPosSpace(state) > 2){
        long long tmp = state;
        moveUp(tmp);
        children.push_back(std::make_pair(tmp, 'U'));
    }

    if(getPosSpace(state) < 6){
        long long tmp = state;
        moveDown(tmp);
        children.push_back(std::make_pair(tmp, 'D'));
    }

    if(getPosSpace(state) % 3 != 0){
        long long tmp = state;
        moveLeft(tmp);
        children.push_back(std::make_pair(tmp, 'L'));
    }

    if(getPosSpace(state) % 3 != 2){
        long long tmp = state;
        moveRight(tmp);
        children.push_back(std::make_pair(tmp, 'R'));
    }
}

//  методы передвигают пустую костяшку в соответствующем направлении
void ProblemSolver::moveUp(long long& state){
    int space = getPosSpace(state);
    setState(state, space, getState(state, space - 3));
    setState(state, space - 3, 0);
}

void ProblemSolver::moveDown(long long& state){
    int space = getPosSpace(state);

    setState(state, space, getState(state, space + 3));
    setState(state, space + 3, 0);
}

void ProblemSolver::moveLeft(long long& state){
    int space = getPosSpace(state);

    setState(state, space, getState(state, space - 1));
    setState(state, space - 1, 0);
}

void ProblemSolver::moveRight(long long& state){
    int space = getPosSpace(state);

    setState(state, space, getState(state, space + 1));
    setState(state, space + 1, 0);
}


void ProblemSolver::setState(long long& state, int pos, int value){

    //  метод устанавливает в состояние state значение value на позицию pos
    state = (state & masksSet[pos]) | static_cast<long long>(value) << (4 * (8 - pos));

}

int ProblemSolver::getState(const long long state, int pos){

    //  метод получает из состояния state значение позиции pos
    return (state & masksGet[pos]) >> (4 * (8 - pos));

}


int ProblemSolver::heuristic(long long state){

    //  эвристика (Манхэттенское расстояние)
    int result = 0;
    for(int i = 0; i < 9; ++i)
    {
        int tmp = getState(state, i);
        if(tmp == 0){
            continue;
        }

        result += std::abs((tmp - 1) / 3 - i / 3);
        result += std::abs((tmp - 1) % 3 - i % 3);
    }

    return result;

}

bool ProblemSolver::possible(){

    //  метод определяет возможность разрешения головоломки через
    //  четность/нечетность количества беспорядков
    int mess = 0;
    for(int i = 0; i < 9; ++i)
    {
        for(int j = i + 1; j < 9; ++j)
        {
            long long stateI = getState(data, i);
            long long stateJ = getState(data, j);
            if(stateI == 0 || stateJ == 0){
                continue;
            }
            if(stateI > stateJ){
                mess++;
            }
        }
    }

    return mess % 2 == 0;
}

int ProblemSolver::findMinPath(std::vector< char >& path){
    if(!possible()){
        return -1;
    }

    return AStar(path);
}

int ProblemSolver::AStar(std::vector< char >& path){

    //  решение задачи с использованием алгоритма A*
    //  
    //  order - приоритетная очередь, порядок посещения вершин. приоритет вершины
    //  определяется как суммарное пройденное расстояние плюс эвристика для этой вершины
    //  visited - поддержка уже посещенных вершин
    //  parents - мапа, хранящая родителя вершины и действие, которым был совершен переход
    std::priority_queue< Point, std::vector< Point>, PointCompare > order;
    std::set< long long > visited;
    std::map< long long, std::pair< long long, char > > parents;

    order.push(Point(data, 0, 0));
    parents[data] = std::make_pair(-1, 'U');

    int ans = -1;

    while(!order.empty()){
        Point current = order.top();
        order.pop();

        //  если уже посещена - ничего не делаем
        if(visited.find(current.state) != visited.end()){
            continue;
        }

        //  необходимое состояние найдено
        if(current.state == AIM_STATE){
            ans = current.vNum;

            break;
        }

        //  просматриваем всех детей вершины
        std::vector< std::pair< long long, char > > children;
        getChildren(current.state, children);

        for(auto ch : children){
            //  если уже посещена - ничего не делаем
            if(visited.find(ch.first) != visited.end()){
                continue;
            }
            //  закидываем в очередь детей с новыми приоритетами
            order.push(Point(ch.first, current.vNum + 1 + heuristic(ch.first), current.vNum + 1));
            //  сохраняем родителя и путь
            parents[ch.first] = std::make_pair(current.state, ch.second);
        }

        visited.insert(current.state);
    }

    //  восстанавливаем путь
    long long current = AIM_STATE;
    while(true){
        if(current == data){
            break;
        }
        path.push_back(parents[current].second);
        current = parents[current].first;
    }

    return ans;

}



int main()
{
    int data = 0;

    for(int i = 0; i < 9; ++i){
        int tmp;
        std::cin >> tmp;
        data = data * 10 + tmp;
    }

    ProblemSolver problemSolver(data);

    std::vector< char > path;
    std::cout << problemSolver.findMinPath(path) << "\n";

    for(int i = 0; i < path.size(); ++i){
        std::cout << path[path.size() - i - 1];
    }

    return 0;
}
