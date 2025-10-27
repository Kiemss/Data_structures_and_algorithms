#pragma once
#include<iostream>

namespace LinkQueueNameSpace
{
    template<typename ElementType>
    //这里使用带有哨兵节点的双向循环链表实现链队列
    class LinkQueue
    {
        private:
        struct Node
        {
            ElementType data_;
            Node* next_;
            Node* pre_;
            Node(ElementType data = ElementType())
            : data_(data)
            , next_(nullptr)
            , pre_(nullptr)
            {}
        };
      
        Node* m_pHead;//头指针

        public:
        //构造函数
        LinkQueue()
        {
           this->m_pHead = new Node();
           m_pHead->next_ = this->m_pHead;
           m_pHead->pre_ = this->m_pHead;
        }

        //析构函数
        ~LinkQueue()
        {
            Node* cur = (this->m_pHead)->next_;
            while(cur != this->m_pHead)
            {
                this->m_pHead->next_  = cur->next_;
                (cur->next_)->pre_ = this->m_pHead;
                delete cur;
                cur = this->m_pHead->next_;
            }
            delete this->m_pHead;
            this->m_pHead = nullptr;
        }

        //入队
        void push(ElementType val)
        {
            Node* new_node = new Node(val);
            new_node->next_ = this->m_pHead;
            m_pHead->pre_->next_ = new_node;
            new_node->pre_ = m_pHead->pre_;
            m_pHead->pre_ = new_node;
        }

        //出队
        void pop()
        {
            if(this->m_pHead->next_ == this->m_pHead)
            {
                throw std::underflow_error("queue is empty!");
            }
            Node* cur = (this->m_pHead)->next_;
            this->m_pHead->next_  = cur->next_;
            (cur->next_)->pre_ = this->m_pHead;
            delete cur;
        }

        //获取队头元素
        const ElementType& front() const
        {
            if(this->m_pHead->next_ == this->m_pHead)
            {
                throw std::underflow_error("queue is empty!");
            }
            return m_pHead->next_->data_;
        }

        //获取队尾元素
        const ElementType& rear() const
        {
            if(this->m_pHead->next_ == this->m_pHead)
            {
                throw std::underflow_error("queue is empty!");
            }
            return m_pHead->pre_->data_;
        }

        //判断队列是否为空
        bool empty() const
        {
            return (this->m_pHead->next_ == this->m_pHead);
        }

        //获取队列元素数量
        size_t size() const
        {
            size_t num {};
            Node* cur = this->m_pHead->next_;
            while(cur != this->m_pHead)
            {
                cur = cur->next_;
                ++num;
            }
            return num;
        }
    };
}