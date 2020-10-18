#include <iostream>
#include <string>
#include <queue>
#include <iomanip>
using namespace std;

/*
    二叉树有个特点
    中间点的左边子树的所有点都是比他小的
    右边子树的所有点都是大于等于他的
    这使得我们找后继结点和前驱结点的时候可以使用这个特性
*/
const int nodeLength = 3;
const int deep = 5;
typedef struct BaseTreeNode
{
    int key;
    string value;
    struct BaseTreeNode* left;
    struct BaseTreeNode* right;
    struct BaseTreeNode* parent;
    BaseTreeNode()
    {
        key = -1;
        value = "NU";
        left = NULL;
        right = NULL;
        parent = NULL;
    }
} BaseTreeNode;

class BaseTree
{
private:
    BaseTreeNode *NILNode;
    BaseTreeNode *root;
    // 判断是否是root结点
    bool isRootNode(BaseTreeNode*);
    // 释放所有结点
    void eraseAllNode(BaseTreeNode*);
    // 获取二叉搜索树的后继结点
    BaseTreeNode *getSucceedNode(BaseTreeNode*);
    // 将y给x的双亲作为孩子代替x
    bool transplant(BaseTreeNode*, BaseTreeNode*);
    // 逐层打印二叉搜索树
    void _treePrint(BaseTreeNode*);
    // 中序打印二叉搜索树
    void _inorderPrint(BaseTreeNode*);
public:
    BaseTree();
    ~BaseTree();
    // 判断是否是空结点
    bool isNILNode(BaseTreeNode*);
    // 获取二叉搜索树的最小结点值
    BaseTreeNode* getMinNode(BaseTreeNode*);
    // 获取二叉搜索树的最大结点值
    BaseTreeNode* getMaxNode(BaseTreeNode*);
    // 删除二叉搜索树的结点
    bool deleteNode(int);
    // 二叉搜索树插入结点
    bool insertNewNode(int, string);
    // 获取二叉搜索树的前驱结点
    BaseTreeNode *getPreNode(BaseTreeNode*);
    // 逐层打印二叉搜索树
    void treePrint();
    // 中序打印二叉搜索树
    void inorderPrint();
};

BaseTree::BaseTree()
{
    NILNode = new BaseTreeNode;
    root = NILNode;
}

BaseTree::~BaseTree()
{
    eraseAllNode(root);
}

bool BaseTree::isRootNode(BaseTreeNode *node)
{
    return (node->parent == NULL || isNILNode(node->parent));
}

bool BaseTree::isNILNode(BaseTreeNode *node)
{
    return (node->left == NULL && node->right == NULL);
}

void BaseTree::eraseAllNode(BaseTreeNode *node)
{
    if (isNILNode(node))
        return;
    eraseAllNode(node->left);
    eraseAllNode(node->right);
    delete node;
}

// 获取二叉搜索树的最小结点值
BaseTreeNode* BaseTree::getMinNode(BaseTreeNode *node)
{
    if (isNILNode(node))
        return NILNode;
    while (!isNILNode(node->left))
    {
        node = node->left;
    }
    return node;
}

// 获取二叉搜索树的最大结点值
BaseTreeNode* BaseTree::getMaxNode(BaseTreeNode *node)
{
    if (isNILNode(node))
        return NILNode;
    BaseTreeNode* tempNode = node->right;
    while (!isNILNode(tempNode))
    {
        tempNode = tempNode->right;
    }
    return tempNode;
}

// x结点的双亲替换给y结点（注意没有改子结点）
bool BaseTree::transplant(BaseTreeNode *x, BaseTreeNode *y)
{
    if (x == NULL || y == NULL)
    {
        return false;
    }
    if (isRootNode(x))
    {
        root = y;
        y->parent = NILNode;
        return true;
    }
    
    if (x->parent->left == x)
    {
        x->parent->left = y;
    }
    else
    {
        x->parent->right = y;
    }
    
    // 注意这里一定要判断y是否为空结点，我们只有一个空结点，不能设置空结点的双亲结点
    if (!isNILNode(y))
        y->parent = x->parent;
    return true;
}

/*
    删除二叉搜索树的结点x
    1 只有右子树，用右子树结点代替x
    2 只有左子树，用左子树结点代替x
    3 有双子，找到后继结点，后继结点必然没有左子树，所以用后继结点的右子树代替后继结点的位置，再用后继结点代替x
*/
bool BaseTree::deleteNode(int key)
{
    if (isNILNode(root))
        return false;
    BaseTreeNode *current = root;
    while (!isNILNode(current) && key != current->key)
    {
        if (key > current->key)
        {
            current = current->right;
        }
        else if (key < current->key)
        {
            current = current->left;
        }
        else
        {
            break;
        }
    }
    if (key != current->key)
        return false;
    
    // 开始删除结点
    if (isNILNode(current->left))
    {
        transplant(current, current->right);
    }
    else if (isNILNode(current->right))
    {
        transplant(current, current->left);
    }
    else
    {
        /*
           如果后继节点不是删除节点的子结点，一定在子结点的左子树
           先用后继节的右子结点顶替后继结点
           再用后继结点顶替删除节点，把删除节点的左右子树都继承给后继结点
        */
        BaseTreeNode* succeedNode = getMinNode(current->right);
        if (current != succeedNode->parent)
        {
            // 后继结点右孩子顶替后继结点
            transplant(succeedNode, succeedNode->right);
            // 把删除节点的右子树继承给后继结点
            succeedNode->right = current->right;
            if (!isNILNode(current->right))
                succeedNode->right->parent = succeedNode;
        }
        // 后继结点顶替删除节点
        transplant(current, succeedNode);
        // 把删除节点的左子树继承给后继结点
        succeedNode->left = current->left;
        if (!isNILNode(current->left))//!!!所有设置父结点的都要注意判断
        {
            succeedNode->left->parent = succeedNode;
        }
    }
    delete current;
    return true;
}


// 二叉搜索树插入结点(插入点必然是最后的叶子结点)
bool BaseTree::insertNewNode(int key, string value)
{
    BaseTreeNode *x = root;     //遍历的当前结点
    BaseTreeNode *y = NILNode;  //x的双亲结点y

    while (!isNILNode(x))
    {
        y = x;
        if (key < x->key)
        {
            x = x->left;
        }
        else
        {
            x = x->right;
        }
    }
    
    // 空树处理
    if (isNILNode(y))
    {
        root = new BaseTreeNode;
        root->left = NILNode;
        root->right = NILNode;
        root->parent = NILNode;
        root->key = key;
        root->value = value;
        return true;
    }
    
    // 插入新结点
    BaseTreeNode *insertNode = new BaseTreeNode;
    insertNode->value = value;
    insertNode->key = key;
    insertNode->left = NILNode;
    insertNode->right = NILNode;
    if (key < y->key)
    {
        y->left = insertNode;
        insertNode->parent = y;
    }
    else
    {
        y->right = insertNode;
        insertNode->parent = y;
    }
}

/*
    获取二叉搜索树的后继结点
    后继节点即树中比x大(或等于)，但是最接近x大小的结点
    
    当x(当前结点)的右子树不为空，则取右子树的小点为后继
    当x(当前结点)的右子树为空，向上找比他大的点
*/
BaseTreeNode* BaseTree::getSucceedNode(BaseTreeNode* x)
{
    if (!isNILNode(x->right))
        return getMinNode(x->right);
    BaseTreeNode *y = x->parent;
    // 这个条件可以循环出x是不是某一个上层结点的左子树中的一个点
    while (!isNILNode(y) && y->right == x)
    {
        x = y;
        y = y->parent;
    }
    return y;
}

// 获取二叉搜索树的前驱结点
BaseTreeNode* BaseTree::getPreNode(BaseTreeNode* x)
{
    if (!isNILNode(x->left))
        return getMaxNode(x->left);
    BaseTreeNode* y = x->parent;
    while (!isNILNode(y) && y->left == x)
    {
        x = y;
        y = y->parent;
    }
    return y;
}

// 打印二叉搜索树
void BaseTree::treePrint()
{
    _treePrint(root);
}

// 打印二叉搜索树（广度优先搜索）
void BaseTree::_treePrint(BaseTreeNode* printRoot)
{
    int count = 0;       // 当前一行已经入队的元素个数
    int lineCount = 2;   // 一行满lineCount个元素就入队回车符
    int nowDeep = 0;
    queue<BaseTreeNode*> printQueue;
    printQueue.push(printRoot);
    
    BaseTreeNode* lineBreak = new BaseTreeNode;
    lineBreak->value = "\n";
    printQueue.push(lineBreak);
    
    while (!printQueue.empty() && nowDeep < deep)
    {
        printRoot = printQueue.front();
        printQueue.pop();
        
        // 打印
        if (printRoot->value == "\n")
        {
            cout<<setw(nodeLength)<<setfill(' ')<<printRoot->value;
            continue;
        }
        cout<<setw(nodeLength)<<setfill(' ')<<printRoot->value<<" ";
        
        // 插入后续要打印的结点
        if (!isNILNode(printRoot))
        {
            if (!isNILNode(printRoot->left))
                printQueue.push(printRoot->left);
            else
                printQueue.push(NILNode);
            if (!isNILNode(printRoot->right))
                printQueue.push(printRoot->right);
            else
                printQueue.push(NILNode);
        }
        else
        {
            printQueue.push(NILNode);
            printQueue.push(NILNode);
        }
        count+=2;
        if (count == lineCount)
        {
            printQueue.push(lineBreak);
            count = 0;
            lineCount *= 2;
            nowDeep++;
        }
    }
}

void BaseTree::inorderPrint()
{
    _inorderPrint(root);
}

void BaseTree::_inorderPrint(BaseTreeNode* printRoot)
{
    if (isNILNode(printRoot))
        return;
    
    _inorderPrint(printRoot->left);
    cout<<printRoot->value<<" ";
    _inorderPrint(printRoot->right);
}

int main()
{
    BaseTree tree;
    tree.insertNewNode(20, "20");
    tree.insertNewNode(10, "10");
    tree.insertNewNode(19, "19");
    tree.insertNewNode(30, "30");
    tree.insertNewNode(40, "40");
    tree.insertNewNode(25, "25");
    tree.insertNewNode(40, "*40");
    tree.insertNewNode(25, "*25");
    tree.insertNewNode(29, "29");
    tree.insertNewNode(39, "39");
    tree.treePrint();
    cout<<endl;
    
    tree.deleteNode(20);
    tree.treePrint();
    cout<<endl;
    
    tree.deleteNode(10);
    tree.treePrint();
    cout<<endl;
    
    tree.deleteNode(40);
    tree.treePrint();
    cout<<endl;
    return 0;
}