#pragma once
#include<iostream>
#include<functional>
#include<queue>
#include<limits>
#include<stack>//用于迭代遍历树
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
        
        //自己写的递归插入
        void my_recursive_insert_impl(const ElementType& val,Node* ptr)
        {
            if (val == ptr->data_) return;//防止树中存在重复元素
            else if (this->m_compare(ptr->data_, val))//比data_大
            {
                if (ptr->right_child_ != nullptr)//不为空继续向右递归
                {
                    my_recursive_insert_impl(val,ptr->right_child_);
                }
                else //为空，可以插入
                {
                    ptr->right_child_ = new Node(val);
                }
            }
            else if (this->m_compare(val, ptr->data_))//比data_小
            {
                if (ptr->left_child_ != nullptr)//不为空继续向左递归
                {
                    my_recursive_insert_impl(val,ptr->left_child_);
                }
                else //为空，可以插入
                {
                    ptr->left_child_ = new Node(val);
                }
            }
        }

        /*非常巧妙！返回值为节点，既能一次性处理根节点为空的情况
        既有"递"又有"归"，符合函数式编程
        便于扩展成删除操作*/
        //标准递归插入实现
        Node* recursive_insert_impl(const ElementType& val,Node* ptr)
        {
            if (ptr == nullptr) //是空白位置，增加节点后返回给上一级节点
            {
                ++this->m_size;
                return (new Node(val));
            }
            if (ptr->data_ == val) return ptr;//有重复元素，直接返回
            else if (this->m_compare(val ,ptr->data_))//插入数据比当前数据小，左插入
            {
                ptr->left_child_ = recursive_insert_impl(val,ptr->left_child_);
                return ptr;
            }
            else if (this->m_compare(ptr->data_, val))//右插入
            {
                ptr->right_child_ = recursive_insert_impl(val, ptr->right_child_);
                return ptr;
            }
        }
        
        //递归删除实现
        Node* recursive_remove_impl(const ElementType& val,Node* ptr)
        {
            if (ptr == nullptr)//ptr->data_ == nullptr是错误的，因为data_是Elementtype类型
            {
                return ptr;//相当于返回nullptr
            }
            if (ptr->data_ == val)
            {
                if (ptr->right_child_ != nullptr && ptr->left_child_ != nullptr)
                {
                    Node* pre = ptr->left_child_;
                    //Node* parent = ptr;//自己写的代码直接跳到前驱节点的上一个节点开始进行删除前驱节点
                    while(pre->right_child_ != nullptr)
                    {
                       //parent = pre;
                        pre = pre->right_child_;
                    }
                    ptr->data_ = pre->data_;
                    //parent = recursive_remove_impl(pre->data_,pre);
                    ptr->left_child_ = recursive_remove_impl(pre->data_,ptr->left_child_);//教程的写法也是直接删除前驱节点，是从ptr开始递归
                }    
                else 
                {
                    Node* child =  (ptr->left_child_ == nullptr) ? ptr->right_child_ : ptr->left_child_;
                    delete ptr;
                    --m_size;
                    return child;
                }
            }
            if (this->m_compare(val, ptr->data_))//val比data小
            {
                ptr->left_child_ =  recursive_remove_impl(val,ptr->left_child_);
                return ptr;
            }
            if (this->m_compare(ptr->data_, val))
            {
                ptr->right_child_ = recursive_remove_impl(val,ptr->right_child_);
                return ptr;
            }
        }
       
        //找区间内元素->用中序遍历，因为中序遍历是从小到大的
        void find_values_impl(Node* node,std::vector<ElementType>& vec,ElementType min,ElementType max)
        {
            if (node != nullptr)
            {
                //L
                if (node->data_ > min)
                {
                    find_values_impl(node->left_child_,vec,min,max);
                }
                //V
                if (node->data_ > min && node->data_ < max)
                {
                    vec.push_back(node->data_);
                }
                //R
                if (node->data_ < max)
                {
                    find_values_impl(node->right_child_,vec,min,max);
                }
            }

        }
        
        //判断二叉树是否是BST的递归函数
        bool is_BST_impl(Node* node,Node* &pre)
        //这里的pre是个指针引用，因此每次"输出"都会将其更新为被输出的数，可以理解为入栈的栈顶
        {
            if (node == nullptr) {return true;}

            if (!is_BST_impl(node->left_child_,pre)) {return false;}//L
            
            if (pre != nullptr)
            {
                if (this->m_compare(node->data_, pre->data_)) //违背升序，递归结束，返回false
                {
                    return false;
                }
            }
            pre = node; //更新中序遍历的前驱节点

            return is_BST_impl(node->right_child_,pre);//R
        }
        
        //判断二叉树是否是该二叉树的子树
        bool is_child_tree_impl(Node* root,Node* child)
        {
            if (root == nullptr && child == nullptr) {return true;}
            if (root == nullptr || child == nullptr) {return false;} 
            if (root->data_ != child->data_) {return false;}
            return is_child_tree_impl(root->left_child_,child->left_child_) 
                && is_child_tree_impl(root->right_child_,child->right_child_);
        }
        
        //获取公共祖先节点
        Node* get_LCA_impl(const ElementType& val1,const ElementType& val2,Node* node) const
        {
            if (node == nullptr) {return nullptr;}//没找到，返回空指针
            if (this->m_compare(node->data_, val1) && this->m_compare(node->data_, val2))//node的数值比两个val小
            {
                return get_LCA_impl(val1,val2,node->right_child_);
            }
            else if (this->m_compare(val1, node->data_) && this->m_compare(val2, node->data_))//node的数值比两个val大
            {
                return get_LCA_impl(val1,val2,node->left_child_);
            }
            else //node的数值在两个val中间
            {
                return node;
            }
        }
        
        public:
        //构造函数
        BinarySearchTree() : m_pRoot(nullptr) , m_size(0){}

        //析构函数

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
    
        //自己写的递归插入封装函数（不标准）
        void my_recursive_insert(const ElementType& val)
        {
            if (this->m_pRoot == nullptr) 
            {
                m_pRoot = new Node(val);
                return;
            }
            my_recursive_insert_impl(val,this->m_pRoot);
            return;
        }

        //递归插入封装函数
        void recursive_insert(const ElementType& val)
        {
            this->m_pRoot = recursive_insert_impl(val,this->m_pRoot);
        }
        
        //递归删除封装函数
        void recursive_remove(const ElementType& val)
        {
            this->m_pRoot = recursive_remove_impl(val,this->m_pRoot);
        }
    
        //迭代前序遍历函数
        void iterative_preorder_traversal(std::vector<ElementType>& vec) const
        {
            if (this->empty()) return;//为空直接返回（节省性能）
            std::stack<Node*> st;//用栈来实现“回溯”遍历
            st.push(this->m_pRoot);
            Node* node{};
            while(!st.empty())
            {
                node = st.top();//栈顶节点为当前要处理的节点
                st.pop();//在存放其子节点前pop
                vec.push_back(node->data_);
                if (node->right_child_ != nullptr) 
                {
                    st.push(node->right_child_);
                }
                if (node->left_child_ != nullptr) 
                {
                    st.push(node->left_child_);
                }
            }
        }
    
        //迭代中序遍历函数
        void iterative_inorder_traversal(std::vector<ElementType>& vec) const
        {
            if (this->empty()) return;//树空：直接返回
            std::stack<Node*> st;
            Node* cur = this->m_pRoot;
            #if 1//最终优化版本1
            while (!st.empty() || cur != nullptr)
            {
                //先将根节点左子树左边节点全部压入栈并将cur定位到最左边的节点
                while (cur != nullptr)
                {
                    st.push(cur);
                    cur = cur->left_child_;
                }
                //然后处理当前节点，再处理右节点
                Node* top = st.top();
                st.pop();
                vec.push_back(top->data_);
                cur = top->right_child_;
            }
            #endif
        
            #if 0//另一个优化版本，每次循环选择左走一步或处理本节点和右节点
            while (!st.empty() || cur != nullptr)
            {
                if (cur != nullptr)
                {
                    st.push(cur);
                    cur = cur->left_child_;
                }
                else
                {
                Node* top = st.top();
                st.pop();
                vec.push_back(top->data_);
                cur = top->right_child_;
                }
            }
            #endif

            #if 0//初始版本，有点像do-while结构
            while (cur != nullptr)
            {
                st.push(cur);
                cur = cur->left_child_;
            }

            while (!st.empty())
            {
                Node* top = st.top();
                st.pop();
                vec.push_back(top->data_);
                cur = top->right_child_;
                while (cur != nullptr)
                {
                    st.push(cur);
                    cur = cur->left_child_;
                }
            }
            #endif
        }

        //迭代后序遍历函数
        void iterative_postorder_traversal(std::vector<ElementType>& vec)
        {
            if (this->empty()) return;//树空：直接返回
            std::stack<Node*> st1;
            std::stack<Node*> st2;
            st1.push(this->m_pRoot);
            Node* top;
            
            while (!st1.empty())
            {
                //先将栈1中栈顶元素弹入栈2
                top = st1.top();
                st1.pop();
                st2.push(top);

                //如果该元素有左/右子树，将其入栈1
                if (top->left_child_ != nullptr)
                {
                    st1.push(top->left_child_);
                }
                if (top->right_child_ != nullptr)
                {
                    st1.push(top->right_child_);
                }
            }

            //将栈st2中元素导入vec中
            while (!st2.empty())
            {
                top = st2.top();
                st2.pop();
                vec.push_back(top->data_);
            }
            /*
            使用st2：对于单个节点能实现L R V 排序，然后对于每一个节点又是L R V排序，有点递归味道
            */
        }
        
        //单栈实现迭代后序遍历函数
        void iterative_postorder_traversal_single_stack(std::vector<ElementType>& vec)
        {
            if (this->empty())
            {
                return;
            }
            
            std::stack<Node*> st;
            Node* current = this->m_pRoot;
            Node* last_visited = nullptr;  // 记录上次访问的节点，非常巧妙的引入！
            
            while (current != nullptr || !st.empty())
            {
                // 1. 遍历到最左节点，路径上的节点全部入栈
                if (current != nullptr)
                {
                    st.push(current);
                    current = current->left_child_;
                }
                else
                {
                    // 2. 查看栈顶节点
                    Node* top_node = st.top();
                    
                    // 3. 如果右子树存在且未被访问过，转向右子树
                    if (top_node->right_child_ != nullptr && top_node->right_child_ != last_visited)
                    {
                        current = top_node->right_child_;
                    }
                    else
                    {
                        // 4. 否则访问当前节点
                        st.pop();
                        vec.push_back(top_node->data_);
                        last_visited = top_node;  // 记录刚刚访问的节点
                    }
                }
            }
        }
    
        //迭代层序遍历函数
        void iterative_leverlorder_traversal(std::vector<ElementType>& vec)
        {
            if (this->empty()) return;

            //自己写的糖丸了😡明明可以用一个队列就解决的,偏偏用了两数组
            std::queue<Node*> que;
            Node* node;
            que.push(this->m_pRoot);
            while(!que.empty())
            {
                node = que.front();
                que.pop();
                vec.push_back(node->data_);
                if (node->left_child_ != nullptr)
                {
                    que.push(node->left_child_);
                }
                if (node->right_child_ != nullptr)
                {
                    que.push(node->right_child_);
                }
            }
            
        }

        //找区间内元素封装函数->利用中序遍历升序特点
        void find_values(std::vector<ElementType>& vec,ElementType min,ElementType max)
        {
            find_values_impl(this->m_pRoot,vec,min,max);
        }
   
        //判断二叉树是否是BST->利用中序遍历升序特点⭐自己写时使用了栈，实际上使用上一个节点的引用即可
        void is_BST()
        {
            Node* pre =nullptr;
            return is_BST_impl(this->m_pRoot, pre);
        }

        //获取根节点
        const Node* get_root() const
        {
            return this->m_pRoot;
        }

        //判断某个二叉树是不是该二叉树的子树
        bool is_child_tree(BinarySearchTree<ElementType, Compare> &child)
        {
            //如果该树为空，则该数是任何数的子树
            if (child.empty()) {return true;}
            Node* node = this->m_pRoot;
            ElementType child_data = child.get_root()->data_;
            while (node != nullptr)
            {
                //找到子树的根节点
                if (node->data_ == child_data)
                {
                    return is_child_tree_impl(node,child.get_root());
                }
                else if (this->m_compare(node->data_, child_data))//子树根在节点右边
                {
                    node = node->right_child_;
                }
                else //子树根在节点左边
                {
                    node = node->left_child_;
                }
            }
            //循环结束了还找不到子树根
            return false;
        }
    
        //LCA问题：求最近公共祖先节点
        ElementType get_LCA(const ElementType& val1,const ElementType& val2) const
        {
            Node* node = get_LCA_impl(val1,val2,this->m_pRoot);
            if ( node == nullptr) {throw "no LCA!";}//如果没有公共祖先节点，抛异常
            else {return node->data_;}
        }
    };
}