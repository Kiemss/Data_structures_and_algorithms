#pragma once
#include<iostream>
#include<queue>

namespace data_structures::red_black_tree
{
    template<typename ElementType>
    class RedBlackTree
    {
    public:
        //构造函数
        RedBlackTree()
        : m_pRoot(nullptr)
        {}

        //析构函数->使用层序遍历
        ~RedBlackTree()
        {
            if (this->m_pRoot == nullptr) {return;}//空树，直接返回
            std::queue<Node*> que;
            que.push(this->m_pRoot);
            Node *cur;
            while (!que.empty())
            {
                cur = que.front();
                if (cur->left_child_ != nullptr)
                {
                    cur->left_child_->parent_ = nullptr;
                    que.push(cur->left_child_);
                }
                if (cur->right_child_ != nullptr) 
                {
                    cur->right_child_->parent_  = nullptr;
                    que.push(cur->right_child_);
                }
                que.pop();
                delete cur;
            }
            this->m_pRoot = nullptr;
            return;
        }
    
        //插入
        void insert(const ElementType &data)
        {
            if (m_pRoot == nullptr) 
            {
                this->m_pRoot = new Node(data);
                return;
            }
            else
            {
                Node *parent = nullptr;
                Node *cur = m_pRoot;
                while (cur != nullptr)
                {
                    if (data > cur->data_)//插入数据比节点数据大，右插
                    { 
                        parent = cur;
                        cur = cur->right_child_;
                    }
                    else if (data < cur->data_)//比节点数据小，左插
                    {
                        parent = cur;
                        cur = cur->left_child_;
                    }
                    else //和节点数据相同，返回，防止插入重复元素
                    {
                        return;
                    }
                }
                //设置当前节点的parent和颜色
                Node *new_node = new Node(data,parent,nullptr,nullptr,Color::RED);
                //用比较来决定左插还是右插
                if (parent->data_ > data) 
                {
                    parent->left_child_ = new_node;
                }
                else 
                {
                    parent->right_child_ = new_node;
                }

                //如果新插入的红色节点，父节点也是红色，不满足红黑树性质，进行插入调整操作
                if (Color::RED == get_color(parent))
                {
                    fix_after_insert(new_node);
                }
            }
        }

        //删除
        void remove(const ElementType &data)
        {
            if (this->m_pRoot == nullptr) {return;}//为空直接返回
            Node *cur = this->m_pRoot;
            while (cur != nullptr)
            {
                if (cur->data_ < data) //在右子树
                {
                    cur = cur->right_child_;
                }
                else if (cur->data_ > data) //在左子树
                {
                    cur = cur->left_child_;
                }
                else //找到了
                {
                    break;//可以将处理代码放进来的，但是为了防止嵌套过多，放下面去了
                }
            }

            if (cur == nullptr) {return;} //未找到：直接return

            //删除cur节点
            //左右子节点同时存在，用前驱或者后继覆盖，不是avl树不用判断用哪个
            if (cur->left_child_ != nullptr && cur->right_child_ != nullptr)
            {
                Node *pre = cur->left_child_;
                while (pre->right_child_ != nullptr)
                {
                    pre = pre->right_child_;
                }
                cur->data_ = pre->data_;
                cur = pre; //cur指向前驱节点 因为没有递归，为了统一处理，直接将cur指向pre
            }

            //删除cur指向的节点
            Node *child  = cur->left_child_;//child是被删除节点的子节点
            if (child == nullptr) {child = cur->right_child_;}
            
            if (child != nullptr)//存在左孩子或者右孩子
            {
                child->parent_ = cur->parent_;
                if (get_parent(cur) == nullptr) //cur节点是根
                {
                    this->m_pRoot = child;
                }
                else 
                {
                    //下面是链接
                    if (cur->parent_->left_child_ == cur)
                    {
                        cur->parent_->left_child_ = child;   
                    }
                    else
                    {
                        cur->parent_->right_child_ = child;
                    }
                }
                Color cur_color = get_color(cur); //记录cur的颜色
                delete cur;

                //如果删除的是黑色节点，要进行删除调整操作
                if (cur_color == Color::BLACK) 
                {
                    fix_after_remove(child);//？
                }
            }
            else //左右孩子均不存在(也就是删除的如果不是根节点就是叶子节点)
            {
                if (cur->parent_ == nullptr)//树中唯一的根节点被删除
                {   
                    delete cur;
                    this->m_pRoot = nullptr;
                    
                }
                else //待删除节点并不是根节点（是叶子节点）
                {
                    //删除了黑色节点，需要进行红黑树调整
                    if (get_color(cur) == Color::BLACK)
                    {
                        fix_after_remove(cur);
                    }
                   
                    //删除的不是叶节点⭐注意这个不能是上面的else,调整的目的就是变成这一步
                    if (cur->parent_->left_child_ == cur)
                    {
                        cur->parent_->left_child_ = nullptr;
                    }
                    else
                    {
                        cur->parent_->right_child_ = nullptr;
                    }
                    delete cur;
                }       
            }
        }

    private:
        enum class Color //枚举类，区分红黑
        {
            BLACK,
            RED,
        };

        struct Node //根节点
        {
            Node(const ElementType &data
                , Node *parent = nullptr
                , Node *left = nullptr
                , Node *right = nullptr
                , Color color = Color::BLACK)
            : data_(data)
            , left_child_(left)
            , right_child_(right)
            , parent_(parent)
            , color_(color)//默认用黑色初始化因为根节点就是黑色的
            {}
            ElementType data_;
            Node *left_child_;
            Node *right_child_;
            Node *parent_;  //指向当前节点的父节点
            Color color_;   //节点的颜色
        };
    
        Node* m_pRoot; //指向红黑树的根节点

        //获取节点颜色
        Color get_color(Node *node)
        {
            if (node == nullptr) {return Color::BLACK;}
            else {return node->color_;}
        }

        //设置节点颜色
        void set_color(Node *node,Color color)
        {
            if (node == nullptr) {return;}
            else {node->color_ = color;}
        }
    
        //获取节点左孩子
        Node* get_left(Node *node)
        {
            if (node == nullptr) {return nullptr;}
            else {return node->left_child_;}
        }

        //获取节点右孩子
        Node* get_right(Node *node)
        {
            if (node == nullptr) {return nullptr;}
            else {return node->right_child_;}
        }

        //获取节点父节点
        Node* get_parent(Node *node)
        {
            return node->parent_;
        }

        //左旋转->由于存在父亲节点，需要非常仔细(更新颜色不在函数内)
        void left_rotate(Node *node)
        {
            Node *child = node->right_child_; //该节点的右孩子
            child->parent_ = node->parent_;//优化：if外将子节点的父节点改为该节点的父节点(因为为空也没问题)
            //额外的判断1：确定该节点是父节点的左孩子还是右孩子
            if (node->parent_ != nullptr)//先判断父节点是否存在
            {
                if(node->parent_->left_child_ == node)//该节点是父节点的左孩子
                {
                    node->parent_->left_child_ = child;//更改父节点的左孩子
                }
                else //该节点是父节点的右孩子 
                {
                    node->parent_->right_child_ = child;
                }
            }
            else //父节点不存在（也就是该节点是根节点）
            {
                this->m_pRoot = child;//⭐如果node就是根节点，记得更改根节点的指向
            }
            
            node->right_child_ = child->left_child_;//将右节点的左孩子挂在该节点上
            //额外的修改2：右节点的左孩子的父节点改为该节点
            if (child->left_child_ != nullptr)
            {
                child->left_child_->parent_ = node;
            }
            child->left_child_ = node; //将该节点挂在该节点的左孩子上
            node->parent_ = child;//额外的修改2：将该节点的父节点改为右子节点
        }
    
        //右旋转->同左旋转😡
        void right_rotate(Node *node)
        {
            Node *child = node->left_child_;
            child->parent_ = node->parent_;
            if (node->parent_ != nullptr)
            {
                if (node->parent_->left_child_ == node)//是父节点的左孩子
                {
                    node->parent_->left_child_ = child;
                }
                else
                {
                    node->parent_->right_child_ = child;
                }
            }
            else 
            {
                this->m_pRoot = child;
            }
            node->left_child_ = child->right_child_;
            if (child->right_child_ != nullptr)
            {
                child->right_child_->parent_ = node;
            }
            
            child->right_child_ = node;
            node->parent_ = child;
        }
    
        //红黑树的插入调整函数
        void fix_after_insert(Node *node)
        {
            //只要父节点的颜色和该节点的颜色同时为红，则一直调整下去
            while (get_color(get_parent(node)) == Color::RED)
            {
                //插入的节点在左子树中
                if (get_left(get_parent(get_parent(node))) == get_parent(node))
                {
                    Node *uncle = get_right(get_parent(get_parent(node)));
                    if (Color::RED == get_color(uncle))//叔叔也是红色
                    {
                        set_color(get_parent(node), Color::BLACK);
                        set_color(uncle, Color::BLACK);
                        set_color(get_parent(get_parent(node)), Color::RED);
                        node = get_parent(get_parent(node)); //爷爷变红色，可能又违背性质，调整爷爷
                    }

                    else //叔叔是黑色
                    {
                        //先处理< >的情况，转成\ /的情况
                        if (get_right(get_parent(node))== node)
                        {
                            //⭐为确保和情况\ /统一，提前将node指向其父节点，这样旋转后就在底部了
                            node = get_parent(node);
                            left_rotate(node);
                            
                        }
                        //再处理处理\ /的情况
                        set_color(get_parent(node), Color::BLACK);
                        set_color(get_parent(get_parent(node)), Color::RED);
                        right_rotate(get_parent(get_parent(node)));
                        //处理完不会出现红红冲突，调整完毕
                        break;
                    }
                }   
                else //插入的节点在右子树中
                {
                    Node *uncle= get_left(get_parent(get_parent(node)));
                    if (get_color(uncle) == Color::RED)//叔叔是红色
                    {
                        set_color(get_parent(get_parent(node)), Color::RED);//爷爷变红
                        set_color(get_parent(node), Color::BLACK);//父节点变黑
                        set_color(uncle, Color::BLACK);//叔叔变黑
                        node = get_parent(get_parent(node));//node指向爷爷节点再次判定
                    }
                    else //叔叔黑色
                    {
                        //先处理>情况
                        if (get_left(get_parent(node)) == node)
                        {
                            node = get_parent(node);//⭐还是很重要的一步
                            right_rotate(node);
                        }
                        else //再处理\情况
                        {
                            set_color(get_parent(node), Color::BLACK);
                            set_color(get_parent(get_parent(node)), Color::RED);
                            left_rotate(get_parent(get_parent(node)));
                            break;
                        }
                    }
                }
            }

            //最后强制设置root节点为黑色
            set_color(this->m_pRoot, Color::BLACK);
        }
    
        //红黑树的删除调整函数
        void fix_after_remove(Node *node)//传入的节点为被删除节点
        {

            while (get_color(node) == Color::BLACK && node != this->m_pRoot)//⭐这个node不为根很容易漏！
            {
                //删除的黑色节点在左子树
                if (get_left(get_parent(node)) == node)  
                {
                    Node* brother = get_right(get_parent(node));
                    //最复杂的情况：兄弟节点为红色->通过变色旋转使其变黑
                    //对应234树种父节点为3键，但是234树中的兄弟节点跑到红黑树的兄弟节点的子节点去了，因此要将红黑树兄弟节点和234树兄弟节点统一
                    if (get_color(brother) == Color::RED) 
                    {
                        set_color(get_parent(node), Color::RED);
                        set_color(brother, Color::BLACK);
                        left_rotate(get_parent(node));
                        brother = get_right(get_parent(node));
                    }

                    
                    //情况3：兄弟节点为黑且兄弟节点的两个子节点都为黑色，无法变红->兄弟变红，双黑上移
                    if (get_color(brother->left_child_) == Color::BLACK
                        && get_color(brother->right_child_) == Color::BLACK)
                    {
                        set_color(brother, Color::RED);
                        node = get_parent(node);
                    }
                    else 
                    {
                        //只要兄弟节点的右节点不为红色，那就需要旋转成为红色的情况
                        if (get_color(get_right(brother)) != Color::RED)
                        {
                            set_color(brother, Color::RED);
                            set_color(get_left(brother), Color::BLACK);
                            right_rotate(brother);
                            brother = get_right(get_parent(node));
                        }
                        //最简单的情况：兄弟节点右节点为红色，直接旋转
                        if (get_color(get_right(brother)) == Color::RED)
                        {
                            //这个换颜色很容易漏
                            set_color(brother, get_color(get_parent(node)));//兄弟节点继承父节点的颜色：维持234树中父节点的键数
                            set_color(get_parent(node), Color::BLACK);//父节点变黑：转到另一边时将被删除的黑色补充回去
                            set_color(get_right(brother), Color::BLACK);//兄弟节点的子节点变黑：维持这一边的黑色数
                            left_rotate(get_parent(node));
                            break;
                        }
                    }
                }
                else //删除的黑色节点在右子树
                {
                    Node *brother = get_left(get_parent(node));
                    //case1:兄弟节点为红色(234树中的兄弟不是红黑树中的兄弟)
                    if (get_color(brother) == Color::RED) 
                    {
                        set_color(get_parent(node), Color::RED);
                        set_color(brother, Color::BLACK);
                        right_rotate(get_parent(node));
                        //容易漏：重设brother节点
                        brother = get_left(get_parent(node));
                    }
                    
                    //兄弟节点为黑色且兄弟节点的子节点均为黑色（兄弟节点自然为黑色，如果是红色也被上面改造成黑色了）
                    if (get_color(get_left(brother)) == Color::BLACK 
                        && get_color(get_right(brother)) == Color::BLACK)
                    {
                        set_color(brother, Color::RED);
                        node = get_parent(node);//双黑上移（兄弟也变红相当于整个父树都少了1个黑节点）
                    }
                    else //兄弟节点为黑色且存在红色子节点
                    {
                        //兄弟节点的左子树不为红->改造成左子树为红
                        if (get_color(get_left(brother)) != Color::RED)
                        {
                            set_color(brother, Color::RED);
                            set_color(get_right(brother), Color::BLACK);
                            left_rotate(brother);
                            brother = get_right(get_parent(node));
                        }
                        //兄弟节点的左子树为红
                        if (get_color(get_left(brother)) == Color::RED) 
                        {
                            set_color(brother, get_color(get_parent(node)));
                            set_color(get_parent(node), Color::BLACK);
                            set_color(get_left(brother), Color::BLACK);
                            right_rotate(get_parent(node));
                            break;
                        }
                    }
                }
            }

            //如果node指向的节点是红色，直接将这个节点涂成红色就补上了被删掉的黑色
            set_color(node, Color::BLACK);
        }
    };
}