/* Task:
   Дано число N и последовательность из N целых чисел. Найти вторую порядковую статистику на заданных диапазонах.
   Для решения задачи используйте структуру данных Sparse Table. 
   Требуемое время обработки каждого диапазона O(1). Время подготовки структуры данных O(nlogn).

   developed by Crashdown

*/
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <climits>


//================================================================================================//
//  Класс, аналогичный Sparse Table, но поддерживающий не только минимум, но и вторую статистику  //
//================================================================================================//
class SparseTable2
{
public:
    SparseTable2(std::vector< int >& sequence);

    ~SparseTable2();

    int get2stat(int left, int right);

private:

    //  значение статистики + ее индекс в исходном массиве
    struct Node;
    //  пара значений - первая и вторая порядковая статистики
    typedef std::pair< Node, Node > Vertex;

    Vertex min(Vertex x, Vertex y);

    std::vector< std::vector< Vertex > > table;

    //  предподсчитанные округленные вверх двоичные логарифмы от чисел 1..n
    std::vector< int > logCeil;

    int exp2(int pow);

};

//================================================================================================//
//================================================================================================//

struct SparseTable2::Node
{
    int stat = 0;

    //  индекс статистики в исходном массиве, чтобы уметь различать одинаковые элементы
    int index = 0;

    Node(int _stat, int _index): stat(_stat), index(_index){}
    Node(){}

    bool operator< (Node& other){
        return stat < other.stat;
    }
};


int SparseTable2::exp2(int pow)
{
    return (1 << pow);
}

SparseTable2::Vertex SparseTable2::min(Vertex x, Vertex y)
{
    std::vector< Node > sorted;
    sorted.push_back(x.first);
    sorted.push_back(x.second);
    sorted.push_back(y.first);
    sorted.push_back(y.second);

    std::sort(sorted.begin(), sorted.end());

    Vertex min;

    min.first = sorted[0];

    int i = 1;
    while(sorted[0].index == sorted[i].index)
    {
        ++i;
    }

    min.second = sorted[i];

    return min;
}


SparseTable2::SparseTable2(std::vector< int >& sequence)
{
    //  инициализация логарифмов
    for (int i = 0; i <= sequence.size(); ++i)
    {
        logCeil.push_back(static_cast<int>(ceil(log2(i))));
    }

    //  инициализация таблицы
    table.resize(logCeil[sequence.size()]);
    for(int i = 0; i < table.size(); ++i)
    {
        table[i].resize(sequence.size(), std::make_pair( Node(INT_MAX, -1), Node(INT_MAX, -1) ));
    }


    //  1 ряд таблицы = исходному массиву
    for(int i = 0; i < sequence.size(); ++i)
    {
        table[0][i] = std::make_pair( Node(sequence[i], i), Node(sequence[i], i) );
    }

    //  остальные ряды
    for(int i = 1; i < table.size(); ++i)
    {
        for(int j = 0; j < table[i].size() - exp2(i) + 1; ++j)
        {
            table[i][j] = min(table[i - 1][j], table[i - 1][j + exp2(i - 1)]);
        }
    }
}

SparseTable2::~SparseTable2(){}

int SparseTable2::get2stat(int left, int right)
{
    int log = logCeil[right - left + 1] - 1;

    Vertex res = min(table[log][left], table[log][right - exp2(log) + 1]);

    return res.second.stat;
}
//================================================================================================//
//================================================================================================//

int main()
{
    int n, m;
    std::cin >> n >> m;

    std::vector< int > sequence;

    for (int i = 0; i < n; ++i)
    {
        int tmp;
        std::cin >> tmp;

        sequence.push_back(tmp);
    }

    SparseTable2 sparseTable2(sequence);

    for(int i = 0; i < m; ++i){
        int left, right;
        std::cin >> left >> right;

        std::cout << sparseTable2.get2stat(left - 1, right - 1) << "\n";
    }

    return 0;
}
