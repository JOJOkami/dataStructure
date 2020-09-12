// 链表(string类型)的简单实现
// 1 实现单向链表的基本功能：增（指定位置） 删（指定位置） 查（确认某个值是否存在）
// 2 附加需求：实现倒转链表
#include <string>
#include <iostream>
using std::string;
using std::cout;
using std::endl;

/*
    边界值：
    1 链表为空是否正常
    2 链表只有一个节点是否正常
    3 链表只有两个节点是否正常
    4 处理头部和尾部是否正常
*/

// 链表结点
typedef struct node
{
    struct node *next;
    string data;
} Node;

class SList
{
private:
    // 头节点不存放数据（哨兵节点，可简化空链表插入和删除的情况）
    Node *pHeadNode;
    // 这个尾部指针使得单向链表可以在尾部插入的时候时间复杂度也是常量级(但是尾部删除还是很慢)
    Node *pEndNode;
    int length;
    Node* createNode(const string);
    
public:
    SList();
    ~SList();
    void push_back(const string);
    void push_front(const string);
    bool insert(const int, const string);
    int find(const string);
    void remove(const string);
    void pop_front();
    void pop_back();
    void sListPrint();
    bool is_empty();
    void reverse();
};