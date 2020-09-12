#include "stack.h"

stack::stack(int n)
{
    items = new string[n];
    count = 0;
    maxCount = n;
}

stack::~stack()
{
    delete[] items;
}

// 要考虑空间不够的情况
bool stack::push(string item)
{
    if (count == maxCount)
        return false;
    items[count] = item;
    count++;
    return true;
}

/*
    易错点：这里的count是数量而不是下标，所以我们在pop的时候必须要先对count-1
            在push的时候则是先赋值再count+1
*/
string stack::pop()
{
    if (count == 0)
        return "";
    count--;
    return items[count];
}

bool stack::isEmpty()
{
    return count==0;
}

void stack::clear()
{
    count=0;
}