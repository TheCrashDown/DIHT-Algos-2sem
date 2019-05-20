/* Task:
   Дан массив из целых чисел a1, a2, ..., an. Для каждого запроса [left, right] найдите такой 
   подотрезок al, al+1, ..., ar этого массива (1 <= left <= l <= r <= right <= n), что
   сумма чисел al + al+1 + ... + ar является максимально возможной.
   Требуемое время ответа на запрос - O(log n).  
   
   developed by Crashdown

*/
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <climits>

//================================================================================================//
//                                       Дерево отрезков                                          //
//================================================================================================//
class SegmentTree
{
public:
    SegmentTree(std::vector< int >& sequence);

    ~SegmentTree();

    long long getMaxUnderSum(int left, int right);

private:

    struct Vertex;

    std::vector< Vertex > tree;

    int n;

    //  построение дерева
    void build(int pos, int left, int right, std::vector< int >& sequence);

    //  запрос максимума
    Vertex max(int pos, int left, int right, int windowLeft, int windowRight);

    //  увеличение размера дерева(если выделено мало места). доказано, что размер ДО <= 4n
    //  метод используется, чтобы не выделять много памяти, если она не нужна по факту
    void grow(int pos);

};

//================================================================================================//
//================================================================================================//

struct SegmentTree::Vertex
{
    //  сумма всего отрезка
    long long sum = 0;
    //  максимальная сумма префикса
    long long prefixSum = 0;
    //  максимальная сумма суффикса
    long long suffixSum = 0;
    //  максимальная сумма на подотрезках
    long long maxUnderSum = 0;

    Vertex(){}
    Vertex(long long _sum, long long _prefixSum, long long _suffixSum, long long _maxUnderSum): sum(_sum),
                                                                                                prefixSum(_prefixSum),
                                                                                                suffixSum(_suffixSum),
                                                                                                maxUnderSum(_maxUnderSum){}
};

SegmentTree::SegmentTree(std::vector< int >& sequence)
{
    n = sequence.size();
    build(0, 0, n - 1, sequence);
}

SegmentTree::~SegmentTree(){}

void SegmentTree::grow(int pos)
{
    if(tree.size() <= pos)
    {
        tree.resize(pos + 1);
    }
}

void SegmentTree::build(int pos, int left, int right, std::vector< int >& sequence)
{
    //  пришли в лист
    if(left == right){
        grow(pos);
        tree[pos] = Vertex(sequence[left], sequence[left], sequence[left], sequence[left]);
        return;
    }

    //  рекурсивно строим детей
    int middle = (left + right) / 2;
    build(pos * 2 + 1, left, middle, sequence);
    build(pos * 2 + 2, middle + 1, right, sequence);


    Vertex resL = tree[pos * 2 + 1];
    Vertex resR = tree[pos * 2 + 2];

    //  собираем ответ для вершины, зная ответы для детей
    tree[pos] = Vertex(resL.sum + resR.sum,                                     //  сумма
                       std::max(resL.prefixSum, resL.sum + resR.prefixSum),     //  максимальны префикс
                       std::max(resR.suffixSum, resR.sum + resL.suffixSum),     //  максимальный суффикс
                       std::max(std::max(resL.maxUnderSum, resR.maxUnderSum), resL.suffixSum + resR.prefixSum));
                       //   ответ либо полностью в левом ребенке, либо полностью в правом, либо частично там и там,
                       //   т.е. в суффиксе левого и префиксе правого
}

SegmentTree::Vertex SegmentTree::max(int pos, int left, int right, int windowLeft, int windowRight)
{
    //  текущий отрезок не попадает в искомое окно
    if(left > windowRight || right < windowLeft){
        return Vertex(INT_MIN, INT_MIN, INT_MIN, INT_MIN);
        //return Vertex(-1000, -1000, -1000, -1000);
    }

    //  текущий отрезок полностью содержится в искомом окне
    if(windowLeft <= left && right <= windowRight){
        return tree[pos];
    }


    //  рекурсивно спрашиваем ответ у детей
    int middle = (left + right) / 2;
    Vertex resL = max(pos * 2 + 1, left, middle, windowLeft, windowRight);
    Vertex resR = max(pos * 2 + 2, middle + 1, right, windowLeft, windowRight);

    //  собираем ответ для вершины, зная ответы для детей
    return Vertex(resL.sum + resR.sum,                                     //  сумма
                       std::max(resL.prefixSum, resL.sum + resR.prefixSum),     //  максимальны префикс
                       std::max(resR.suffixSum, resR.sum + resL.suffixSum),     //  максимальный суффикс
                       std::max(std::max(resL.maxUnderSum, resR.maxUnderSum), resL.suffixSum + resR.prefixSum));
                       //   ответ либо полностью в левом ребенке, либо полностью в правом, либо частично там и там,
                       //   т.е. в суффиксе левого и префиксе правого;
}


long long SegmentTree::getMaxUnderSum(int left, int right)
{
    return max(0, 0, n - 1, left, right).maxUnderSum;
}

//================================================================================================//
//================================================================================================//


int main()
{
    int n, m;
    std::cin >> n >> m;

    while(!std::cin.eof())
    {
        std::vector< int > sequence;
        for(int i = 0; i < n; ++i)
        {
            int tmp;
            std::cin >> tmp;
            sequence.push_back(tmp);
        }

        SegmentTree segmentTree(sequence);

        for(int i = 0; i < m; ++i)
        {
            int left, right;
            std::cin >> left >> right;

            std::cout << segmentTree.getMaxUnderSum(left - 1, right - 1) << "\n";
        }

        std::cin >> n >> m;
    }

    return 0;
}
