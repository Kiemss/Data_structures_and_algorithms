#pragma once
#include<iostream>
#include<algorithm>
#include<stack>
#include<cmath>

namespace data_structures::avl_tree
{
    template<typename ElementType,typename Compare = std::less<ElementType>>
    class AVLTree
    {
    private:
        struct Node;
    public:
        //构造函数
        AVLTree(const Compare& comp = Compare())
        : m_compare(comp)
        , m_pRoot(nullptr)
        {}

        //析构函数->使用层序遍历
        ~AVLTree()
        {
            if (m_pRoot == nullptr) {return;}
            std::queue<Node*> que;
            que.push(this->m_pRoot);
            Node* node {};
            while (!que.empty())
            {
                node = que.front();
                que.pop();
                if (node->left_child_ != nullptr) {que.push(node->left_child_);}
                if (node->right_child_ != nullptr) {que.push(node->right_child_);}
                delete node;
            }
            m_pRoot = nullptr;
        }
   
        //插入封装函数
        void insert(const ElementType& data)
        {
            this->m_pRoot = insert_impl(data,this->m_pRoot);
        }
        
        //删除封装函数
        void remove(const ElementType& data)
        {
            this->m_pRoot = remove_impl(data,this->m_pRoot);
        }


    private:
        //节点
        struct Node
        {
            Node(const ElementType& data = ElementType())
            : data_(data)
            , height_(1)//创建根节点时默认高度为1
            , left_child_(nullptr)
            , right_child_(nullptr)
            {}
            ElementType data_;
            size_t height_; //记录节点的高度值
            Node* left_child_;
            Node* right_child_;            
        };
        
        Node* m_pRoot;
        Compare m_compare;
        
        //返回节点的高度值
        int node_height(const Node* node) const//⭐返回值不应该是size_t!否则后面计算高度差时如果是负数就直接变成正的了
        {
            return (node == nullptr) ? 0 : node->height_;
        }

        //右旋转->以node的左子节点child为轴进行右旋转操作，并返回新的根节点
        Node* right_rotate(Node* node)
        {
            //右旋转
            Node* child = node->left_child_;
            node->left_child_ = child->right_child_;//直接转移就行（自己写的代码还多余地加了个判断其右孩子是否存在）
            child->right_child_ = node;
            //更新高度
            node->height_ = std::max(node_height(node->left_child_),node_height(node->right_child_)) + 1;
            child->height_ = std::max(node_height(child->left_child_),node_height(child->right_child_)) + 1;
            return child;
        }
   
        //左旋转->以node的右子节点为轴进行左旋转操作，并返回新的根节点
        Node* left_rotate(Node* node)
        {
            //左旋转
            Node* child = node->right_child_;
            node->right_child_ = child->left_child_;//直接转移就行（自己写的代码还多余地加了个判断其右孩子是否存在）
            child->left_child_ = node;
            //更新高度
            node->height_ = std::max(node_height(node->left_child_),node_height(node->right_child_)) + 1;
            child->height_ = std::max(node_height(child->left_child_),node_height(child->right_child_)) + 1;
            return child;
        }
    
        //左平衡->先左旋转子节点，再右旋转该节点
        Node* left_balance(Node* node)
        {
            node->left_child_ = left_rotate(node->left_child_);
            return right_rotate(node);
        }

        //右平衡操作->先右旋转子节点，再左旋转该节点
        Node* right_balance(Node* node)
        {
            node->right_child_ = right_rotate(node->right_child_);
            return left_rotate(node);
        }
    
        //标准递归插入私有函数
        Node* insert_impl(const ElementType& data, Node* node)
        {
            if (node == nullptr)//节点为空，找到合适位置插入
            {
                return new Node(data);
            }
            if (this->m_compare(node->data_, data))//data比当前节点data_大
            {
                node->right_child_ =  insert_impl(data, node->right_child_);
                //1.回溯时判断是否右失衡
                if (node_height(node->right_child_) - node_height(node->left_child_) > 1)//不用比较绝对值，因为右插不会导致-2情况发生
                {
                    //插到右子树的右子树了，需要左旋
                    if (node_height(node->right_child_->right_child_) >= node_height(node->right_child_->left_child_))
                    {
                        node = left_rotate(node);
                    }
                    else //插到右子树的左子树了，需要右平衡
                    {
                        node = right_balance(node);
                    }
                }
            }
            else if (this->m_compare(data, node->data_))//data比当前节点data_小
            {
                node->left_child_ =  insert_impl(data, node->left_child_);
                //2.回溯时判断是否左失衡
                if (node_height(node->left_child_) - node_height(node->right_child_) > 1)//不用比较绝对值，因为左插不会导致-2情况发生
                {
                    //插到左子树的左子树了，需要右旋
                    if (node_height(node->left_child_->left_child_) >= node_height(node->left_child_->right_child_))
                    {
                        node = right_rotate(node);
                    }
                    else //插到左子树的右子树了，需要左平衡
                    {
                        node = left_balance(node);
                    }
                }
            }
            //有相同元素，不往下递归，直接返回
            else
            {
                return node;//直接返回不会改变树结构，不用执行下面回溯更新节点高度代码
            }
            

            //3.回溯时判断检测更新节点高度
            node->height_ = std::max(node_height(node->left_child_),node_height(node->right_child_)) + 1;

            return node;
        }
    
        //递归删除私有函数
        Node* remove_impl(const ElementType& data, Node* node)
        {
            if (node == nullptr) {return node;}//找不到，返回

            if (node->data_ < data)//节点数据比要删除的数据小，待删除节点在右子树
            {
                node->right_child_ = remove_impl(data, node->right_child_);
                //右子树被删除可能导致左子树过高，需要旋转
                if (node_height(node->left_child_) - node_height(node->right_child_) > 1)
                {
                    //左子树的左子树太高，进行右旋
                     if (node_height(node->left_child_->left_child_) > node_height(node->left_child_->right_child_))
                    {
                        node = right_rotate(node);
                    }
                    else //左子树的右子树太高，需要左平衡
                    {
                        node = left_balance(node);
                    }
                }
            }
            else if (node->data_ > data) //待删节点在左子树
            {
                node->left_child_ = remove_impl(data, node->left_child_);
                //左子树被删除可能导致右子树过高，需要旋转
                if (node_height(node->right_child_) - node_height(node->left_child_) > 1)
                {
                    //右子树的右子树太高，进行左旋
                    if (node_height(node->right_child_->right_child_) > node_height(node->right_child_->left_child_))
                    {
                        node = left_rotate(node);
                    }
                    else //右子树的左子树太高，需要右平衡
                    {
                        node = right_balance(node);
                    }
                }
            }
            else //找到待删除节点
            {
                //先处理该节点两个子节点同时存在的情况
                if (node->left_child_ != nullptr && node->right_child_ != nullptr)
                {
                    //巧妙的处理方式：改为删除最高的子树，防止出现不平衡
                    if (node_height(node->left_child_) < node_height(node->right_child_))
                    //左子树比右子树低，删除后继
                    {
                        Node* post = node->right_child_;
                        while (post->left_child_ != nullptr)
                        {
                            post = post->left_child_;//最终post指向后继
                        }
                        node->data_ = post->data_;//用后继data_覆盖node的data_
                        /*⭐下面是自己写的错误代码：递归不应该破坏函数连续性!下面代码导致node的父节点指向混乱
                        node = post; //方便后面处理
                        post = remove_impl(node->data_ ,node); //删除节点
                        */
                        node->right_child_ = remove_impl(post->data_, node->right_child_);
                    }
                    else //右子树比左子树低，删除前驱
                    {
                        Node* pre = node->left_child_;
                        while (pre->right_child_ != nullptr)
                        {
                            pre = pre->right_child_;//最终pre指向前驱
                        }
                        
                        node->data_ = pre->data_;//用前驱数据覆盖node的数据
                        /*错误原因同上
                        node = pre;//方便后面处理
                        node = remove_impl(node->data_, node); //删除节点
                        */
                        node->left_child_ = remove_impl(pre->data_,node->left_child_);
                    }
                }
                else //统一成只有一个子节点或者没有子节点的情况
                {
                    
                    Node* child = node->left_child_;
                    if (child == nullptr) {child = node->right_child_;}
                    delete node;
                    node = nullptr;
                    return child;
                }

            }
            //更新节点高度
            node->height_ = std::max(node_height(node->left_child_), node_height(node->right_child_)) + 1;

            return node;//将节点返回给父节点
        }
    };
}