#include<iostream>
#include<memory>

namespace DoublyLinkListNameSpace
{
    template<typename ElementType>
    struct Node
    {
        std::unique_ptr<Node<ElementType>> next_ ;
        Node<ElementType>* pre_;
        ElementType value_;
        Node(ElementType value = ElementType()) 
        : value_(value) 
        , pre_(nullptr) 
        , next_( nullptr)
        {}
    };
    
    template<typename ElementType>
    class DoublyLinkList
    {
        private:
        std::unique_ptr<Node<ElementType>> m_pHead;
        public:
        //构造函数
        DoublyLinkList();
        //析构函数
        ~DoublyLinkList();
        //头插法
        void push_front(ElementType val);
        //尾插法
        void push_back(ElementType val);
        //遍历链表
        void show_list();
        //删除第一个指定元素节点
        bool delete_first(ElementType val);
        //搜索
        bool search(ElementType val);
    };

    //构造函数
    template<typename ElementType>
    DoublyLinkList<ElementType>::DoublyLinkList()
    {
        this->m_pHead = std::make_unique<Node<ElementType>>();
    }

    //析构函数
    template<typename ElementType>
    DoublyLinkList<ElementType>::~DoublyLinkList()
    {}

    //头插法
    template<typename ElementType>
    void DoublyLinkList<ElementType>::push_front(ElementType val)
    {
        std::unique_ptr<Node<ElementType>> new_node = std::make_unique<Node<ElementType>>(val);
        if((this->m_pHead)->next_ == nullptr)
        {
            new_node->pre_ = (this->m_pHead).get();
            (this->m_pHead)->next_ = std::move(new_node);
        }
        else
        {
        new_node->next_ = std::move((this->m_pHead)->next_);
        new_node->pre_ = (this->m_pHead).get();
        (new_node->next_)->pre_ = (new_node).get();
        (this->m_pHead)->next_ = std::move(new_node);
        }
    }
    
    //尾插法
    template<typename ElementType>
    void DoublyLinkList<ElementType>::push_back(ElementType val)
    {
        std::unique_ptr<Node<ElementType>> new_node = std::make_unique<Node<ElementType>>(val);
        if(!(this->m_pHead)->next_)//等同于this->m_pHead->next_ == nullptr
        {
            new_node->pre_ = (this->m_pHead).get();
            (this->m_pHead)->next_ = std::move(new_node);
        }
        else
        {
            Node<ElementType>* cur_ptr = (this->m_pHead)->next_.get();
            while(cur_ptr->next_)//等同于cur_ptr->next_ != nullptr
            {
                cur_ptr = (cur_ptr->next_).get();
            }
            new_node->pre_ = cur_ptr;
            cur_ptr->next_ = std::move(new_node);
        }
        return;
    }

    //遍历链表
    template<typename ElementType>
    void DoublyLinkList<ElementType>::show_list()
    {
        Node<ElementType>* cur_ptr = ((this->m_pHead)->next_).get();
        while(cur_ptr != nullptr)
        {
            std::cout << cur_ptr->value_ << " ";
            cur_ptr = (cur_ptr->next_).get();
        }
        std::cout << std::endl;
    }

    //查询指定元素节点
    template<typename ElementType>
    bool DoublyLinkList<ElementType>::search(ElementType val)
    {
        Node<ElementType>* cur_ptr = (this->m_pHead).get();
        while(cur_ptr->next_ != nullptr)
        {
            if((cur_ptr->next_)->value_ == val)
            {
                return true;
            }
            cur_ptr = cur_ptr->next_.get();
        }   
        return false;
    }

    //删除第一个指定元素节点
    template<typename ElementType>
    bool DoublyLinkList<ElementType>::delete_first(ElementType val)
    {
        Node<ElementType>* cur_ptr = (this->m_pHead)->next_.get();
        while(cur_ptr != nullptr)
        {
            if(cur_ptr->value_ == val)
            {
                (cur_ptr->next_)->pre_ = cur_ptr->pre_;//注意这句和下一句的先后顺序
                (cur_ptr->pre_)->next_ = std::move(cur_ptr->next_);
                //无需手动释放cur_ptr指向的节点，这会在该函数结束时自动释放
                return true;
            }
            cur_ptr = cur_ptr->next_.get();
        }   
        return false;
    }   
}