/*
    冒泡排序法思路即n个数据遍历n次，然后每次把最大或最小的一个数据放到最后时间复杂度为n^2
    本来想用list作为底层数据结构，发现使用list作为底层数据结构，有两点限制：
    1、list 为双向迭代器，故不支持it+=i
    2、list 不支持下标访问和at方法访问
    这使得我们不能对冒泡排序进行优化
*/
#include <iostream>
#include <list>
#include <string>
using namespace std;


void bubbleSort(list<int> &bubbleSortList, bool fun(int, int))
{
    list<int>::iterator iter = bubbleSortList.begin();
    
    for (; iter != bubbleSortList.end(); iter++)
    {
        list<int>::iterator iter2 = bubbleSortList.begin();
        for (; iter2 != bubbleSortList.end(); iter2++)      //这里不能用end-n，因为是list..
        {
            if (fun(*iter2, *iter))
            {
                int temp = *iter2;
                *iter2 = *iter;
                *iter = temp;
            }
        }
    }
}

bool compare(int a, int b)
{
    return a > b;
}

int main()
{
    list<int> bubbleSortList(4);
    bubbleSortList.push_back(1);
    bubbleSortList.push_back(6);
    bubbleSortList.push_back(3);
    bubbleSortList.push_back(0);
    bubbleSort(bubbleSortList, compare);
    list<int>::iterator iter = bubbleSortList.begin();
    
    for (; iter != bubbleSortList.end(); iter++)
    {
        cout<<*iter<<endl;
    }
}