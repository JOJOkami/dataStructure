#include "webControl.h"

webControl::webControl(int maxLength)
{
    webFrontStack = new stack(maxLength);
    webBackStack = new stack(maxLength);
}

webControl::~webControl()
{
    delete webFrontStack;
    delete webBackStack;
}

bool webControl::searchNewPage(string newHtml)
{
    // 已经有在访问的页面，把在访问的页面存到后退栈
    if (!presentPage.empty())
    {
        webBackStack->push(presentPage);
    }
    presentPage = newHtml;

    // 清空前进栈，因为访问了新页面，没有前进的空间了
    webFrontStack->clear();
    return true;
}

bool webControl::frontPage()
{
    if (webFrontStack->isEmpty())
    {
        return false;
    }
    webBackStack->push(presentPage);     //当前页入后退栈
    presentPage = webFrontStack->pop();  //修改当前页为向前一页
    return true;
}

bool webControl::backPage()
{
    if (webBackStack->isEmpty())
    {
        return false;
    }
    webFrontStack->push(presentPage);   //当前页入前进栈
    presentPage = webBackStack->pop();  //修改当前页为后退一页
    return true;
}

string webControl::getPresentPage()
{
    return presentPage;
}