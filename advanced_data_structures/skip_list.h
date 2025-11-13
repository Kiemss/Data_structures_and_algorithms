#pragma once
#include<iostream>
#include<time.h>
#include<algorithm>
namespace data_structures::skip_list
{
    class SkipList
    {
    public:
        //构造函数
        SkipList()
        {
            this->m_pHead = new HeadNode(1);
            srand(static_cast<unsigned int>(time(nullptr)));//在构造函数中生死随机数种子而不是在每次函数调用都生成
        }

        //析构函数
        ~SkipList()
        {
            int level = this->m_pHead->level_;
            for (int i = 0; i < level; ++i)
            {
                Node *cur = this->m_pHead->next_;
                while (cur != nullptr)
                {
                    Node *temp = cur->next_;
                    delete cur;
                    cur = temp;
                }
                HeadNode *temp_head = this->m_pHead;
                this->m_pHead = static_cast<HeadNode*>(this->m_pHead->down_);
                delete temp_head;
            }
        }

        //搜索数据
        bool find(int data) const
        {
            Node *pre =this->m_pHead; //前驱指针，指向哨兵节点
            Node *cur = pre->next_; //cur指针，指向当前节点
            for (;;)
            {
                if (cur != nullptr) //当前节点不为空
                {
                    if (cur->data_ < data) //在当前节点右边，往后走
                    {
                        pre = cur;  //⭐方便遍历结束没找到时下次搜索不用从头来，重要!
                        cur = cur->next_;
                        continue;
                    }
                    else if (cur->data_ == data) //找到了！
                    {
                        return true;
                    }
                    //⭐cur->data_ < data这种情况整合到下面了（就是没找到）
                }

                //注意不要用else，否则会漏情况！
                //该层遍历完了没找到，需要向下走一层     
                if (pre->down_ == nullptr) //是最底层，跳出
                {
                    break; //不直接返回false是方便和其他跳出循环情况整合
                }

                //不是最底层，往下走
                pre = pre->down_;
                cur = pre->next_;
            }
            return false;
        }

        //添加数据
        void add(int data)
        {
            //先查找数据是否存在
            if (find(data)) {return;}

            //抛硬币，data数据添加几层
            int level = get_level();
            if (level > this->m_pHead->level_)//level过大时，跳表只增长一层
            {
                //添加最上层头节点
                level = this->m_pHead->level_ + 1;
                HeadNode *head_node = new HeadNode(level);
                head_node->down_ = this->m_pHead;
                this->m_pHead = head_node;
            }

            //先创建level层的data节点，修改down指针域
            Node **node_list = new Node*[level];
            for (int i = level - 1 ;i >= 0;--i)//注意i = level - 1而不是level
            {
                node_list[i] = new Node(data);
                if (i < level - 1)
                {
                    //上面节点的down指针域存储下面节点的地址
                    node_list[i]->down_ = node_list[i + 1];
                }
            }

            Node *head = this->m_pHead; //head表示与新节点层数相同的层的头节点

            //这里是从上往下找和新生成节点最高层数的头节点，后面就从这一层开始插入
            for (int i = this->m_pHead->level_; i > level; i--) 
            {
                head = head->down_;
            }

            //head就指向了要插入节点的链表了
            Node *pre = head;
            Node *cur = pre->next_;

            for (int i = 0; i < level; ++i)
            {
                while (cur != nullptr && cur->data_ < data)
                {
                    pre = cur;
                    cur = cur->next_;
                }

                //找到了。把新节点插入到pre的后面（cur可能为空）
                node_list[i]->next_ = cur;
                pre->next_ = node_list[i];

                pre = pre->down_;
                if (pre != nullptr) //如果pre->down_已经为空，就不要让cur也往下跑了
                {
                    cur = pre->next_;//否则会导致nullptr->next_
                }
            }
            delete[] node_list;
            node_list = nullptr;
        }

        //从上往下依次进行打印
        void show() const
        {
            Node *head = this->m_pHead;
            while (head != nullptr)
            {   
                Node *cur = head->next_;
                while (cur != nullptr)
                {
                    std::cout << cur->data_ << " ";
                    cur = cur->next_;
                }
                std::cout << std::endl;
                head = head->down_;
            }
            std::cout << "======================" << std::endl;
        }

        //删除数据
        void remove(int data)
        {
            Node *pre = this->m_pHead;
            Node *cur = this->m_pHead->next_;
            for (;;)
            {
                if (cur != nullptr)
                {
                    if (cur->data_ < data) //在右边，往后走
                    {
                        pre = cur;
                        cur = cur->next_;
                        continue;
                    }
                    else if (cur->data_ == data) //找到了
                    {
                        //删除cur指向的节点
                        pre->next_ = cur->next_; //一次走一步，避免了复杂循环
                        delete cur;
                    }
                }

                //cur == nullptr || cur->data > data
                if (pre->down_ == nullptr)
                {
                    break;
                }
                
                pre = pre->down_;//先下移，方便下面情况重置头指针指向
                //特殊情况：删除了最上层链表唯一的节点，就要删除这个空链表，防止出现多个空链表
                if (this->m_pHead->next_ == nullptr)
                {
                    delete this->m_pHead;
                    this->m_pHead = static_cast<HeadNode*>(pre);
                }

                cur = pre->next_;
                /*⭐让next从pre的下一层开始遍历而不是让pre追上next
                cur = cur->down_;
                然后while (pre->next_ != cur) pre = pre->next_...
                循环嵌套太复杂了
                */
            }
        }

    private:
        //跳跃表的基本节点
        struct Node
        {
            Node(int &data)
            : data_(data)
            , next_(nullptr)
            , down_(nullptr)
            {}
            Node(int &&data = int())
            : data_(data)
            , next_(nullptr)
            , down_(nullptr)
            {}
            int data_;  //数据域
            Node *next_;    //指向同一层最后一个节点的指针域
            Node* down_;    //指向下一层相同节点的指针域
        };

        //跳跃表的头节点类型
        struct HeadNode : public Node
        {
            HeadNode(int level)
            : level_(level)
            , Node(-1)
            {}
            int level_; //跳跃表的层数
        };

        HeadNode *m_pHead; //指向跳表最上面一层链表的头节点
    
        //获取节点层数的函数
        int get_level() const
        {
            int level = 1;
            while (rand() % 2 == 1)
            {
                ++level;
            }
            return level;
        }
    };
}