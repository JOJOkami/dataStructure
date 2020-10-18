#include <iostream>
#include <string>
#include <queue>
#include <iomanip>
using namespace std;

/*
    红黑树性质
    1、结点由红黑两个颜色组成
    2、叶子结点为黑色（这是为了方便编程实现）
    3、根结点是黑色
    4、红色结点的左右结点必须为黑色（就是红色结点不是连续出现两个）
    5、对每一个结点，该结点到其后代叶子结点路径上的黑色结点数量都相等
    
    我们在普通的二叉树中实现了增删，而相对的红黑树这里要维护这几个属性，需要几个特殊的处理
    1、左/右旋
    2、变色
    3、结点要有红黑属性
    
    坑：
    1、在对结点进行左旋右旋的时候要先处理一对结点的关系后再处理另一对，避免错误
    2、赋值结点的父结点一定要判断这个结点为空，空结点不能设置其父结点
    3、打印结点没有设置层数，将空结点也打印出来，是无法将每层对应的下层点描述清楚的
    4、在左旋右旋之后结点的父子关系会发生变化要画图来判断以哪个点作为下一个处理点
*/

const int nodeLength = 5;
const int deep = 5;  //打印树的深度
enum Color
{
    BLACK = 0,
    RED = 1
};

typedef struct RBTreeNode
{
    int key;
    string value;
    Color color;
    struct RBTreeNode* left;
    struct RBTreeNode* right;
    struct RBTreeNode* parent;
    RBTreeNode()
    {
        key = -1;
        value = "nil";
        left = NULL;
        right = NULL;
        parent = NULL;
        color = BLACK;
    }
} RBTreeNode;

class RBTree
{
private:
    RBTreeNode *NILNode;
    RBTreeNode *root;
    // 判断是否是root结点
    bool isRootNode(RBTreeNode*);
    // 释放所有结点
    void eraseAllNode(RBTreeNode*);
    // 获取红黑树的后继结点
    RBTreeNode *getSucceedNode(RBTreeNode*);
    // 将y给x的双亲作为孩子代替x
    bool transplant(RBTreeNode*, RBTreeNode*);
    void _inorderPrint(RBTreeNode*);
    // 左旋
    void leftRotate(RBTreeNode*);
    // 右旋
    void rightRotate(RBTreeNode*);
    void _treePrint(RBTreeNode*);

public:
    RBTree();
    ~RBTree();
    // 判断是否是空结点
    bool isNILNode(RBTreeNode*);
    // 获取红黑树的最小结点值
    RBTreeNode* getMinNode(RBTreeNode*);
    // 获取红黑树的最大结点值
    RBTreeNode* getMaxNode(RBTreeNode*);
    // 删除红黑树的结点
    bool deleteNode(int);
    // 红黑树插入结点
    bool insertNewNode(int, string);
    // 获取红黑树的前驱结点
    RBTreeNode *getPreNode(RBTreeNode*);
    // 中序遍历红黑树
    void inorderPrint();
    void deleteFixUp(RBTreeNode*);
    void insertFixUp(RBTreeNode*);
    void treePrint();
    
};

RBTree::RBTree()
{
    NILNode = new RBTreeNode;
    root = NILNode;
}

RBTree::~RBTree()
{
    eraseAllNode(root);
}

bool RBTree::isRootNode(RBTreeNode *node)
{
    return (node->parent == NULL || isNILNode(node->parent));
}

bool RBTree::isNILNode(RBTreeNode *node)
{
    return (node->left == NULL && node->right == NULL);
}

void RBTree::eraseAllNode(RBTreeNode *node)
{
    if (isNILNode(node))
        return;
    eraseAllNode(node->left);
    eraseAllNode(node->right);
    delete node;
}

// 获取二叉搜索树的最小结点值
RBTreeNode* RBTree::getMinNode(RBTreeNode *node)
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
RBTreeNode* RBTree::getMaxNode(RBTreeNode *node)
{
    if (isNILNode(node))
        return NILNode;
    RBTreeNode* tempNode = node->right;
    while (!isNILNode(tempNode))
    {
        tempNode = tempNode->right;
    }
    return tempNode;
}

// x结点的双亲替换给y结点（注意没有改子结点）
bool RBTree::transplant(RBTreeNode *x, RBTreeNode *y)
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
    
    y->parent = x->parent;
    return true;
}

/*
    删除结点后调节平衡
    x为具有附加黑色属性的点
    如果x本身属性为红色，那么可以直接把自己改成黑色就平衡了
    一、x在父结点左边
    情况1: x的兄弟结点是红色，这个时候，我们没有办法把黑色属性往上传，要想办法把兄弟变黑，变成情况2
           方法：修改父节点为红色，兄弟结点为黑色，然后对x的父结点进行左旋
    情况2：x的兄弟结点是黑色
        情况2.1：兄弟结点的两个孩子都是黑色，这个时候我们就可以把兄弟结点的黑色和x的附加属性一起传到父结点了
                 兄弟结点被抽出了黑色属性变成红色，而x结点被抽走的是附加属性，不需要变色
                 然后我们的关注结点x自然就变成了他们的父亲结点了，进入下一个循环
        情况2.2：兄弟结点有孩子不是黑色
            情况2.2.1：兄弟的右孩子是黑色，左孩子是红色
                       兄弟结点改为红色，左孩子改成黑色，对兄弟结点进行一次右旋，这样就变成情况2.2.2了
            情况2.2.2：兄弟的右孩子是红色，这个情况比较特殊，因为如果右孩子是红色，
                       我们不需要再去把附加黑属性交给父结点了，
                       而是通过左旋转让附加属性到这个右孩子上面就可以解决问题，直接退出循环
                       方法：把兄弟结点改成父节点的颜色
                             把父节点改成黑色
                             把兄弟结点右孩子改成黑色
                             对父节点进行左旋
    【因为我们有哨兵结点，所以不需要处理 情况1 或 情况2.1 中那些孩子结点不存在的情况】
    二、
        反过来旋转即可，略
    
    通过上面的分析得出，我们可以把x本身属性不为黑色作为循环的终止条件
    另外，如果x已经指向了根结点，没有父结点可以上传属性了，也直接退出循环
    终止之后，直接把x设置为黑色
*/
void RBTree::deleteFixUp(RBTreeNode* x)
{
    while (x->color == BLACK && !isRootNode(x))
    {
        if (x->parent->left == x)
        {
            RBTreeNode *brotherNode = x->parent->right;
            // 兄弟结点是红色，转换为兄弟结点是黑色的情况
            if (brotherNode->color == RED)
            {
                x->parent->color = RED;
                brotherNode->color = BLACK;
                leftRotate(x->parent);
            }
            
            // 兄弟结点的两个孩子都是黑色
            if (brotherNode->left->color == BLACK && brotherNode->right->color == BLACK)
            {
                brotherNode->color = RED;
                x = x->parent;
            }
            else
            {
                // 兄弟结点的右孩子是黑色，转换为右孩子是红色的情况
                if (brotherNode->right->color == BLACK)
                {
                    brotherNode->left->color = BLACK;
                    brotherNode->color = RED;
                    rightRotate(brotherNode);
                }
                
                brotherNode->color = x->parent->color;
                x->parent->color = BLACK;
                brotherNode->right->color = BLACK;
                leftRotate(x->parent);
                x = root;   //退出循环
            }
        }
        else
        {
            RBTreeNode *brotherNode = x->parent->left;
            // 兄弟结点是红色，转换为兄弟结点是黑色的情况
            if (brotherNode->color == RED)
            {
                x->parent->color = RED;
                brotherNode->color = BLACK;
                rightRotate(x->parent);
            }
            
            // 兄弟结点的两个孩子都是黑色
            if (brotherNode->left->color == BLACK && brotherNode->right->color == BLACK)
            {
                brotherNode->color = RED;
                x = x->parent;
            }
            else
            {
                // 兄弟结点的左孩子是黑色，转换为左孩子是红色的情况
                if (brotherNode->left->color == BLACK)
                {
                    brotherNode->right->color = BLACK;
                    brotherNode->color = RED;
                    leftRotate(brotherNode);
                }
                
                brotherNode->color = x->parent->color;
                x->parent->color = BLACK;
                brotherNode->left->color = BLACK;
                rightRotate(x->parent);
                x = root;   //退出循环
            }
        }
    }
    x->color = BLACK;
}

/*
    删除二叉搜索树的结点x
    1 只有右子树，用右子树结点代替x
    2 只有左子树，用左子树结点代替x
    3 有双子，找到后继结点，后继结点必然没有左子树，所以用后继结点的右子树代替后继结点的位置，再用后继结点代替x
*/
bool RBTree::deleteNode(int key)
{
    if (isNILNode(root))
        return false;
    RBTreeNode *current = root;
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
    
    // 被删除点的颜色
    Color deleteColor = current->color;
    // 被删除点的子结点（实际被删除点必然只有一个孩子），用于做平衡调节
    RBTreeNode *x = current;
    // 开始删除结点
    if (isNILNode(current->left))
    {
        current->right->color = current->color;
        x = current->right;
        transplant(current, current->right);
    }
    else if (isNILNode(current->right))
    {
        current->left->color = current->color;
        x = current->left;
        transplant(current, current->left);
    }
    else
    {
        // 得到后继结点(后继结点一定没有左子结点)
        RBTreeNode* succeedNode = getMinNode(current->right);
        // succeedNode为实际被删除点
        x = succeedNode->right;
        deleteColor = succeedNode->color;
        
        if (current == succeedNode->parent)
        {
            // 算法导论中伪代码有这一步，必须有，因为如果x为nil节点，需要给这个nil设置父结点后面的调整才能正确执行
            x->parent = succeedNode;
        }
        else
        {
            // 后继结点右孩子顶替后继结点
            transplant(succeedNode, succeedNode->right);
            // 把删除节点的右子树继承给后继结点
            succeedNode->right = current->right;
            // 给节点赋值父节点，一定要判断是否为空结点
            if (!isNILNode(current->right))
                succeedNode->right->parent = succeedNode;
        }
        // 后继结点顶替删除节点
        transplant(current, succeedNode);
        succeedNode->color = current->color;
        // 把删除节点的左子树继承给后继结点
        succeedNode->left = current->left;
        if (!isNILNode(current->left))
        {
            succeedNode->left->parent = succeedNode;
        }
    }
    
    delete current;
    if (deleteColor == BLACK)
    {
        deleteFixUp(x);
    }
    return true;
}

/*
    插入新结点调整（因为插入本来就是在最后插入，所以情况比较少）
    情况1：插入点的父结点是黑色，不用做处理
    情况2：插入点的父结点是红色，叔叔结点为红色，这种情况把祖父结点设置为红色，把叔叔和父结点设置为黑色，继续关注祖父结点
    情况3：插入点的父结点是红色，叔叔结点为黑色或没有叔叔结点
        情况3.1：父节点是祖父结点的左结点
            情况3.1.1：插入点在父结点的左边
            情况3.1.2：插入点在父结点的右边
        情况3.2：父节点是祖父结点的右结点
            情况3.2.1：插入点在父结点的左边
            情况3.2.2：插入点在父结点的右边

    因此，我们可以设置一个循环条件，当前点的父结点是红色才需要继续循环处理
    因为每次循环都是让红色属性(我们的关注点就是这个点)上移，要么破坏性质3，要么破坏性质4
    当我们的红色属性点满足其父结点为黑色或者跑到根的时候，就只破坏了性质1，把根直接改成黑色即可
    所以，我们可以设置第二个循环条件，跑到根的时候停止循环

    while (当前点父结点是红色 and 当前非根节点)
    {
        if (关注点x的父结点是祖父结点的左子树)
            if (叔叔结点为红色)
                把祖父结点设置为红色
                把叔叔和父结点设置为黑色
                把关注点x设置为祖父结点
                【很简单的道理如果祖父结点改为红色没有跟上一个节点颜色冲突，就平衡了，
                如果冲突，用同样的思路(while)处理即可】
            else (叔叔结点为黑色)
                if (关注点x在父结点的右边)
                    关注点x设置为父节点
                    围绕x进行左旋
                    【目的是为了让两个红色的结点在同一条左边的直线上，这样可以通过祖父结点旋转后改颜色】
                把父亲结点改为黑色
                把祖父结点改为红色
                对祖父结点进行右旋
                【这是最后一步，这步之后必然平衡】
                【很简单的道理因为叔叔为黑色，而父结点为红色，所以祖父一定也是黑色的
                所以祖父和叔叔一定都是黑的，那么叔叔结点和祖父之间再插入一个红色结点也不会影响平衡
                这个else处理的就是通过旋转把红色结点弄到叔叔结点和祖父之间】
        else
            略，反过来旋转即可
    }
    root->color = BLACK
    【这步是必须的，当根被置为红色的时候不会进入循环】
*/
void RBTree::insertFixUp(RBTreeNode* x)
{
    while (x != root && x->parent->color == RED)
    {
        /* 
            不需要判断x->parent->parent为空的情况，
            如果x->parent->parent为空，表示已经在第2层，
            因为我们的根节点必然是黑色，不会进入循环
            如果x指向根并把它改为红色，也会被x!=root退出循环
        */
        
        if (x->parent == x->parent->parent->left)
        {
            RBTreeNode *uncleNode = x->parent->parent->right;
            if (uncleNode->color == RED)              //叔叔结点为红色，直接改颜色
            {
                x->parent->parent->color = RED;
                uncleNode->color = BLACK;
                x->parent->color = BLACK;
                x = x->parent->parent;
            }
            else
            {
                if (x->parent->right == x)            //祖父结点的左边父亲结点的右边，先左旋
                {
                    x = x->parent;                    //左旋之后x和parent的位置互换，所以需要修改x
                    leftRotate(x);
                }
                x->parent->color = BLACK;
                x->parent->parent->color = RED;
                rightRotate(x->parent->parent);
                x = root;                             //直接结束
            }
        }
        else
        {
            RBTreeNode *uncleNode = x->parent->parent->left;
            if (uncleNode->color == RED)
            {
                x->parent->parent->color = RED;
                uncleNode->color = BLACK;
                x->parent->color = BLACK;
                x = x->parent->parent;
            }
            else
            {
                if (x->parent->left == x)
                {
                    x = x->parent;
                    rightRotate(x);
                }
                x->parent->color = BLACK;
                x->parent->parent->color = RED;
                leftRotate(x->parent->parent);
                x = root;
            }
        }
    }
    root->color = BLACK;
}

// 二叉搜索树插入结点(插入点必然是最后的叶子结点)
bool RBTree::insertNewNode(int key, string value)
{
    RBTreeNode *current = root;     //遍历的当前结点
    RBTreeNode *y = NILNode;        //current的双亲结点y

    while (!isNILNode(current))
    {
        y = current;
        if (key < current->key)
        {
            current = current->left;
        }
        else    // 若key不可重复，可在这里加一个判断条件直接赋值return
        {
            current = current->right;
        }
    }

    // 插入新结点
    RBTreeNode *insertNode = new RBTreeNode;
    insertNode->value = value;
    insertNode->key = key;
    insertNode->left = NILNode;
    insertNode->right = NILNode;
    insertNode->parent = y;
    insertNode->color = RED;
    
    // 空树处理
    if (isNILNode(y))
    {
        root = insertNode;
    }
    else if (key < y->key)
    {
        y->left = insertNode;
    }
    else
    {
        y->right = insertNode;
    }
    insertFixUp(insertNode);
}

/*
    获取二叉搜索树的后继结点
    后继节点即树中比x大(或等于)，但是最接近x大小的结点
    
    当x(当前结点)的右子树不为空，则取右子树的小点为后继
    当x(当前结点)的右子树为空，向上找比他大的点
*/
RBTreeNode* RBTree::getSucceedNode(RBTreeNode* x)
{
    if (!isNILNode(x->right))
        return getMinNode(x->right);
    RBTreeNode *y = x->parent;
    // 这个条件可以循环出x是不是某一个上层结点的左子树中的一个点
    while (!isNILNode(y) && y->right == x)
    {
        x = y;
        y = y->parent;
    }
    return y;
}

// 获取二叉搜索树的前驱结点
RBTreeNode* RBTree::getPreNode(RBTreeNode* x)
{
    if (!isNILNode(x->left))
        return getMaxNode(x->left);
    RBTreeNode* y = x->parent;
    while (!isNILNode(y) && y->left == x)
    {
        x = y;
        y = y->parent;
    }
    return y;
}

// 打印二叉搜索树
void RBTree::treePrint()
{
    _treePrint(root);
}

// 打印二叉搜索树（广度优先搜索）
void RBTree::_treePrint(RBTreeNode* printRoot)
{
    int count = 0;       // 当前一行已经入队的元素个数
    int lineCount = 2;   // 一行满lineCount个元素就入队回车符
    int nowDeep = 0;
    queue<RBTreeNode*> printQueue;
    printQueue.push(printRoot);
    
    RBTreeNode* lineBreak = new RBTreeNode;
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
        cout<<setw(nodeLength)<<setfill(' ')<<printRoot->value;
        cout<<(printRoot->color == RED?"红":"黑")<<" ";
        
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

void RBTree::inorderPrint()
{
    _inorderPrint(root);
}

void RBTree::_inorderPrint(RBTreeNode* printRoot)
{
    if (isNILNode(printRoot))
        return;
    
    _inorderPrint(printRoot->left);
    cout<<printRoot->value<<" ";
    _inorderPrint(printRoot->right);
}

// 左旋（x是y的父结点，把y的左孩子给x作为右孩子，把x作为y的左孩子）
void RBTree::leftRotate(RBTreeNode* x)
{
    if (isNILNode(x) || isNILNode(x->right))
        return;
    RBTreeNode* y = x->right;
    
    //注意设置parent一定要判断是否为空结点
    if (!isNILNode(y->left))
        y->left->parent = x;
    x->right = y->left;
    y->parent = x->parent;
    y->left = x;
    
    //设置x的父结点的指向y，要判断根结点，这里很容易出错
    if (isNILNode(x->parent))
    {
        root = y;
    }
    else if (x->parent->left == x)
    {
        x->parent->left = y;
    }
    else
    {
        x->parent->right = y;
    }
    x->parent = y;
}

// 右旋
void RBTree::rightRotate(RBTreeNode* x)
{
    if (isNILNode(x) || isNILNode(x->left))
        return;
    RBTreeNode* y = x->left;
    
    if (!isNILNode(y->right))
        y->right->parent = x;
    x->left = y->right;
    y->parent = x->parent;
    y->right = x;
    if (isNILNode(x->parent))
    {
        root = y;
    }
    else if (x->parent->left == x)
    {
        x->parent->left = y;
    }
    else
    {
        x->parent->right = y;
    }
    x->parent = y;
}

int main()
{
    RBTree tree;
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
    cout<<"插入结点后的红黑树"<<endl;
    tree.treePrint();
    cout<<endl;
    
    cout<<"删除20结点"<<endl;
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
