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
            std::queue<ElementType> que;
            que.push(this->m_pRoot);
            Node *cur;
            while (!que.empty())
            {
                cur = que.front();
                que.pop();
                if (cur->left_child_ != nullptr) {que.push(cur->left_child_);}
                if (cur->right_child_ != nullptr) {que.push(cur->right_child_);}
                delete cur;
            }
            this->m_pRoot = nullptr;
            return;
        }
    


    private:
        enum class Color //枚举类，区分红黑
        {
            BLACK,
            RED,
        };

        struct Node //根节点
        {
            Node(const ElementType& data)
            : data_(data)
            , left_child_(nullptr)
            , right_child_(nullptr)
            , parent_(nullptr)
            , color_(Color::BLACK)//默认用黑色初始化因为根节点就是黑色的
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
            else {return node->left_child_};
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

        //左旋转->由于存在父亲节点，需要非常仔细(待更新颜色)
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
                    node->left_child_ = child;
                }
                else
                {
                    node->right_child_ = child;
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
    };
}