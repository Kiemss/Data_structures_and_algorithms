#pragma once
#include<iostream>
#include<functional>
//#include<memory>沟槽的智能指针，太费时间了
namespace data_structures::binary_search_tree
{
    template<typename ElementType, typename Compare = std::less<ElementType>>
    class BinarySearchTree
    {
    private:
        //节点
        struct Node
        {
            Node(ElementType data = ElementType())
            : data_ (data)
            , left_child_(nullptr)
            , right_child_(nullptr)
            {}
            ElementType data_;
            Node* left_child_;
            Node* right_child_;
        };
        
        //成员变量
        Node* m_pRoot;//指向树的根节点的指针
        size_t m_size;//当前节点数
        Compare m_compare;//需要定义函数对象才能进行调用

        //前序遍历
        /*⭐自己写的，总结：如果需要用成员变量给成员函数赋值，说明该用另一个函数封装该函数了
        void preorder_traversal(std::vector<ElementType>& vec,Node* ptr = nullptr)
        {
            if (ptr == nullptr) ptr = this->m_pRoot;
            vec.push_back(ptr->data_);
            if (ptr->left_child_ != nullptr)
            {
                preorder_traversal(vec,ptr->left_child_);
            }

            if (ptr->right_child_ != nullptr)
            {
                preorder_traversal(vec,ptr->right_child_);
            }
        }
        */
        void preorder_traversal(Node* ptr,std::vector<ElementType>& vec)
        {
            if (ptr != nullptr)
            {
                vec.push_back(ptr->data_);
                preorder_traversal(ptr->left_child_,vec);
                preorder_traversal(ptr->right_child_,vec);
            }
        }
        //中序遍历
        void inorder_traversal(Node* ptr,std::vector<ElementType>& vec)
        {
            if (ptr != nullptr)
            {
                inorder_traversal(ptr->left_child_,vec);
                vec.push_back(ptr->data_);
                inorder_traversal(ptr->right_child_,vec);
            }
        }
        //后序遍历
        void postorder_traversal(Node* ptr,std::vector<ElementType>& vec)
        {
            if (ptr != nullptr)
            {
                postorder_traversal(ptr->left_child_,vec);
                postorder_traversal(ptr->right_child_,vec);
                vec.push_back(ptr->data_);
            }
        }
        //层序遍历
        void levelorder_traversal_impl(Node* ptr,std::vector<ElementType>& vec,size_t h)
        {
            if (ptr == nullptr) return;
            if (h == 0) vec.push_back(ptr->data_);
            else
            {
                levelorder_traversal_impl(ptr->left_child_,vec,h - 1);//不要使用--h，会导致下面代码h偏小（当时我怎么想的，糖丸了）
                levelorder_traversal_impl(ptr->right_child_,vec,h - 1);
            }
        }
        //迭代获取二叉树层数
        size_t high(Node* ptr) const
        {
            if (ptr == nullptr) return 0;
            size_t left = high(ptr->left_child_);
            size_t right = high(ptr->right_child_);
            return left > right ? (left + 1) : (right + 1);
        }
        //迭代获取二叉树节点数
        size_t num(Node* ptr) const
        {
            if (ptr == nullptr) return 0;
            size_t left = num(ptr->left_child_);
            size_t right = num(ptr->right_child_);
            return (left + right + 1);
        }
        public:
        //构造函数
        BinarySearchTree() : m_pRoot(nullptr) , m_size(0){}

        //析构函数->使用了智能指针管理，不用手动释放内存
        
        //获取节点数
        size_t size() const
        {
            return this->m_size;
        }

        //判断树是否为空
        bool empty() const
        {
            return !this->m_size;
        }

        //插入
        void iterative_insert(const ElementType& data)
        {
            //树为空生成根节点
            if (this->empty())
            {
                this->m_pRoot = new Node(data);
                ++this->m_size;
                return;
            }

            //树不为空，搜索合适的插入位置，注意记录父节点位置
            Node* cur = this->m_pRoot;
            Node* parent = cur;
            while (cur != nullptr)
            {
                if (data == cur->data_)//确保树中无相同元素
                {
                    return;
                }
                else if (this->m_compare(cur->data_, data))//使用Compare，以便于自定义函数对象
                {
                    parent = cur;
                    cur = cur->right_child_;
                }
                else if (this->m_compare(data , cur->data_))
                {
                    parent = cur;
                    cur = cur->left_child_;
                }

            }
            
            //把新节点插到parent节点的孩子上
            if (this->m_compare(data, parent->data_))
            {
                parent->left_child_ = new Node(data);
            }
            else
            {
                parent->right_child_ = new Node(data);
            }

            //处理成员变量
            ++this->m_size;
            return;
        }

        //删除,注意链表结构的删除不同于数组结构，是可以用断开重连的方式保持结构完整的
        bool iterative_remove(const ElementType& data)
        {
            if (this->empty()) throw std::underflow_error("BST is empty!");
            Node* cur = this->m_pRoot;
            Node* parent = nullptr;    
            while (cur != nullptr)
            {
                if (this->m_compare(data,cur->data_))
                {
                    parent = cur;
                    cur = cur->left_child_;
                }
                else if (this->m_compare(cur->data_, data))//对于push中使用!的问题，这里使用调换参数位置解决
                {
                    parent = cur;
                    cur = cur->right_child_;
                }
                else//匹配成功
                {
                    //先判断是否是左右子树均存在的情况，如果是，可以用找前驱或者后继的方式处理成左右子树不同时存在的情况,巧妙！
                    if (cur->left_child_ != nullptr && cur->right_child_ != nullptr)
                    {
                        parent = cur;//由于转换了问题，这里使用parent作为pre的父节点，最后也会统一成cur的父节点，巧妙！
                        Node* pre = cur->left_child_;
                        //这里是找前驱（就是左子树中最大的值），也可以找后继（右子树中最小的值）
                        while (pre->right_child_ != nullptr)//用pre->right_child_作为判断条件而不是pre，巧妙！
                        {
                            parent = pre;
                            pre = pre->right_child_;
                        }
                        cur->data_ = pre->data_;
                        cur = pre;//方便后面统一处理
                    }
                
                    //下面统一处理左右子树不都存在的情况
                    Node* child = cur->left_child_;//很巧妙地找到子节点存在还是右节点存在，避免了冗长的控制语句
                    if (child == nullptr)
                    {
                        child = cur->right_child_;
                    }


                    if (parent == nullptr)//删除的是根节点
                    {
                        this->m_pRoot = child;
                    }
                    else
                    {
                        if (parent->left_child_ == cur)
                        {
                            parent->left_child_ = child;
                        }
                        else
                        {
                            parent->right_child_ = child;
                        }
                    }
                    delete cur;//删除当前节点
                    cur = nullptr;
                    --this->m_size;
                    return true;
                }
            }
            return false;//未找到，删除失败
        }
        
        //查询，最简单的一集
        bool iterative_find(const ElementType& data) const
        {
            Node* cur = this->m_pRoot;
            while (cur != nullptr)
            {
                if (this->m_compare(data, cur->data_))
                {
                    cur = cur->left_child_;
                }
                else if (this->m_compare(cur->data_, data))
                {
                    cur = cur->right_child_;
                }
                else
                {
                    return true;
                }
            }
            return false;
        }
        //获取根节点的数据
        ElementType& top() const
        {
            if (this->empty()) throw std::underflow_error("BSL is empty!");
            return m_pRoot->data_;
        }
    
        enum class Traversal
        {
            PREORDER,
            INORDER,
            POSTRDER,
        };
        //递归遍历封装函数
        void traversal(std::vector<ElementType>& vec,Traversal method)
        {
            switch(method)
            {
                case Traversal::PREORDER:
                preorder_traversal(this->m_pRoot,vec);
                break;
                case Traversal::INORDER:
                inorder_traversal(this->m_pRoot,vec);
                break;
                case Traversal::POSTRDER:
                postorder_traversal(this->m_pRoot,vec);
            }
        }

        //递归层序遍历封装函数
        void levelorder_traversal(std::vector<ElementType>& vec)
        {
            for (int i = 0;i < this->high(this->m_pRoot);++i)
            {
                levelorder_traversal_impl(this->m_pRoot,vec,i);
            }
        }
    };
}