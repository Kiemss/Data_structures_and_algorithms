#include<iostream>
#include<memory>

//cpp的stl中的list本质上就是使用原始指针双向循环链表
//废案代码中尝试使用如下所言，最终决定遵循STL全部采用原始指针
//⭐对于双向循环链表，头节点应该使用原始指针管理防止链表为空时和头节点的next节点冲突
//如果一定要使用智能指针管理，那么应该让哨兵节点作为一个成员变量添加进类中
namespace DoublyCircularLinkListNameSpace
{
    template<typename ElementType>
    struct Node
    {
        ElementType value_;
        Node<ElementType>* pre_;
        Node<ElementType>* next_;
        Node(ElementType val = ElementType()) 
        : value_(val)
        ,pre_(nullptr)
        ,next_(nullptr)
        {}
    };

    template<typename ElementType>
    class DoublyCircularLinkList
    {
        private:
        
        Node<ElementType>* m_pHead;

        public:
        //构造函数
        DoublyCircularLinkList();
        //析构函数
        ~DoublyCircularLinkList();
        //遍历链表
        void show_list() const;
        //头插法
        void push_front(ElementType val);
        //尾插法
        void push_back(ElementType val);
        //搜索
        bool search(ElementType val) const;
        //删除指定的第一个元素
        bool delete_first(ElementType val);

    };

    //构造函数
    template<typename ElementType>
    DoublyCircularLinkList<ElementType>::DoublyCircularLinkList()
    {
        this->m_pHead = new Node<ElementType>();
        m_pHead->next_ = this->m_pHead;
        m_pHead->pre_ = this->m_pHead;
    }

    //析构函数
    template<typename ElementType>
    DoublyCircularLinkList<ElementType>::~DoublyCircularLinkList()
    {
        Node<ElementType>* cur_ptr = (this->m_pHead)->next_;
        while(cur_ptr != this->m_pHead)
        {
        (this->m_pHead)->next_ = cur_ptr->next_;
        (cur_ptr->next_)->pre_ = this->m_pHead;//自己写的代码是对m_pHead->next->pre_进行操作
        delete cur_ptr;
        cur_ptr = (this->m_pHead)->next_;
        }
        delete this->m_pHead;
        this->m_pHead = nullptr;
    }

    //遍历链表
    template<typename ElementType>
    void DoublyCircularLinkList<ElementType>::show_list() const
    {
        if(this->m_pHead->next_ == this->m_pHead)
        {
            std::cout << "链表为空！" << std::endl;
            return;
        }
        Node<ElementType>* cur_ptr = (this->m_pHead)->next_;
        while(cur_ptr != this->m_pHead)
        {
            std::cout << cur_ptr->value_ << " ";
            cur_ptr = cur_ptr->next_;
        }
        std::cout << std::endl;
    }

    //搜索元素
    template<typename ElementType>
    bool DoublyCircularLinkList<ElementType>::search(ElementType val) const
    {
        if(this->m_pHead->next_ == this->m_pHead)
        {
            std::cout << "链表为空！" << std::endl;
            return false;
        }
        Node<ElementType>* cur_ptr = (this->m_pHead)->next_;
        while(cur_ptr != this->m_pHead)
        {
            if(cur_ptr->value_ == val)
            {
                return true;
            }
            cur_ptr = cur_ptr->next_;
        }
        return false;
    }

    //头插法(和双向链表的一样)
    template<typename ElementType>
    void DoublyCircularLinkList<ElementType>::push_front(ElementType val)
    {
       Node<ElementType>* new_node = new Node<ElementType>(val);
        new_node->next_ = (this->m_pHead)->next_;
        new_node->pre_ = (this->m_pHead);
        (new_node->next_)->pre_ = (new_node);
        (this->m_pHead)->next_ = new_node;      
    }

    //尾插法
    template<typename ElementType>
    void DoublyCircularLinkList<ElementType>::push_back(ElementType val)
    {
        Node<ElementType>* new_node = new Node<ElementType>(val);
        new_node->pre_ = m_pHead->pre_;
        (m_pHead->pre_)->next_ = new_node;
        m_pHead->pre_ = new_node;
        new_node->next_ = m_pHead;
    }

    //删除第一个指定元素节点
    template<typename ElementType>
    bool DoublyCircularLinkList<ElementType>::delete_first(ElementType val)
    {
        if(this->m_pHead->next_ == this->m_pHead)
        {
            std::cout << "链表为空！" << std::endl;
            return false;
        }
        Node<ElementType>* cur_ptr = (this->m_pHead)->next_;
        while(cur_ptr != this->m_pHead)
        {
            if(cur_ptr->value_ == val)
            {
                (cur_ptr->next_)->pre_ = cur_ptr->pre_;
                (cur_ptr->pre_)->next_ = cur_ptr->next_;
                return true;
                delete cur_ptr;
            }
            cur_ptr = cur_ptr->next_;
        }
        return false;
    }
}