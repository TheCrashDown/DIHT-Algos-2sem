/* Task:
   Задано дерево с корнем, содержащее (1 ≤ n ≤ 100 000) вершин, пронумерованных от 0 до n-1.
   Требуется ответить на m (1 ≤ m ≤ 10 000 000) запросов о наименьшем общем предке для пары вершин.
   Запросы генерируются следующим образом. Заданы числа a1, a2 и числа x, y и z.
   Числа a3, ..., a2m генерируются следующим образом: ai = (x ⋅ ai-2 + y ⋅ ai-1 + z) mod n.
   Первый запрос имеет вид (a1, a2). Если ответ на i-1-й запрос равен v, 
   то i-й запрос имеет вид ((a2i-1 + v) mod n, a2i).

   Для решения задачи можно использовать метод двоичного подъёма.

   ...но можно не значит нужно, так что просто сведем задачу к задаче RMQ и будем радоваться жизни.

   developed by Crashdown

*/
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <climits>


//===============================================================================//
//=======================скопированный файл IGraph.h=============================//

class IGraph
{
public:

    virtual ~IGraph() {};

    virtual void AddEdge(int from, int to) = 0;

    virtual int VerticesCount() const = 0;

    virtual void GetNextVertices(int vertex, std::vector<int>& vertices) const = 0;
    virtual void GetPrevVertices(int vertex, std::vector<int>& vertices) const = 0;

};

//===============================================================================//
//=====================скопированный файл ListGraph.h============================//

class ListGraph : public IGraph
{
public:

    ListGraph(int _numVertices);

    ListGraph(const IGraph& other);

    ~ListGraph() override;

    void AddEdge(int from, int to) override;

    int VerticesCount() const override;

    void GetNextVertices(int vertex, std::vector<int>& vertices) const override;
    void GetPrevVertices(int vertex, std::vector<int>& vertices) const override;

private:

    std::vector< std::vector<int> > graph;

    int numVertices;
};

//===============================================================================//
//====================скопированный файл ListGraph.cpp===========================//


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



//==========================конец скопированной части============================//
//===============================================================================//



//================================================================================================//
//          Класс ProblemSolver, сводящий задачу LCA к RMQ и решающий ее деревом отрезков         //
//================================================================================================//
class ProblemSolver
{
public:

    ProblemSolver(const IGraph& _graph);

    ~ProblemSolver();

    int getLCA(int left, int right);

private:

    int n;

    const IGraph& graph;

    //  дерево обхода для RMQ (список вершин)
    std::vector< int > order;

    //  список глубин вершин
    std::vector< int > depths;

    //  массив первых вхождений в список глубин
    std::vector< int > firstIndex;

    //  дерево отрезков
    std::vector< int > tree;

    //  построение массива для решения RMQ обходом в глубину
    void LCAtoRMQBuild();
    void LCAtoRMQ_DFS(int vertex, int depth);

    //  сравниваем две вершины по глубине и возвращаем ту, у которой глубина меньше
    int compareByDepth(int resL, int resR);

    //  построение дерева отрезков
    void treeBuild(int pos, int left, int right);

    //  запрос минимума
    int treeMin(int pos, int left, int right, int windowLeft, int windowRight);

    //  увеличение размера дерева(если выделено мало места). доказано, что размер ДО <= 4n
    //  метод используется, чтобы не выделять много памяти, если она не нужна по факту
    void grow(int pos);

};

//================================================================================================//
//================================================================================================//

ProblemSolver::ProblemSolver(const IGraph& _graph): graph(_graph), n(_graph.VerticesCount())
{
    LCAtoRMQBuild();
}

ProblemSolver::~ProblemSolver(){}

void ProblemSolver::grow(int pos)
{
    if(tree.size() <= pos)
    {
        tree.resize(pos + 1);
    }
}

int ProblemSolver::compareByDepth(int resL, int resR)
{
    if(resL == -1){
        return resR;
    }
    if(resR == -1){
        return resL;
    }
    if(depths[resL] < depths[resR]){
        return resL;
    }
    return resR;
}


void ProblemSolver::LCAtoRMQBuild()
{
    firstIndex.resize(n, -1);
    depths.resize(n, -1);

    LCAtoRMQ_DFS(0, 0);

    treeBuild(0, 0, order.size() - 1);
}


void ProblemSolver::LCAtoRMQ_DFS(int vertex, int depth)
{
    //  прокидываем в список посещений
    order.push_back(vertex);

    if(depths[vertex] == -1)
    {
        depths[vertex] = depth;
    }

    //  если надо, ставим первую встречу
    if(firstIndex[vertex] == -1)
    {
        firstIndex[vertex] = order.size() - 1;
    }

    std::vector< int > children;
    graph.GetNextVertices(vertex, children);

    for(auto child : children)
    {
        LCAtoRMQ_DFS(child, depth + 1);

        //  прокидываем в список посещений еще раз на выходе из ребенка
        order.push_back(vertex);
    }
}

void ProblemSolver::treeBuild(int pos, int left, int right)
{
    //  пришли в лист
    if(left == right){
        grow(pos);
        tree[pos] = order[left];
        return;
    }

    //  рекурсивно строим детей
    int middle = (left + right) / 2;
    treeBuild(pos * 2 + 1, left, middle);
    treeBuild(pos * 2 + 2, middle + 1, right);


    int resL = tree[pos * 2 + 1];
    int resR = tree[pos * 2 + 2];

    //  собираем ответ для вершины, зная ответы для детей
    tree[pos] = compareByDepth(resL, resR);
}

int ProblemSolver::treeMin(int pos, int left, int right, int windowLeft, int windowRight)
{
    //  текущий отрезок не попадает в искомое окно
    if(left > windowRight || right < windowLeft){
        return -1;
    }

    //  текущий отрезок полностью содержится в искомом окне
    if(windowLeft <= left && right <= windowRight){
        return tree[pos];
    }


    //  рекурсивно спрашиваем ответ у детей
    int middle = (left + right) / 2;
    int resL = treeMin(pos * 2 + 1, left, middle, windowLeft, windowRight);
    int resR = treeMin(pos * 2 + 2, middle + 1, right, windowLeft, windowRight);

    //  собираем ответ для вершины, зная ответы для детей
    return compareByDepth(resL, resR);
}

int ProblemSolver::getLCA(int left, int right)
{
    if(left > right)
    {
        std::swap(left, right);
    }

    int indexLeft = std::min(firstIndex[left], firstIndex[right]);
    int indexRight = std::max(firstIndex[left], firstIndex[right]);

    return treeMin(0, 0, order.size() - 1, indexLeft, indexRight);
}


//================================================================================================//
//================================================================================================//


int main()
{
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);
    
    int n, m;

    std::cin >> n >> m;

    IGraph* graph = new ListGraph(n);

    for(int i = 1; i < n; ++i)
    {
        int tmp;
        std::cin >> tmp;

        graph->AddEdge(tmp, i);
    }

    ProblemSolver problemSolver(*graph);

    long long a, b;
    std::cin >> a >> b;

    // index - индекс числа b в исчислении массива a[1..2m] из условия. Хранить весь массив не нужно,
    // будем поддерживать только последний - b, и предпоследний - a элементы.
    int index = 2;

    int x, y, z;
    std::cin >> x >> y >> z;

    //  искомая сумма
    long long ans = 0;
    //  запомненный последний ответ
    int last = 0;

    last = problemSolver.getLCA(a, b);
    ans += last;

    for(int i = 2; i <= m; ++i)
    {
        //  получение нужного элемента массива
        while(index < 2 * i)
        {
            index++;
            long long c = (x * a + y * b + z) % n;
            a = b;
            b = c;
        }
        int tmp = problemSolver.getLCA((a + last) % n, b);
        ans += tmp;
        last = tmp;
    }

    std::cout << ans;

    delete graph;

    return 0;
}
