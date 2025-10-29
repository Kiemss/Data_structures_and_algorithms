#include<iostream>
#include<memory>

namespace SeqStackNameSpace
{
    template<typename ElementType>
    class SeqStack
    {
        private:
        ElementType* m_pStack;
        int m_top;
        int m_cap;
        static constexpr double EXPANSION_FACTOR = 2;
        private:
        //扩容
        void expand(int size );

        public:
        //栈没有遍历（只允许对栈顶元素进行访问和操作）和获取容量（栈底层是可扩容的）
        //构造函数
        SeqStack(size_t cap = 10);
        //析构函数
        ~SeqStack();
        // 拷贝构造和赋值运算符,这里注重于数据结构，因此禁用了，
        //若不禁用则需要实现以解决浅拷贝问题
        SeqStack(const SeqStack&) = delete;
        SeqStack& operator=(const SeqStack&) = delete;
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

    //扩容
    template<typename ElementType>
    void SeqStack<ElementType>::expand(int size)
    {
        m_cap = size * EXPANSION_FACTOR;
        ElementType* new_seq = new ElementType[this->m_cap];
        memcpy(new_seq,this->m_pStack,m_top*sizeof(ElementType));
        delete[] this->m_pStack;
        this->m_pStack = new_seq;
        
    }

    //构造函数
    template<typename ElementType>
    SeqStack<ElementType>::SeqStack(size_t cap )
    {
        this->m_cap = cap;
        this->m_top = 0;
        this->m_pStack = new ElementType[cap];
    }

    //析构函数
    template<typename ElementType>
    SeqStack<ElementType>::~SeqStack()
    {
        delete[] this->m_pStack;
        m_pStack = nullptr;
    }

    //插入
    template<typename ElementType>
    void SeqStack<ElementType>::push(ElementType val)
    {
        if(this->m_top == this->m_cap)
        {
            this->expand(this->m_cap * EXPANSION_FACTOR);
        }
        this->m_pStack[this->m_top++] = val;
    }

    //删除
    template<typename ElementType>
    void SeqStack<ElementType>::pop()
    {
        if(this->m_top == 0)
        {
            throw std::underflow_error("栈为空，无法删除！");
        }
        else
        {
            m_top--;
        }
    }

    //判断栈是否为空
    template<typename ElementType>
    bool SeqStack<ElementType>::empty() const
    {
       if(this->m_top == 0)
       {
            return true;
       } 
       else
       {
            return false;
       }
    }

    //获取栈大小
    template<typename ElementType>
    int SeqStack<ElementType>::size() const
    {
        return this->m_top;
    }

    //获取栈顶元素
    template<typename ElementType>
    ElementType SeqStack<ElementType>::top() const
    {
        if(this->m_top == 0)
        {
            throw std::underflow_error("栈为空，获取栈顶元素失败！") ;
        }
        else
        {
            return this->m_pStack[m_top - 1];
        }
    }
}