#include <string>
#include <iostream>
using std::string;
using std::cout;
using std::endl;

#ifndef __STACK__
#define __STACK__
class stack
{
private:
    string *items;  // 数组(顺序栈)
    int count;      // 栈中元素个数 
    int maxCount;   // 栈的大小
public:
    stack(int n);
    ~stack();
    bool push(string item);
    string pop();
    bool isEmpty();
    void clear();
};

#endif