#pragma once
#include<iostream>
#include<string>
#include<memory>
#include"LinearList.hpp"

//使用了智能指针

//节点类型定义到外面，方便后续操作
template<typename ElementType>
struct Node
{
    //仅允许ElementType是系统内置类型和有默认构造函数的类
    ElementType value_;
    std::unique_ptr<Node<ElementType>> next_;
    Node( ElementType value = ElementType()) : value_(value) ,next_ (nullptr){}//调用了ElementType类型的默认构造函数
    //下面的是废案，强制要求提供初始化参数，这和链表头节点作为不存储数据的哨兵节点矛盾了
    //Node(ElementType value) : value_(value) , next_(nullptr){}
    //Node() = delete;//管提供了构造函数不会生成默认构造函数。这句代码的用处是明令禁止
};

template<typename ElementType>
class SinglyLinkList
{
    private:
    std::unique_ptr<Node<ElementType>> m_pHead;//指向链表的头节点
    public:
    //构造函数
    SinglyLinkList();
    //析构函数
    ~SinglyLinkList();
    //链表尾插法
    void push_back(ElementType val);
    //链表头插法
    void push_front(ElementType val);
    //遍历链表
    void show_list();
    //链表节点的删除,只删除第一个
    void remove_once(ElementType val);
};

//构造函数
template<typename ElementType>
SinglyLinkList<ElementType>::SinglyLinkList() 
{
    this->m_pHead = std::make_unique<Node<ElementType>>();//创建头节点
}

//析构函数
template<typename ElementType>
SinglyLinkList<ElementType>::~SinglyLinkList() 
{
    //释放节点
}

//尾插
template<typename ElementType>
void SinglyLinkList<ElementType>::push_back(ElementType val)
{
    //遍历链表找到链表尾节点
    Node<ElementType>* current_ptr = this->m_pHead.get();
    while(current_ptr->next_ != nullptr)
    {
        current_ptr = (current_ptr->next_).get();
    }

    //生成新节点, 把新节点地址放在尾节点地址域
    current_ptr->next_ = std::make_unique<Node<ElementType>>(val);
}

//链表头插
template<typename ElementType>
void SinglyLinkList<ElementType>::push_front(ElementType val)
{
    std::unique_ptr<Node<ElementType>> new_pNode = std::make_unique<Node<ElementType>>(val);
    new_pNode->next_ = std::move((this->m_pHead)->next_);
    (this->m_pHead)->next_ = std::move(new_pNode);
}

//遍历链表
template<typename ElementType>
void SinglyLinkList<ElementType>::show_list()
{
    Node<ElementType>* current_ptr = (this->m_pHead)->next_.get();
    while(current_ptr != nullptr)
    {
        std::cout << current_ptr->value_ << " ";
        current_ptr = (current_ptr->next_).get();
    }
    std::cout << endl;
    //下面的是废案，不够简洁
    //Node<ElementType>* current_ptr = (this->m_pHead).get();
    /*
    if(current_ptr->next_ != nullptr)
    {
        current_ptr = (current_ptr->next_).get();
    }
    while(true)
    {
        std::cout << current_ptr->value_ << " " ;
        if(current_ptr->next_ == nullptr)
        {
            break;
        }
        current_ptr= (current_ptr->next_).get();
    }
    */
}

template<typename ElementType>
void SinglyLinkList<ElementType>::remove_once(ElementType val)
{
    //使用双指针法。检查下一个节点的方法更加高效，但是不如双指针法直观。
    Node<ElementType>* fast_ptr = (this->m_pHead)->next_.get();
    Node<ElementType>* slow_ptr = (this->m_pHead).get();
    while(fast_ptr != nullptr)
    {
        if(fast_ptr->value_ == val)
        {
            
                slow_ptr->next_ = std::move(fast_ptr->next_);//无需手动释放内存
                return;
        }
        fast_ptr = (fast_ptr->next_).get();
        slow_ptr = (slow_ptr->next_).get();
    }
}