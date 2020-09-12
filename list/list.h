/*
    双向链表类，提供双向链表接口
    需求：
    1、在指定位置进行删除操作复杂度为O(1)，这里必须暴露对外的Node指针
    2、实现头部插入、尾部插入、头部删除、尾部删除，时间复杂度为O(1)
    3、实现指定指针前一位、后一位插入、指定指针删除
    4、实现空判断、find查找输出节点指针

    因为双向链表如果提供两个哨兵节点在进行正反向遍历的时候反而不太方便（要无视遍历到的最后一个节点，因此这里不使用哨兵，看看要多处理到什么步骤，正好做个对比）

    思考：
    【当我们对外提供了Node节点的控制方法之后，也意味着使用者可以通过这个节点的next值使得链表失效
    STL是怎么做的，可以去看看他是怎么避免对外提供的指针破坏了结构，或者只是通过使用者自己把控】

    出错情况：
    1、在头部插入和尾部插入的时候错误（应该先画图）
    2、头尾指针都指向同一个地址，所以对于尾指针的数据修改也会导致头指针的数据修改
       所以除了处理插入空链表，还要处理插入只有一个节点的链表，单向也是一样的（哨兵节点的优势又有体现出来）
    3、忘记length的修改操作，判断空其实可以不使用length，但是计算长度以及处理1个2个节点的情况却很有用
    --
    --    移除节点考虑三种情况(指定指针移除点不必使用临时指针变量，因为指定的这个指针可以直接用)
    --    1、只有一个点
    --    2、头尾移除
    --    3、中间点移除
    --
*/
#include <string>
#include <iostream>
using std::string;
using std::cout;
using std::endl;

typedef struct Node
{
    struct Node *next;
    struct Node *prev;
    string data;
} Node;


class List
{
private:
    Node *pHeadNode;
    Node *pEndNode;
    int length;
public:
    List();
    ~List();
    // 新建节点
    Node* createNode(const string data)
    {
        Node* newNode = new(Node);
        newNode->next = NULL;
        newNode->prev = NULL;
        newNode->data = data;
        return newNode;
    }

    void push_front(const string);               //头部插入元素
    void push_back(const string);                //尾部插入元素
    Node* find(const string);                    //查找节点O(n)
    void erase(const string);                    //指定值移除节点O(n)
    void erase(Node*);                           //指定节点指针移除节点O(1)
    bool insert_perv(Node*, const string);       //在指定节点前面插入O(1)
    bool insert_next(Node*, const string);       //在指定节点后面插入O(1)
    bool pop_front(string &);                    //弹出头部元素
    bool pop_back(string &);                     //弹出尾部元素
    bool front(string &);                        //取第一个元素数据
    bool back(string &);                         //取最后一个元素数据
    bool is_empty();                             //判断是否为空
    void list_print();                           //打印链表
};