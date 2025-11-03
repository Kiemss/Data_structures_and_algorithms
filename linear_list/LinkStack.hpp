#include<iostream>


namespace LinkStackNameSpace
{
    template<typename ElementType>
    class LinkStack
    {
        private:
        //注意：这里的Node定义在类内，没有自己的template，是一个具体的类型，后面不需要模板参数！
        //也就是不需要Node<ElementType>
        struct Node
        {
            Node(ElementType val = ElementType()) 
            : value_(val)
            , next_(nullptr)
            {}
            ElementType value_;
            Node* next_;
        };

        int m_size;//添加该成员函数可以让size()函数的时间复杂度变为O(1)
        Node* m_pTop;//对于链式栈可以只使用栈顶指针而不额外添加哨兵节点

        public:
        //构造函数
        LinkStack();
        //析构函数
        ~LinkStack();
        //入栈
        void push(ElementType val);
        //出栈
        void pop();
        //判断栈是否为空
        bool empty() const;
        //获取栈大小
        int size() const;
        //获取栈顶元素
        ElementType top() const;
    };

    //构造函数
    template<typename ElementType>
    LinkStack<ElementType>::LinkStack() : m_size(0) , m_pTop(nullptr)
    {}

    //析构函数
    template<typename ElementType>
    LinkStack<ElementType>::~LinkStack()
    {
        Node* cur_ptr = this->m_pTop;
        while(m_pTop != nullptr)
        {
            m_pTop = m_pTop->next_;
            delete cur_ptr;
            cur_ptr = m_pTop;
        }
    }

    //入栈
    template<typename ElementType>
    void LinkStack<ElementType>::push(ElementType val)
    {
        Node* new_node = new Node(val);
        new_node->next_ = this->m_pTop;
        this->m_pTop = new_node;
        m_size++;
    }

    //出栈
    template<typename ElementType>
    void LinkStack<ElementType>::pop()
    {
        if(m_pTop == nullptr)
        {
            throw std::underflow_error("stack is empty!");
        }
        else
        {
            Node* cur_ptr = this->m_pTop;
            this->m_pTop = m_pTop->next_;
            delete cur_ptr;
            m_size--;
        }
    }

    //判断栈是否为空
    template<typename ElementType>
    bool LinkStack<ElementType>::empty() const
    {
        return (!this->m_pTop);
    }

    //获取栈大小
    template<typename ElementType>
    int LinkStack<ElementType>::size() const
    {
        return this->m_size;
    }

    //获取栈顶元素
    template<typename ElementType>
    ElementType LinkStack<ElementType>::top() const
    {
        if(!this->m_pTop) throw std::underflow_error("stack is empty!");
        return (this->m_pTop)->value_;
    }
}

