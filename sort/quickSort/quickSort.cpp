/*
    快排属于不稳定排序，但是大量数据情况下性能非常高，并且是原地排序算法，不会占用多余内存
    递归公式： quickSort(p..r) = quickSort(p..m-1) + quickSort(m+1..r)
    这里的+不是merge，而代表的只是子问题处理完了，父问题就解决了
    可以看出，快排是上层分好区了，子递归去处理剩下的问题
    归并排序是我上层等下层排好了，上层再把两个合起来

    再说快排能够不占用多余内存的关键（毕竟如果是用多的内存来实现，那还不如归并排序）
    假设有n个元素 x1,x2,x3,x4,x5我们把第一个点作为中间点value存放起来，
    [... 比value小的数据 ... i] value [... 比value大的数据 ...]
    用一个下标i从x5开始向左遍历,i表示等会排好后i是左边最后一个节点，我们要做的就是创造这个i，让这个i前面的值都是比value小的
    假设是 1 2 3 4 5 6，1被我们存起来为value作为比较点，i先为6，发现比1大那么，6这个位置就不是i，i向前移动，直到移动到1的位置，已经到头了，那么1的位置就是value的左边最近点，我们把1和1交换即可
    那如果是 4 1 2 3 5 6呢，4被我们存起来为value作为比较点，i移动到3的位置的时候发现3比4小，那么我们需要把一个比4大的替换过来，i才能继续前移，于是我们用j继续去前面找一个比4大数，找到4的位置都没有找到，那么刚刚i就是value的左边最近点，我们把i位置的数和4替换就会变成312456,4左边比4小，4右边比4大

    同理，我们也可以选最后面一个元素的位置，作为中间value，然后i从左向右遍历，找出来的i就是value右边最近的点
    [... 比value小的数据 ... ] value [i ... 比value大的数据 ...]

    快排没有归并排序的性能那么稳定，因为如果我们每次找的参照点在整个数组里面大小是最小或最大的话，就会照成递归的下一层还是要处理跟上层一样大的数据，时间复杂度退化成n^2
*/
#include <iostream>
using namespace std;

void swap(int &a, int &b)
{
    int temp = a;
    a = b;
    b = temp;
}

int partition(int *array, int p, int r, bool fun(int, int))
{
    int pivot = array[r - 1];
    int i = p; //i先假设为第一个点
    for (int j = p; j < r - 1; j++)
    {
        // 这个条件为true表示当前的i元素不配作为右边最近一点，i要加1
        // 注意这个函数判断>表示true，表示大到小排序
        if (fun(array[j], pivot))
        {
            if (i != j)
                swap(array[i], array[j]);
            i++;
        }
    }
    swap(array[r - 1], array[i]);
    return i;
}

void quickSort(int *array, int p, int r, bool fun(int, int))
{
    if (p >= r) return;
    int middle = partition(array, p, r, fun);
    quickSort(array, p, middle - 1, fun);
    quickSort(array, middle + 1, r, fun);
}

bool compare(int a, int b)
{
    if (a < b)
        return true;
}

int main()
{
    int array[10] = {2,1,6,7,8,10,4,9,1,0};
    quickSort(array, 0, 10, compare);
    for (int i = 0; i < 10; i++)
    {
        cout<<array[i]<<",";
    }
    
    return 0;
}