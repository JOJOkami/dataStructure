#include "list.h"

List::List()
{
    pHeadNode = NULL;
    pEndNode = NULL;
    length = 0;
}

List::~List()
{
    Node *iNode = pHeadNode;
    while (iNode != NULL)
    {
        Node *tempNode = iNode->next;
        delete(iNode);
        iNode = tempNode;
    }
}

void List::push_front(const string data)
{
    Node *newNode = createNode(data);
    if (length == 0)
    {
        pHeadNode = newNode;
        pEndNode = newNode;
        length++;
        return;
    }
    newNode->next = pHeadNode;
    pHeadNode->prev = newNode;
    if (length == 1)            //!!这里很容易被忽略
        pEndNode = pHeadNode;
    pHeadNode = newNode;
    length++;
}

void List::push_back(const string data)
{
    Node *newNode = createNode(data);
    if (length == 0)
    {
        pHeadNode = newNode;
        pEndNode = newNode;
        length++;
        return;
    }
    pEndNode->next = newNode;
    newNode->prev = pEndNode;
    if (length == 1)            //!!这里很容易被忽略
        pHeadNode = pEndNode;
    pEndNode = newNode;
    length++;
}

Node* List::find(const string data)
{
    Node *iNode = pHeadNode;
    while (iNode != NULL)
    {
        if (iNode->data == data)
        {
            return iNode;
        }
        iNode = iNode->next;
    }
    return NULL;
}

/* 
    注意这里我们用bool来表示是否插入成功 
    思考：这里如果传入的findNode有问题，没有next成员，而是非法的地址会有异常，要怎么处理
*/
bool List::insert_next(Node* findNode, const string data)
{
    if (findNode == NULL)
        return false;
    Node *insertNode = createNode(data);
    insertNode->prev = findNode;
    insertNode->next = findNode->next;
    findNode->next->prev = insertNode;
    findNode->next = insertNode;
    length++;
    return true;
}

/* 
    注意这里我们用bool来表示是否插入成功 
    思考：这里如果传入的findNode有问题，没有next成员，而是非法的地址会有异常，要怎么处理
*/
bool List::insert_perv(Node* findNode, const string data)
{
    if (findNode == NULL)
        return false;
    Node *insertNode = createNode(data);
    insertNode->next = findNode;
    insertNode->prev = findNode->prev;
    findNode->prev->next = insertNode;
    findNode->prev = insertNode;
    length++;
    return true;
}

void List::erase(const string data)
{
    Node* delNode = this->find(data);
    this->erase(delNode);
}

/*
    移除节点考虑三种情况(指定指针移除点不必使用临时指针变量，因为指定的这个指针可以直接用)
    1、只有一个点
    2、头尾移除
    3、中间点移除
*/
void List::erase(Node* delNode)
{
    if (delNode == NULL || this->is_empty())
        return;
    if (length == 1)
    {
        delete(pHeadNode);
        pHeadNode = NULL;
        pEndNode = NULL;
        length = 0;
        return;
    }
    if (delNode == pHeadNode)
    {
        pHeadNode = delNode->next;
        pHeadNode->prev = NULL;
        length--;
        delete(delNode);
    }
    else if (delNode == pEndNode)
    {
        pEndNode = delNode->prev;
        pEndNode->next = NULL;
        length--;
        delete(delNode);
    }
    else
    {
        delNode->prev->next = delNode->next;
        length--;
        delete(delNode);
    }
}

// 指定位置删除，需要中间变量
bool List::pop_front(string &data)
{
    if (this->is_empty())
        return false;
    if (length == 1)
    {
        delete(pHeadNode);
        pHeadNode = NULL;
        pEndNode = NULL;
        length--;
        return true;
    }
    Node *tempNode = pHeadNode->next;
    tempNode->prev = NULL;
    data = pHeadNode->data;
    delete(pHeadNode);
    pHeadNode = tempNode;
    length--;
    return true;
}

bool List::pop_back(string &data)
{
    if (this->is_empty())
        return false;
    if (length == 1)
    {
        delete(pHeadNode);
        pHeadNode = NULL;
        pEndNode = NULL;
        length--;
        return true;
    }
    Node *tempNode = pEndNode->prev;
    tempNode->next = NULL;
    data = pEndNode->data;
    delete(pEndNode);
    pEndNode = tempNode;
    length--;
    return true;
}

bool List::front(string &data)
{
    if (length == 0)
        return false;
    data = pHeadNode->data;
}

bool List::back(string &data)
{
    if (length == 0)
        return false;
    data = pEndNode->data;
}

bool List::is_empty()
{
    return length==0;
}

void List::list_print()
{
    cout<<"-------print--------"<<endl;
    if (pHeadNode == NULL)
        return;
    Node *iNode = pHeadNode;
    while (iNode != NULL)
    {
        cout<<iNode->data<<endl;
        iNode = iNode->next;
    }
}