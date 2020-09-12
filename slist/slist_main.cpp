#include "link.h"
#include <iostream>
using namespace std;

void test1()
{
    SList link;
    link.push_back("test1");
    link.push_back("test2");
    link.push_back("test3");
    cout<<"尾部插入三个元素"<<endl;
    link.sListPrint();

    cout<<"头部插入三个元素"<<endl;
    link.push_front("test-1");
    link.push_front("test-2");
    link.push_front("test-3");
    link.sListPrint();

    cout<<"在第四个位置插入test0"<<endl;
    link.insert(4, "test0");
    link.sListPrint();

    cout<<"移除test0"<<endl;
    link.remove("test0");
    link.sListPrint();

    cout<<"移除头节点"<<endl;
    link.pop_front();
    link.sListPrint();

    cout<<"移除尾节点"<<endl;
    link.pop_back();
    link.sListPrint();

    cout<<"尾部释放所有节点"<<endl;
    while (!link.is_empty())
    {
        link.pop_back();
    }
    
    cout<<"头部插入1个元素"<<endl;
    link.push_front("test-1");

    cout<<"头部释放所有节点"<<endl;
    while (!link.is_empty())
    {
        link.pop_front();
    }

    cout<<"在空串第四个位置插入test0"<<endl;
    link.insert(4, "test0");
    link.sListPrint();
}

void test2()
{
    SList link;
    link.push_back("test1");
    link.push_back("test2");
    link.push_back("test3");
    cout<<"尾部插入三个元素"<<endl;
    link.sListPrint();

    cout<<"头部插入三个元素"<<endl;
    link.push_front("test-1");
    link.push_front("test-2");
    link.push_front("test-3");
    link.sListPrint();

    cout<<"反转前"<<endl;
    link.sListPrint();
    link.reverse();
    cout<<"反转后"<<endl;
    link.sListPrint();
}

int main(int argc, char *argv[])
{
    test2();
    return 0;
}