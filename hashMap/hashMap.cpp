#include <iostream>
#include <list>
using namespace std;
#define EXPAND_NUM 2   //扩容倍数

// MapNode是使用者插入的键值对
class MapNode
{
private:
    string m_key;
    string m_data;
    MapNode *m_next;
public:
    MapNode(string, string);
    ~MapNode(){}
    void setData(string);
    void setNext(MapNode *);
    string getKey();
    MapNode* getNext();
    string getData();
};

MapNode::MapNode(string key="", string data="")
{
    m_key = key;
    m_data = data;
    m_next = NULL;
}

void MapNode::setData(string data)
{
    m_data = data;
}

string MapNode::getKey()
{
    return m_key;
}

string MapNode::getData()
{
    return m_data;
}

void MapNode::setNext(MapNode* newNode)
{
    m_next = newNode;
}

MapNode* MapNode::getNext()
{
    return m_next;
}

/*
    需求：
    1、解决散列冲突，这里使用链表法
    2、计算散列因子，插入新键值对过大时进行动态扩容
    3、支持功能：
        构造散列表可以设置初始大小，和临界装载因子
        插入新键值对，但是键不能冲突
        根据键删除键值对
        根据键获取值
*/

class HashMap
{
private:
    list<MapNode>* m_nMap;      // 用于动态扩容临时存放数据
    list<MapNode>* m_oMap;      // 用于存放Map的数据
    float m_loadFactor;         // 散列表的装载因子=填入表中的元素个数/散列表的长度
    int m_nCapacity;            // new总容量，用于计算装载因子
    int m_oCapacity;            // old总容量，用于计算装载因子
    int m_nCount;               // new表中当前元素数量，用于计算装载因子
    int m_oCount;               // old表中当前元素数量，用于计算装载因子

    int m_moveIndex;            // 指向有值结点,优化动态扩容数据转移时间复杂度
    void push(MapNode &list, MapNode *newNode);
    float getLoadFactor(int count, int capacity);
    int getHash(string data);
    
public:
    HashMap(int capacity, float loadFactor);
    ~HashMap();
    void insert(string key, string value);
    void remove(string key);
    string getValue(string key);
    void printAllOld();
    void printAllNew();
};

HashMap::HashMap(int capacity, float loadFactor)
{
    m_oMap = new list<MapNode>[capacity];
    m_nMap = NULL;
    m_oCount = 0;
    m_nCount = 0;
    m_oCapacity = capacity;
    m_nCapacity = 0;
    m_loadFactor = loadFactor;
}

HashMap::~HashMap()
{

}

float HashMap::getLoadFactor(int count, int capacity)
{
    return float(count) / float(capacity);
}

int HashMap::getHash(string data)
{
    int hashResult = 0;
    for (int i = 0; i < data.length(); i++)
    {
        hashResult += int(data[i]-'A');
    }
    return hashResult;
}

// 现在老map找，再到新map找
string HashMap::getValue(string key)
{
    int ix = getHash(key) % m_oCapacity;
    list<MapNode>::iterator itor = m_oMap[ix].begin();
    while(itor != m_oMap[ix].end())
    {
        if (itor->getKey() == key)
            return itor->getData();
        itor++;
    }
    
    if (m_nMap)
    {
        itor = m_nMap[ix].begin();
        while(itor != m_nMap[ix].end())
        {
            if (itor->getKey() == key)
                return itor->getData();
            itor++;
        } 
    }

    return "";
}

void HashMap::insert(string key, string value)
{
    MapNode newNode(key, value);
    cout<<"计算的装载因子为"<<getLoadFactor(m_oCount + 1, m_oCapacity)<<endl;
    if (m_nMap == NULL && getLoadFactor(m_oCount + 1, m_oCapacity) <= m_loadFactor)
    {
        m_oCount++;
        int tmpIndex = getHash(key) % m_oCapacity;
        if (tmpIndex < m_moveIndex)
            m_moveIndex = tmpIndex;
        m_oMap[tmpIndex].push_back(newNode);
        return;
    }

    if (m_nMap == NULL)
    {
        m_nCapacity = m_oCapacity * EXPAND_NUM;
        m_nMap = new list<MapNode>[m_nCapacity];
        m_nCount = 0;
    }
    
    m_nMap[getHash(key) % m_nCapacity].push_back(newNode);
    m_nMap[m_moveIndex] = m_oMap[m_moveIndex];
    m_nCount = m_nCount + m_nMap[m_moveIndex].size() + 1;

    while(!m_oMap[m_moveIndex].empty())
        m_oMap[m_moveIndex].pop_front();

    // 优化:寻找下一个m_moveIndex
    int i = m_moveIndex;
    for (; i < m_oCapacity; i++)
    {
        if (!m_oMap[m_moveIndex].empty())
        {
            m_moveIndex = i;
            break;
        }
    }

    // 若全部数据从老map转移到新map后，销毁老map
    if (i == m_oCapacity)
    {
        delete[] m_oMap;
        m_oMap = m_nMap;
        m_oCapacity = m_nCapacity;
        m_nCapacity = 0;
        m_nMap = NULL;
    }
}

void HashMap::printAllOld()
{
    cout<<"OLD"<<endl;
    for (size_t i = 0; i < m_oCapacity; i++)
    {
        cout<<i<<"-";
        list<MapNode>::iterator itor = m_oMap[i].begin();
        while(itor != m_oMap[i].end())
        {
            cout<<itor->getData()<<",";
            itor++;
        }
    }
    cout<<endl;
}

void HashMap::printAllNew()
{
    cout<<"NEW"<<endl;
    for (size_t i = 0; i < m_nCapacity; i++)
    {
        cout<<i<<"-";
        list<MapNode>::iterator itor = m_nMap[i].begin();
        while(itor != m_nMap[i].end())
        {
            cout<<itor->getData()<<",";
            itor++;
        }
    }
    cout<<endl;
}

void HashMap::remove(string key)
{

}

int main()
{
    HashMap map(10, 0.5);
    map.insert("testa", "testa");
    map.insert("testc", "testc");
    map.insert("testb", "testb");
    map.insert("testa", "testa");
    map.insert("teste", "teste");
    map.insert("testf", "testf");
    map.printAllOld();
    map.printAllNew();
    return 0;
}