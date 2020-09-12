#include <string>
using std::string;

/*
    思路2：使用tail == head作为是否满的标志，用(tail+1)%n == head作为是否空的标志，
           即尾部永远不能与头部重叠，这就意味着一定会空出一个元素空间来
    1、插入元素的时候用(tail+1)%n判断是否tail下一格为head，
       注意，我们这个判断是在插入数据(items[tail]=value)前的，
       所以items[tail]永远不会有数据，如果创建一个1个元素的循环队列，那么这个队列一个元素都插不进去
    2、删除元素的时候用head==tail判断是否为空
    3、head和tail向后移动的时候要做取模运算
    
    第2种思路会更优雅，因为相对于普通列表，仅仅只要修改判断条件tail==n(tail+1)%n == head
    就可以省去进行数据迁移的判断逻辑，而思路1要维护flag，逻辑较为复杂，不好理解
*/

class Queue2
{
private:
    int head;
    int tail;
    int maxCount;
    string *items;
public:
    Queue2(int);
    ~Queue2();
    bool enQueue(string);
    bool deQueue(string &);
    bool empty();
};

Queue2::Queue2(int n)
{
    maxCount = n;
    head = 0;
    tail = 0;
    items = new string[n];
}

Queue2::~Queue2()
{
    delete[] items;
}

bool Queue2::enQueue(string data)
{
    // 因为我们用这个判断使得插入数据不会使得head == tail成立
    // 只有删除数据head后移的使得head == tail成立
    // 所以我们才能用(head == tail)来判断循环列表为空
    if ((tail + 1) % maxCount == head)
        return false;
    items[tail] = data;
    tail = (tail + 1) % maxCount;
}

bool Queue2::deQueue(string &data)
{
    if (head == tail)
        return false;
    data = items[head];
    tail = (tail + 1) % maxCount;
}

bool Queue2::empty()
{
    return head == tail;
}