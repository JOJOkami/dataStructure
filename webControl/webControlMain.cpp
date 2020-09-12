#include "webControl.h"
const int MAX_STACK = 10;
const int MAX_TEST_STACK = 10;
const int MIN_TEST_STACK = 10;

void showHtml(string htmlData)
{
    cout<<htmlData<<endl;
}

int main()
{
    webControl webControler(MAX_STACK);
    string inputData;
    string newHtml;
    string direction;
    while (true)
    {
        cout<<"请选择方向（d）/浏览新页面（n）:";
        std::cin>>inputData;
        if (inputData == "n")
        {
            // 浏览一个新的网页(清空前进栈,插入后退栈)
            cout<<"请输入要浏览的页面:";
            std::cin>>newHtml;
            webControler.searchNewPage(newHtml);
        }
        else if (inputData == "d")
        {
            // 后退一个页面
            cout<<"当前页面"<<webControler.getPresentPage()<<"请选择前进（f）/后退（b）:";
            string pageData;
            std::cin>>direction;
            if (direction == "f")
            {
                if (webControler.frontPage() == false)
                {
                    cout<<"没有前一页"<<endl;
                    continue;
                }
                cout<<webControler.getPresentPage()<<endl;
            }
            else if (direction == "b")
            {
                if (webControler.backPage() == false)
                {
                    cout<<"没有后一页"<<endl;
                    continue;
                }
                cout<<webControler.getPresentPage()<<endl;
            }
            else
            {
                cout<<"请输入合法字符"<<endl;
                continue;
            }
        }
        else
        {
            cout<<"请输入合法字符"<<endl;
            continue;
        }
    }
    return 0;
}