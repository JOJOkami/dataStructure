#include <iostream>
using namespace std;

void chooseSort(int *data, int n)
{
    for (int i=0; i < n-1; i++)
    {
        int j = i;
        int max = j;
        for (; j < n; j++)
        {
            if(data[j] > data[max])
                max = j;  //注意要记录的是最大值的下标
        }
        int temp = data[max];
        data[max] = data[i];
        data[i] = temp;
    }
}

int main()
{
    int data[10] = {1,2,3,4,4,3,2,1,9,8};
    chooseSort(data, 10);
    for (int i=0; i < 10; i++)
    {
        cout<<data[i]<<", ";
    }
    return 0;
}