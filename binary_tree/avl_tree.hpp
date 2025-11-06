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
            std::stack<Node*> st;
            st.push(this->m_pRoot);
            Node* node {};
            while (!st.empty())
            {
                node = st.top();
                st.pop();
                if (node->left_child_ != nullptr) {st.push(node->left_child_);}
                if (node->right_child_ != nullptr) {st.push(node->right_child_);}
                delete node;
            }
            m_pRoot = nullptr;
        }
   
        //插入封装函数
        void insert(const ElementType& data)
        {
            this->m_pRoot = insert_impl(data,this->m_pRoot);
        }
    private:
        //节点
        struct Node
        {
            Node(const ElementType& data = ElementType())
            : data_(data)
            , height(1)//创建根节点时默认高度为1
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
        size_t node_heigh(const Node*& node) const
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
            node->height_ = std::max(node_heigh(node->left_child_),node_heigh(node->right_child_)) + 1;
            child->height_ = std::max(node_heigh(child->left_child_),node_heigh(child->right_child_)) + 1;
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
            node->height_ = std::max(node_heigh(node->left_child_),node_heigh(node->right_child_)) + 1;
            child->height_ = std::max(node_heigh(child->left_child_),node_heigh(child->right_child_)) + 1;
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
    
        //标准迭代插入私有函数
        Node* insert_impl(const ElementType& data, Node* node)
        {
            if (node == nullptr)//节点为空，找到合适位置插入
            {
                return new Node(data);
            }
            if (this->m_compare(node->data_, data))//data比当前节点data_大
            {
                node->right_child_ =  insert(data, node->right_child_);
                //1.回溯时判断是否右失衡
                if (node_heigh(node->right_child_) - node_heigh(node->left_child_) > 1)//不用比较绝对值，因为右插不会导致-2情况发生
                {
                    //插到右子树的右子树了，需要左旋
                    if (node_heigh(node->right_child_->right_child_) >= node_heigh(node->right_child_->left_child_))
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
                node->left_child_ =  insert(data, node->left_child_);
                //2.回溯时判断是否左失衡
                if (node_heigh(node->left_child_) - node_heigh(node->right_child_) > 1)//不用比较绝对值，因为左插不会导致-2情况发生
                {
                    //插到左子树的左子树了，需要右旋
                    if (node_heigh(node->left_child_->left_child_) >= node_heigh(node->left_child_->right_child_))
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
            node->height_ = std::max(node_heigh(node->left_child_),node_heigh(node->right_child_)) + 1;

            return node;
        }
    };
}