#include <iostream>
using namespace std;

void quickSort(int *a, int head, int tail) {
    int low = head;
    int high = tail;
    int pivot = a[low];
    if (low < high) {
        while (low<high) {
            while (low < high && pivot <= a[high]) high--;
            a[low] = a[high];
            while (low < high && pivot >= a[low]) low++;
            a[high]=a[low];
        }
        a[low] = pivot;

        quickSort(a,head,low-1);
        quickSort(a,high+1,tail);
    }
}

int main()
{
    int array[10] = {2,1,6,7,8,10,4,9,1,0};
    quickSort(array, 0, 9);
    for (int i = 0; i < 10; i++)
    {
        cout<<array[i]<<",";
    }
    return 0;
}