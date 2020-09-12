/*
    插入排序复杂度是O(n^2)，因为有两层循环，但是插入排序有很多优化方法，所以比冒泡排序实用得多
*/

#include <iostream>
using namespace std;

int *insertSort(int *data, int n)
{
    for (int i=1; i < n; i++)
    {
        int j = i - 1;
        int value = data[i]; //注意，我们这里需要一个空间用来存储要插入的元素，这样才能空出一个元素的空间给前面的元素后移
        while (j >= 0)
        {
            if(value > data[j])
            {
                data[j+1] = data[j];
            }
            else
            {
                break; //注意，这里如果没有达成条件即已经找到插入位置,break之后j+1就是对应位置，也就是说不论什么条件退出的循环，插入位置都是j+1
            }
            j--;
        }
        data[j+1] = value;
    }
}

int main()
{
    int data[10] = {1,2,3,4,4,3,2,1,9,8};
    insertSort(data, 10);
    for (int i=0; i < sizeof(data)/sizeof(int); i++)
    {
        cout<<data[i]<<", ";
    }
    return 0;
}