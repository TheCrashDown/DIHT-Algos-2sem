/* Task:
   Реализуйте структуру данных “массив строк” на основе декартового дерева
   по неявному ключу со следующими методами:
   // Добавление строки в позицию position.
   // Все последующие строки сдвигаются на одну позицию вперед.
   void InsertAt( int position, const std::string& value );
   // Удаление строки из позиции position.
   // Все последующие строки сдвигаются на одну позицию назад.
   void DeleteAt( int position );
   // Получение строки из позиции position.
   std::string GetAt( int position );

   Все методы должны работать за O(log n) в среднем,
   где n – текущее количество строк в массиве.

   developed by Crashdown

*/
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <climits>
#include <memory>
#include <stdio.h>


//================================================================================================//
//                                            Декартач                                            //
//================================================================================================//
class Treap
{
public:

    Treap();
    ~Treap();

    void insertAt(int pos, const std::string& string);

    void deleteAt(int pos);

    std::string getAt(int pos);

private:

    struct Node;

    std::shared_ptr< Node > root = nullptr;

    //  сплитим root по key, результат -  left и right
    void split(std::shared_ptr<Node> root, std::shared_ptr<Node>& left, std::shared_ptr<Node>& right, int key);

    //  мерджим left и right, результат - root
    void merge(std::shared_ptr<Node> left, std::shared_ptr<Node> right, std::shared_ptr<Node>& root);
};

//================================================================================================//
//================================================================================================//

Treap::Treap(): root(nullptr) {}


Treap::~Treap() {}


struct Treap::Node
{
    std::string string = "";

    int priority = 0;

    //  поддержка размера поддерева
    int subtree = 1;

    std::shared_ptr< Node > left = nullptr;
    std::shared_ptr< Node > right = nullptr;

    Node(): priority(rand()) {}
    Node(const std::string& _string): string(_string), priority(rand()) {}

    //  пересчет размера поддерева
    void updateSubtree()
    {
        subtree = 1;

        //  прибавляем размеры поддеревьев детей, если они ненулевые
        subtree += (left != nullptr) ? left->subtree : 0;
        subtree += (right != nullptr) ? right->subtree : 0;
    }
};


void Treap::split(std::shared_ptr<Node> root, std::shared_ptr<Node>& left, std::shared_ptr<Node>& right, int pos)
{
    if (root == nullptr){
        left = nullptr;
        right = nullptr;
        return;
    }

    int size = (root->left == nullptr) ? 0 : root->left->subtree;

    if(size >= pos){
        split(root->left, left, root->left, pos);
        root->updateSubtree();
        right = root;
        return;
    }

    split(root->right, root->right, right, pos - size - 1);
    root->updateSubtree();
    left = root;
}


void Treap::merge(std::shared_ptr<Node> left, std::shared_ptr<Node> right, std::shared_ptr<Node>& root)
{
    if(right == nullptr)
    {
        root = left;
        return;
    }

    if(left == nullptr)
    {
        root = right;
        return;
    }

    if(left->priority > right->priority)
    {
        merge(left->right, right, left->right);
        left->updateSubtree();
        root = left;
        return;
    }

    merge(left, right->left, right->left);
    right->updateSubtree();
    root = right;
}


void Treap::insertAt(int pos, const std::string& string)
{
    std::shared_ptr< Node > left = nullptr;
    std::shared_ptr< Node > right = nullptr;

    std::shared_ptr< Node > node = std::make_shared< Node >(string);

    //  разрезаем дерево по ключу pos
    split(root, left, right, pos);

    //  сливаем результаты и новый элемент, используя промежуточное значение tmp
    std::shared_ptr< Node > tmp = nullptr;
    merge(left, node, tmp);
    merge(tmp, right, root);
}


void Treap::deleteAt(int pos)
{
    std::shared_ptr< Node > left = nullptr;
    std::shared_ptr< Node > right = nullptr;

    //  разрезаем дерево по ключу pos
    split(root, left, right, pos);

    //  убираем удаляемый элемент (после последней операции он стал крайним в поддереве) -
    //  сплитим поддерево, где он находится, на этот элемент и все кроме него
    std::shared_ptr< Node > cutted = nullptr;
    std::shared_ptr< Node > rest = nullptr;
    split(right, cutted, rest, 1);

    //  собираем дерево без этого элемента - left и rest
    merge(left, rest, root);
}


std::string Treap::getAt(int pos)
{
    std::shared_ptr< Node > current = root;

    while(true)
    {
        int left_size  = (current->left == nullptr) ? 0 : current->left->subtree;

        if(left_size == pos)
        {
            break;
        }

        //  спускаемся в нужного сына
        if (left_size > pos)
        {
            current = current->left;
        }
        else
        {
            pos -= (left_size + 1);
            current = current->right;
        }
    }

    return current->string;
}



//================================================================================================//
//================================================================================================//

int main()
{
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);

    int k;
    std::cin >> k;

    Treap* treap = new Treap();

    for(int i = 0; i < k; ++i)
    {
        char query = 0;
        std::cin >> query;

        switch(query)
        {
            case '+' :
            {
                int pos = 0;
                std::string tmp = "";
                std::cin >> pos >> tmp;

                treap->insertAt(pos, tmp);
                break;
            }
            case '-' :
            {
                int left, right = 0;
                std::cin >> left >> right;
                for(int j = left; j <= right; ++j)
                {
                    treap->deleteAt(left);
                }
                break;
            }

            case '?' :
            {
                int pos = 0;
                std::cin >> pos;
                std::cout << treap->getAt(pos) << "\n";
                break;
            }
        }
    }

    delete treap;

    return 0;
}
