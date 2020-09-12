#include "list.h"

int main()
{
    List list;
    string data;
    list.push_back("test1");
    list.push_front("test0");
    list.push_back("test2");
    list.insert_perv(list.find("test1"), "test0.5");
    list.insert_next(list.find("test1"), "test1.5");
    list.list_print();

    list.front(data);
    cout<<"first point "<<data<<endl;
    list.back(data);
    cout<<"last point "<<data<<endl;

    list.pop_back(data);
    cout<<"pop back "<<data<<endl;
    list.pop_front(data);
    cout<<"pop front "<<data<<endl;

    cout<<"erase point "<<list.find("test1.5")->data<<endl;
    list.erase(list.find("test1.5"));
    list.list_print();
    while (!list.is_empty())
    {
        list.pop_back(data);
    }
    list.list_print();

    cout<<"NULL test"<<endl;
    list.erase(NULL);
    list.find("22");
    list.front(data);
    list.back(data);
    list.pop_back(data);
    list.pop_front(data);
    return 0;
}