#pragma once
#include<iostream>

namespace SinglyCircularLinkListNameSpace
{
template<typename ElementType>
struct Node
{
    Node<ElementType>* next_ {};
    ElementType value_ {};
    Node(ElementType value = ElementType()) : value_(value) , next_(nullptr)
    {}
};


template<typename ElementType>
class SinglyCircularLinkList
{
    private:
        Node<ElementType>* m_pHead;
        //相比于单链表多了一个尾指针，因此要常常考虑关于尾指针位置是否要改变的特殊情况
        Node<ElementType>* m_pTail;
    public:
    //构造函数
    SinglyCircularLinkList();
    //析构函数
    ~SinglyCircularLinkList();

    //尾插法
    void push_back(ElementType val);
    //头插法
    void push_front(ElementType val);
    //删除第一个值为val的节点
    bool delete_first(ElementType val);
    //链表查询
    bool search(ElementType val) const;
    //遍历链表
    void show_list() const;

    //约瑟夫问题
    void joseph(size_t num);
};

//构造函数
template<typename ElementType>
SinglyCircularLinkList<ElementType>::SinglyCircularLinkList() 
{
    Node<ElementType>* new_node = new Node<ElementType>();
    this->m_pHead = new_node;
    this->m_pTail = this->m_pHead;
    this->m_pHead->next_ = this->m_pHead;
}

//析构函数
template<typename ElementType>
SinglyCircularLinkList<ElementType>::~SinglyCircularLinkList()
{
    Node<ElementType>* cur_ptr = (this->m_pHead)->next_;
    while(cur_ptr != this->m_pHead)
    {
        (this->m_pHead)->next_  = cur_ptr->next_;
        delete cur_ptr;
        cur_ptr = (this->m_pHead)->next_;
    }
    delete this->m_pHead;
}

//尾插法
template<typename ElementType>
void SinglyCircularLinkList<ElementType>::push_back(ElementType val)
{
    Node<ElementType>* new_node = new Node<ElementType>(val);
    (this->m_pTail)->next_ = new_node;
    new_node->next_ = this->m_pHead;
    this->m_pTail = new_node;//注意更新尾指针指向新的尾节点
}

//头插法
template<typename ElementType>
void SinglyCircularLinkList<ElementType>::push_front(ElementType val)
{
    Node<ElementType>* new_node = new Node<ElementType>(val);
    new_node->next_ = (this->m_pHead)->next_;
    (this->m_pHead)->next_ = new_node;
    //注意边界条件：链表为空时要额外移动尾节点至新节点
    if((this->m_pTail) == this->m_pHead)
    {
        this->m_pTail = (this->m_pTail)->next_;
    }
}

//删除第一个val
template<typename ElementType>
bool SinglyCircularLinkList<ElementType>::delete_first(ElementType val)
{
    if(this->m_pHead == this->m_pTail)
    {
        return false;
    }
    Node<ElementType>* fast_ptr = (this->m_pHead)->next_;
    Node<ElementType>* slow_ptr = this->m_pHead;
    while(fast_ptr != this->m_pHead)
    {
        if(fast_ptr->value_ == val)
        {
            //记得考虑尾节点！
            if(fast_ptr == this->m_pTail)
            {
                m_pTail = slow_ptr;
            }
            slow_ptr->next_ = fast_ptr->next_;
            delete fast_ptr;
            return true;
        }
        fast_ptr = fast_ptr->next_;
        slow_ptr = slow_ptr->next_;
    }
    return false;
}

//链表查询
template<typename ElementType>
bool SinglyCircularLinkList<ElementType>::search(ElementType val) const
{
    Node<ElementType>* current_ptr = (this->m_pHead)->next_;
    while(current_ptr != this->m_pHead)
    {
        if(current_ptr->value_ == val)
        {
                return true;
        }
        current_ptr = (current_ptr->next_);
    }
    return false;
}

//遍历链表
template<typename ElementType>
void SinglyCircularLinkList<ElementType>::show_list() const
{
    if(this->m_pHead->next_ == this->m_pHead)
    {
        std::cout << "该链表为空！"  << std::endl;
        return;
    }
    Node<ElementType>* current_ptr = (this->m_pHead)->next_;
    while(current_ptr != this->m_pHead)
    {
        std::cout << current_ptr->value_ << " ";
        current_ptr = (current_ptr->next_);
    }
    std::cout << std::endl;
}

//约瑟夫问题
//⭐尾节点和跳过头节点数数是重难点!!!
template<typename ElementType>
void SinglyCircularLinkList<ElementType>::joseph(size_t num)
{
    if(this->m_pHead == this->m_pTail)
    {
        std::cout << "链表为空！" << std::endl;
        return;
    }
    //新建一个循环单向链表（直接在本链表做回破环链表结构）
    SinglyCircularLinkList<ElementType> tempScll{};
    Node<ElementType>* slow_ptr = (this->m_pHead)->next_;
    do
    {
        tempScll.push_back(slow_ptr->value_);
        slow_ptr = slow_ptr->next_;
    } while (slow_ptr != m_pHead);

    
    //解决约瑟夫问题
    slow_ptr = tempScll.m_pTail;
    //tempScll.m_pTail->next_ = slow_ptr->next_;
    Node<ElementType>* fast_ptr = tempScll.m_pHead->next_;
    //slow_ptr = tempScll.m_pTail;
    //delete tempScll.m_pHead;

    while(fast_ptr != slow_ptr)
    {
        for(int i = 0;i < num - 1;i++)
        {
            if(fast_ptr == tempScll.m_pHead)
            {
                fast_ptr = fast_ptr->next_;
            }
            slow_ptr = fast_ptr;
            fast_ptr = fast_ptr->next_;
        }
        if(fast_ptr == tempScll.m_pHead)
        {
            slow_ptr = fast_ptr;
            fast_ptr = fast_ptr->next_;
        }
        
        slow_ptr->next_ = fast_ptr->next_;
        std::cout << fast_ptr->value_ << " ";
        if(fast_ptr == tempScll.m_pTail)
        {
            m_pTail = slow_ptr;
        }
        delete fast_ptr;
        fast_ptr = slow_ptr->next_;
    }
}
}