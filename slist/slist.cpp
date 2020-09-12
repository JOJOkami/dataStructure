#include "link.h"

SList::SList()
{
    pHeadNode = new(Node);
    pHeadNode->next = NULL;
    pHeadNode->data = "";
    pEndNode = NULL;
    length = 0;
}

// 新建节点
Node* SList::createNode(const string data)
{
    Node* newNode = new(Node);
    newNode->next = NULL;
    newNode->data = data;
    return newNode;
}

SList::~SList()
{
    // temp临时指向下一个节点，删除本节点i，然后让i继续遍历下一个temp
    Node* i = pHeadNode;
    while(i != NULL)
    {
        Node* temp = i->next;
        delete(i);
        i = temp;
    }
}

bool SList::is_empty()
{
    return (length == 0);
}

void SList::push_back(const string data)
{
    // 如果链表为空，为头节点和尾节点赋值
    if (pHeadNode->next == NULL)
    {
        Node* insertNode = createNode(data);
        pHeadNode->next = insertNode;
        pEndNode = insertNode;
    }
    // 如果链表不为空，直接在尾部插入即可
    else
    {
        Node* insertNode = createNode(data);
        pEndNode->next = insertNode;
        pEndNode = insertNode;
    }
    length++;
}

void SList::push_front(const string data)
{
    // 如果链表为空，为头节点和尾节点赋值
    if (pHeadNode->next == NULL)
    {
        Node* insertNode = createNode(data);
        pHeadNode->next = insertNode;
        pEndNode = insertNode;
    }
    // 如果链表不为空，直接在首节点前插入即可
    else
    {
        Node* insertNode = createNode(data);
        insertNode->next = pHeadNode->next;
        pHeadNode->next = insertNode;
    }
    length++;
}

bool SList::insert(const int n, const string data)
{
    if (n < 1)
        return false;
    // 如果链表为空，直接返回错误
    if (pHeadNode->next == NULL)
    {
        return false;
    }
    // 如果链表不为空，遍历找到插入点进行插入
    else
    {
        // 这也是哨兵节点pHeadNode的好处，可以简洁的处理首尾插入的情况
        Node *frontNode = pHeadNode;
        int count = 1; 
        // frontNode为上一个节点，循环到尾部或者count数量达到目的插入点进行插入操作
        while(frontNode->next != NULL && count < n)
        {
            frontNode = frontNode->next;
            count++;
        }
        // 目的插入点不存在，返回错误
        if (count < n)
            return false;
        // frontNode为上一个节点，因此只要将frontNode->next指向新节点，新节点指向原frontNode->next即可
        /* !!!! 不写错插入的精髓，新插入的节点要先给其next赋值 !!!! */
        Node* insertNode = createNode(data);
        insertNode->next = frontNode->next;
        frontNode->next = insertNode;
        length++;
    }
    return true;
}

int SList::find(const string value)
{
    Node *iNode = pHeadNode->next;
    int count = 0; 
    while(iNode != NULL)
    {
        if (iNode->data == value)
        {
            return count;
        }
        iNode = iNode->next;
    }
    return count;
}

// 没有哨兵节点的话就需要特殊处理第一个节点，因为第一个节点没有上一个节点
void SList::remove(const string value)
{
    Node *frontNode = pHeadNode;
    int count = 0; 
    while(frontNode->next != NULL)
    {
        if (frontNode->next->data == value)
        {
            /* !!!删除得用temp !!! */
            Node *tempNode = frontNode->next;
            frontNode->next = frontNode->next->next;
            delete(tempNode);
            length--;
        }
        frontNode = frontNode->next;
    }
}

void SList::pop_front()
{
    if (pHeadNode->next == NULL)
        return;
    Node* tempNode = pHeadNode->next;
    pHeadNode->next = pHeadNode->next->next;
    delete(tempNode);
    length--;
}

void SList::pop_back()
{
    if (pHeadNode->next == NULL)
        return;
    Node* iNode = pHeadNode;
    /* !!! 这里end节点简化了编程难度，不用出现n->next->next的判断 !!! */
    while (iNode->next != pEndNode)
    {
        iNode = iNode->next;
    }
    delete(pEndNode);
    pEndNode = iNode;
    /* !!! 这里释放掉最后一个节点之后，倒数第二个节点的下一个节点要指向NULL，否则是指向非法内存 !!! */
    pEndNode->next = NULL;
    length--;
}

void SList::sListPrint()
{
    Node* i = pHeadNode->next;
    while(i != NULL)
    {
        cout<<(i->data)<<endl;
        i = i->next;
    }
}

/*
    反转思路：
    1、上一个点frontNode初始化为NULL，iNode为当前节点，
    2、赋值nextNode为iNode->next作为下一个处理的点
    3、iNode->next指向上一个点frontNode
    4、赋值frontNode为iNode作为上一个点 回到第2步(终止条件为nextNode为空)
    pEnd处理：把第一个节点的地址存到end指针
    pHead处理：反转到最后一个节点把最后一个节点的地址作为pHead的next
*/
void SList::reverse()
{
    if (length <= 1)
        return;
    Node *frontNode = NULL;
    Node *iNode = pHeadNode->next;
    pEndNode = iNode;
    Node *nextNode = NULL;
    do{
        nextNode = iNode->next;
        iNode->next = frontNode;
        frontNode = iNode;
        iNode = nextNode;
    } while(nextNode != NULL);
    pHeadNode->next = frontNode;
}