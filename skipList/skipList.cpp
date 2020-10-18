#include <string>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <sstream>
#define MAX_LEVEL 15
#define PROBABILITY 3
#define ramdom(x) rand()%(x)
using namespace std;

/*
    跳表相对于红黑树结构简单得多
    坑：
    1、在进行插入的时候注意外循环的次数和内循环的终止条件
    2、注意new的使用，我们的结点空间申请必须在list内部完成，而不能由外部new提供
*/

template <typename T>
string to_string(T value)
{
	std::ostringstream os ;
	os << value ;
	return os.str() ;
}

class listNode
{
private:
    int m_key;
    string m_data;
    listNode *m_next;
    listNode *m_nextLevel;
public:
    listNode(int key, string data)
    {
        m_key = key;
        m_data = data;
        m_next = NULL;
        m_nextLevel = NULL;
    }
    ~listNode(){}
    listNode* getNext()
    {
        return m_next;
    }
    listNode* getLevelNext()
    {
        return m_nextLevel;
    }
    int getKey()
    {
        return m_key;
    }
    string getValue()
    {
        return m_data;
    }
    void setValue(string value)
    {
        m_data = value;
    }
    void setNext(listNode* newNode)
    {
        m_next = newNode;
    }
    void setNextLevel(listNode* newNode)
    {
        m_nextLevel = newNode;
    }
};

class SkipList
{
private:
    listNode *m_lpList[MAX_LEVEL];   //作为头节点，不用于存储数据
    int getRamdomLevel(int range);
public:
    void testSkipList();
    SkipList();
    ~SkipList();
    void insert(int key, string data);
    listNode *find(int key);
    void printAll();
};

SkipList::SkipList()
{
    for (int i = 0; i < MAX_LEVEL; i++)
        m_lpList[i] = new listNode(-1, "");

    for (int i = 0; i < MAX_LEVEL - 1; i++)
        m_lpList[i]->setNextLevel(m_lpList[i+1]);
}

void SkipList::testSkipList()
{
    listNode* testNode = m_lpList[0];

    while (testNode != NULL)
    {
        cout<<testNode->getKey()<<endl;
        testNode = testNode->getLevelNext();
    }
    
}


SkipList::~SkipList()
{
    for(int i = 0; i < MAX_LEVEL; i++)
    {
        listNode* nextNode;
        while(m_lpList[i] != NULL)
        {
            nextNode = m_lpList[i]->getNext();
            delete m_lpList[i];
            m_lpList[i] = nextNode;
        }
    }
}

int SkipList::getRamdomLevel(int range)
{
    int level = 1;
    for (int i = 1; i < range; i++)
    {
        if (ramdom(PROBABILITY) == 1)
        {
            level++;
        }
    }
    return level;
}

/*
    插入方法：
    1、设置一个记录点upNode记录上一层的节点
    2、从需要插入的层的头节点开始向后遍历，找到插入点并在插入点前一点向下移动
*/
void SkipList::insert(int key, string data)
{
    int level = getRamdomLevel(MAX_LEVEL);
    cout<<"插入层数"<<level<<endl;
    
    /* 查找插入点并插入节点 */
    listNode* currentNode = m_lpList[MAX_LEVEL-level-1];
    listNode* nextNode = NULL;
    listNode* upNode = NULL;    //插入结点的上层对应点
    for (int i = MAX_LEVEL-level-1; i < MAX_LEVEL; i++)
    {
        listNode *newNode = new listNode(key, data);
        cout<<"第"<<i<<"层插入元素:"<<newNode->getValue()<<endl;
        for(;;)
        {
            nextNode = currentNode->getNext();
            if (nextNode != NULL && nextNode->getKey() < key)
            {
                currentNode = nextNode;
                continue;
            }
            // 找到插入点在currentNode后
            newNode->setNext(nextNode);
            currentNode->setNext(newNode);

            // 若上一层有新增点，需要建立与当前新结点的联系
            if (upNode)
            {
                cout<<newNode->getValue()<<"连接上层插入的结点，进入下一层"<<endl;
                upNode->setNextLevel(newNode);
            }
            
            // 上一层结点赋值为新结点进入下一层
            upNode = newNode;
            currentNode = currentNode->getLevelNext();
            break;
        }
    }
}

/*
    查找元素的时候我们并不比较头结点
    currentNode作为比较点的上一个点用于前往下一层
*/
listNode* SkipList::find(int key)
{
    listNode* currentNode = m_lpList[0];
    for(int i = 0; i < MAX_LEVEL; i++)
    {
        listNode* nextNode;
        cout<<"查找第"<<i<<"层:"<<endl;
        while(currentNode != NULL)
        {
            nextNode = currentNode->getNext();
            if (nextNode == NULL || nextNode->getKey() > key)
            {
                currentNode = currentNode->getLevelNext();
                if (currentNode == NULL)
                {
                    cout<<"没有下一层，查找结束"<<endl;
                    return NULL;
                }
                cout<<"查找第"<<i<<"层无果，往下个"<<currentNode->getValue()<<"结点"<<endl;
                break;
            }
            else if (nextNode != NULL && nextNode->getKey() == key)
            {
                return nextNode;
            }
            currentNode = nextNode;
        }
    }
    return NULL;
}

void SkipList::printAll()
{
    for (int i = 0; i < MAX_LEVEL; i++)
    {
        listNode* nextNode;
        listNode* tempList = m_lpList[i];
        cout<<"第"<<i<<"层:";
        tempList = tempList->getNext();
        while (tempList)
        {
            nextNode = tempList->getNext();
            cout<<tempList->getKey()<<"-"<<tempList->getValue()<<",";
            tempList = nextNode;
        }
        cout<<endl;
    }
    
}

int main()
{
    srand((int)time(0));
    SkipList skipList;
    for(int i=1; i< 50; i++)
    {
        if((i%3) == 0)
        {
            skipList.insert(i, to_string(i));
        }
    }

    skipList.printAll();
    
    if (skipList.find(42))
        cout<<"查找到42元素"<<endl;
    
    if (skipList.find(3))
        cout<<"查找到3元素"<<endl;

    if (skipList.find(48))
        cout<<"查找到48元素"<<endl;
    
    if (skipList.find(148))
        cout<<"查找到148元素"<<endl;
    return 0;
}