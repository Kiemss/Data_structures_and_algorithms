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
    //删除所有对应元素
    void remove_all(ElementType val);
    //搜索,stl中是返回迭代器，这里仅仅返回bool类型值 
    //线性搜索 时间复杂度O(n)
    bool search(ElementType val); 

    //链表逆序
    void reverse();
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
    //由于使用了智能指针，所以能自动释放内存（甚至析构函数都不用显示写出来！）
    /*
    while(m_pHead != nullptr)
    {
        // 模拟：Node* next = current->next;
        auto next = std::move(m_pHead->next_);
        // 模拟：delete current;
        m_pHead.reset();
        // 模拟：current = next;
        m_pHead = std::move(next);
         // 注意：value_ 不需要手动释放，因为它是值类型，会随Node一起自动销毁
    }
    */
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

//删除第一个对应元素
template<typename ElementType>
void SinglyLinkList<ElementType>::remove_once(ElementType val)
{
    //使用双指针法。检查下一个节点的方法更加高效（时间复杂度系数更小），但是不如双指针法方便理解。
    Node<ElementType>* fast_ptr = (this->m_pHead)->next_.get();
    Node<ElementType>* slow_ptr = (this->m_pHead).get();
    while(fast_ptr != nullptr)
    {
        if(fast_ptr->value_ == val)
        {
            
                slow_ptr->next_ = std::move(fast_ptr->next_);//无需手动释放内存
                return;
        }
        else//这个else是和删除所有元素的代码相对应
        {
            fast_ptr = (fast_ptr->next_).get();
            slow_ptr = (slow_ptr->next_).get();
        }
    }
}

//删除所有对应元素
template<typename ElementType>
void SinglyLinkList<ElementType>::remove_all(ElementType val)
{
    Node<ElementType>* fast_ptr = (this->m_pHead)->next_.get();
    Node<ElementType>* slow_ptr = (this->m_pHead).get();
    while(fast_ptr != nullptr)
    {
        if(fast_ptr->value_ == val)
        {
            
                slow_ptr->next_ = std::move(fast_ptr->next_);
                fast_ptr = (slow_ptr->next_).get();//将快指针置为慢指针的下一个
        }
        else
        {
            //删除节点后就不移动指针，否则会漏节点   
            fast_ptr = (fast_ptr->next_).get();
            slow_ptr = (slow_ptr->next_).get();
            //下面这个更新方式不如上面直观，但是更符合传统
            //slow_ptr = fast_ptr;
            //fast_ptr = fast_ptr->next_.get();
        }
    }
    delete slow_ptr;
    delete fast_ptr;
    slow_ptr = nullptr;
    fast_ptr = nullptr;
    return;
}

//搜索,stl中是返回迭代器，这里仅仅返回bool类型值
template<typename ElementType>
bool SinglyLinkList<ElementType>::search(ElementType val)
{
    //使用双指针法。检查下一个节点的方法更加高效（时间复杂度系数更小），但是不如双指针法方便理解。
    Node<ElementType>* current_ptr = (this->m_pHead)->next_.get();
    while(current_ptr != nullptr)
    {
        if(current_ptr->value_ == val)
        {
                return true;
        }
        current_ptr = (current_ptr->next_).get();
    }
    return false;
}

//头插法链表逆序
template<typename ElementType>
void SinglyLinkList<ElementType>::reverse()
{
    /*自己写的代码，有问题,使用了原始指针，和智能指针有点冲突，且复杂
    Node<ElementType>* temp_ptr;
    Node<ElementType>* current_ptr = (this->m_pHead)->next_.get();
    while(current_ptr != nullptr)
    {
        temp_ptr = (current_ptr->next_).get();
        //下面两句代码会导致current_ptr指向的节点同时被两个智能指针管理！
        current_ptr->next_ = std::move((this->m_pHead)->next_);
        (this->m_pHead)->next_.reset(current_ptr);
        current_ptr = temp_ptr;
    }
    return;
    */

    //全部使用智能指针，先将管理节点以及成员的指针move掉，防止冲突
    if((this->m_pHead)->next_ == nullptr)
    {
        return;
    }

    std::unique_ptr<Node<ElementType>> prev = nullptr;//
    std::unique_ptr<Node<ElementType>> current = std::move((this->m_pHead)->next_);

    while(current != nullptr)
    {
        //保存下一个节点
        std::unique_ptr<Node<ElementType>> next = std::move(current->next_);

        //反转指针
        current->next_ = std::move(prev);

        //移动指针
        prev = std::move(current);
        current = std::move(next);
    }
    m_pHead->next_ = std::move(prev);
}