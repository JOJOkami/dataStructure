#include "../arrayStack/stack.h"

#ifndef __WEB_CONTROL__
#define __WEB_CONTROL__
/*
    用了两个栈实现页面的前进后退功能
    问题：
    1、指定了栈的大小，如果栈满了就不能继续前进或者后退网页，这要怎么处理
    2、用了两个栈，配合的时候会不会出现一个可以pop一个不能push的情况
    【不会存在这种情况，因为我们对外只暴露了一个插入接口，这个插入接口在back满的情况下就不能继续插入，
    而front的数据只能从back来，两个栈的大小又一致】
    3、需要一个节点用于存储当前所在的页面，只有又出现新点的时候才放入栈中
*/

class webControl
{
private:
    stack *webFrontStack; // 实现前进
    stack *webBackStack;  // 实现后退
    string presentPage;   // 当前所在的页
public:
    webControl(int maxLength);
    ~webControl();
    bool searchNewPage(string);
    bool frontPage();
    bool backPage();
    string getPresentPage();
};

#endif