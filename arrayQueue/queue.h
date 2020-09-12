#include <string>
#define QUEUE_NULL 0
#define QUEUE_FULL 1
#define QUEUE_SPACE 2
using std::string;

/* 
    循环列表优势是不用做数据迁移
    思路1：不使用tail == head作为是否满的标志，因为循环列表在向后移动的过程是允许跳到第一位的
           所以删除头部元素和插入尾部元素都可以导致tail == head，这就是说满和空都是tail==head
    1、用一个flag表示空 满的状态
    2、插入元素的时候判断如果是tail == head，标记为满
    3、删除元素的时候判断如果是tail == head，标记为空
    4、head和tail向后移动的时候要做取模运算
    
    思路2：使用tail == head作为是否满的标志，用(tail+1)%n == head作为是否空的标志，
           即尾部永远不能与头部重叠，这就意味着一定会空出一个元素空间来[重点，tail和单向一样，指向的是最后一个的下一个]
    1、插入元素的时候用(tail+1)%n判断是否tail下一格为head，
       注意，我们这个判断是在插入数据(items[tail]=value)前的，
       所以items[tail]永远不会有数据，如果创建一个1个元素的循环队列，那么这个队列一个元素都插不进去
    2、删除元素的时候用head==tail判断是否为空
    3、head和tail向后移动的时候要做取模运算
    
    第2种思路会更优雅，因为相对于普通列表，仅仅只要修改判断条件tail==n(tail+1)%n == head
    就可以省去进行数据迁移的判断逻辑，而思路1要维护flag，逻辑较为复杂，不好理解
*/
class Queue
{
private:
    int head;
    int tail;
    int maxCount;
    int fullFlag; // 0为空 1为满 2为有空余
    string *items;
public:
    Queue(int);
    ~Queue();
    bool enQueue(string);
    bool deQueue(string &);
    bool empty();
};

Queue::Queue(int n)
{
    maxCount = n;
    head = 0;
    tail = 0;
    fullFlag = QUEUE_NULL;
    items = new string[n];
}

Queue::~Queue()
{
    delete[] items;
}

bool Queue::enQueue(string data)
{
    if (fullFlag == QUEUE_FULL)
        return false;
    if (fullFlag == QUEUE_NULL)
        fullFlag = QUEUE_SPACE;
    items[tail] = data;
    tail = ++tail % maxCount;
    if (tail == head)
        fullFlag = QUEUE_FULL;
    return true;
}

bool Queue::deQueue(string &data)
{
    if (fullFlag == QUEUE_NULL)
        return false;
    if (fullFlag == QUEUE_FULL)
        fullFlag = QUEUE_SPACE;
    data = items[head];
    head = ++head % maxCount;
    if (head == tail)
        fullFlag = QUEUE_NULL;
    return true;
}

bool Queue::empty()
{
    return fullFlag == QUEUE_NULL;
}