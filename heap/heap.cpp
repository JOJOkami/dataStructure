#include <iostream>
using namespace std;

/*
    堆排序需要的数据结构比较简单，因为是完全二叉树，所以可以用数组存储数据
    【只需要满足父节点>=子结点就可以作为最大堆，堆的重点在于调整堆的部分】
    从上向下调整：处理删除的情况
    从下向上调整：处理插入的情况

    比较快排和堆排序，快速排序更优秀，原因在于：
    1、堆排序比快速排序的交换次数多，因为堆排序要建堆，原本就排好序（有序度高）被堆化后有序度反而降低了
    2、快速排序是左到右进行查找的，顺序访问内存对于CPU更加友好

    注意：
    1、堆需要一个count用于记录当前的位置，以及一个m_maxCount记录容量，插入的时候进行判断
    2、插入和删除的时候while使用的退出条件都比较巧妙，要顾及容量和大小比较
    3、我用m_count表示当前的数组下标，需要用-1表示空，处理元素的时候要注意要覆盖到[m_count]
*/

class Heap
{
private:
    int *m_dataArray;
    int m_count;
    int m_maxCount;
public:
    Heap(int capacity);
    Heap(int a[], int length);
    void heapify(int a[], int i, int length);
    ~Heap();
    bool insert(int newNode);
    bool removeMax();
    void sort(int* &result, int &length);
    void printAll();
};

Heap::Heap(int capacity)
{
    m_dataArray = new int[capacity];
    m_maxCount = capacity - 1;
    m_count = -1;
}

/*
    将数组堆化，从下向上逐层处理有子结点的结点
    [不能从上到下逐层处理，父结点的左右结点不一定就是全局最大，从上到下处理，处理过的点就不会被重新比较了]
    有子结点的结点处理方法跟删除类似，自顶向下堆化
*/
Heap::Heap(int a[], int length)
{
    int endIndex = length - 1;
    for (int i = (endIndex-1)/2; i >= 0; i--)
    {
        heapify(a, i, endIndex);
    }

    // 将堆化后的数组赋值到类成员变量
    m_count = endIndex;
    m_maxCount = endIndex;
    m_dataArray = new int[length];
    for (int i = 0; i < length; i++)
    {
        m_dataArray[i] = a[i];
    }
}

/*
    对i结点进行堆化，endIndex为数组最后一位下标
*/
void Heap::heapify(int a[], int i, int endIndex)
{
    while (true)
    {
        int maxChild = i;
        
        if (i*2+1 <= endIndex && a[i*2+1] > a[i])
            maxChild = i*2+1;

        if (i*2+2 <= endIndex && a[i*2+2] > a[maxChild])
            maxChild = i*2+2;
        
        if (i == maxChild)
            break;
        
        swap(a[maxChild], a[i]);
        i = maxChild;
    }  
}

Heap::~Heap()
{
    delete[] m_dataArray;
}

// 自底向上
bool Heap::insert(int data)
{
    if (m_count == m_maxCount)
        return false;
    m_dataArray[++m_count] = data;
    
    // 比较父节点，若比父节点大，则交换，直到根结点[0]
    int i = m_count;
    while ((i-1)/2 >= 0 && m_dataArray[i] > m_dataArray[(i-1)/2])
    {
        swap(m_dataArray[i], m_dataArray[(i-1)/2]);
        i = (i-1)/2;
    }
    return true;
}

bool Heap::removeMax()
{
    if (m_count == -1)
        return false;
    swap(m_dataArray[0], m_dataArray[m_count]);
    m_count--;
    heapify(m_dataArray, 0, m_count);
    return true;
}

/*
    堆排序
    【
        这和删除结点十分相似，
        但是删除结点只需处理一个删除，并且修改m_count值
        排序需要每次删除max结点直到全部删除，则排序完成
    】
    1、建堆(在Heap类中，直接使用已经构建的堆m_dataArray即可)
    2、逐个删除max放到数组最后，进行堆调整
    复杂度：堆调整时间logn * n
*/
void Heap::sort(int* &result, int &length)
{
    length = m_count + 1;
    result = new int[m_count + 1];
    for (int i = 0; i <= m_count; i++)
    {
        result[i] = m_dataArray[i];
    }
    
    for (int i = m_count; i >= 0; i--)
    {
        swap(result[i], result[0]);
        heapify(result, 0, i-1);
    }
}

void Heap::printAll()
{
    int *tmp = m_dataArray;
    for (int i = 0; i <= m_count; i++)
    {
        cout<<tmp[i]<<",";
    }
    cout<<endl;
}

int main()
{
    Heap testHeap(10);
    testHeap.insert(9);
    testHeap.insert(99);
    testHeap.insert(999);
    testHeap.insert(2);
    testHeap.insert(80);
    testHeap.insert(19);
    testHeap.insert(99);
    testHeap.insert(100);
    testHeap.insert(99);
    testHeap.insert(99);

    if (testHeap.insert(99) == false)
        cout<<"插入失败"<<endl;
    
    testHeap.printAll();

    cout<<"对元素进行堆排序后"<<endl;
    int *sortArray;
    int length;
    testHeap.sort(sortArray, length);
    for (int i = 0; i < length; i++)
    {
        cout<<sortArray[i]<<",";
    }
    cout<<endl;

    for (int i = 0; i < 11; i++)
    {
        if (testHeap.removeMax())
        {
            cout<<"删除最大元素后:";
            testHeap.printAll();
        }
        else
        {
            cout<<"删除"<<i<<"失败"<<endl;
        }
    }

    cout<<"对排序后的数组重新生成堆"<<endl;
    Heap test2Heap(sortArray, length);
    test2Heap.printAll();
    return 0;
}