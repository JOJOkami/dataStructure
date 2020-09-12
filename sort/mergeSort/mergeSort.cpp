#include <iostream>
using namespace std;

#define MAX_NUM 0x7fffffff;

// 写递归代码的技巧就是，分析得出递推公式，然后找到终止条件，最后将递推公式翻译成递归代码。

// result = merge_two_array(merge(start, middle) + merge(middle, end))
// 终止条件时array长度为1

/*
C数组定义时要指定大小。
当然，万事也不是绝对的。例如：
1.作为形参的时候，可以不指定，因为在函数调用的时候，数组会转为指针的。
2.当为字符串数组时，char ST[]="hello word"这儿定义了一个ST字符数值，不需要你自己去指定大小，系统会自动生成长度为11.
*/
void mergeTwoArray(int *result, int start, int middle, int end)
{
	int *tmpArray = new int[end - start + 1];
	
	// 比较start->middle， middle->end的数据
	int tmpIndex = 0, i = start, j = middle;
	while (i <= middle - 1 and j <= end)  //这里注意middle是右边数组的开始下标，所以要减1
	{
		if (result[i] <= result[j])
			tmpArray[tmpIndex++] = result[i++];
		else
			tmpArray[tmpIndex++] = result[j++];
	}
	
	while (i <= middle - 1)
		tmpArray[tmpIndex++] = result[i++];
	
	while (j <= end)
		tmpArray[tmpIndex++] = result[j++];
	
	for (int i = 0; i < tmpIndex; i++)
		result[i + start] = tmpArray[i];
	
	delete[] tmpArray;
}

void mergeTwoArray2(int *result, int start, int middle, int end)
{
	// 用哨兵处理边际值，省掉两个收尾的循环（其实就是把原本在result里的数据拿出来放到新建的两个数组，尾部加一个最大的元素）
	int leftLen = middle - start;
	int *tmpArrayLeft = new int[leftLen + 1];
	tmpArrayLeft[leftLen] = MAX_NUM;
	for (int i = 0; i < leftLen; i++)
		tmpArrayLeft[i] = result[i + start];
	
	int rightLen = end - middle + 1;
	int *tmpArrayRight = new int[rightLen + 1];
	tmpArrayRight[rightLen] = MAX_NUM;
	for (int i = 0; i < rightLen; i++)
		tmpArrayRight[i] = result[i + middle];
	
	// 这里有个思想的转换，不用哨兵的时候，我们的tmp是用来临时存放的，最后还需要重新放回result
	// 用哨兵后我们用两个tmp来存放左右两个数组，所以只需要拷贝一次到result，这也是一个优化
	int lIndex = 0, rIndex = 0;
	for (int i = start; i <= end; i++)
	{
		if (tmpArrayLeft[lIndex] <= tmpArrayRight[rIndex])
			result[i] = tmpArrayLeft[lIndex++];
		else
			result[i] = tmpArrayRight[rIndex++];
	}
	
	delete[] tmpArrayLeft;
	delete[] tmpArrayRight;
}

void mergeSortReal(int * array, int start, int end)
{
	// 递归终止条件(会有start大于end的情况，因为middle-1)
	if (start >= end)
		return;
	int middle = (start + end) / 2;//这里是重点，如果是偶数除以2之后得到的下标是中间靠前的，如果是奇数除以2之后得到的下标是正中间的
	mergeSortReal(array, start, middle);//如果用middle-1会导致只有两个元素的时候，后半部分永远会有一个元素，进入死循环
	mergeSortReal(array, middle + 1, end);
	mergeTwoArray2(array, start, middle + 1, end);
}

int* mergeSort(int *array, int n)
{
	mergeSortReal(array, 0, n-1); //这里注意必须-1，end是存在的元素下标
	return array;
}

int main()
{
	int array[10] = {2,6,1,2,3,9,7,78,6,1};
	mergeSort(array, 10);
	for (int i = 0; i < 10; i++)
	{
		cout<<array[i]<<",";
	}
	return 0;
}