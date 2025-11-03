#pragma once
#include<iostream>
#include<string>

//顺序队列
namespace SeqQueueNameSpace
{
    template<typename ElementType>
    class SeqQueue
    {
        private:
        ElementType* m_pQue;
        int m_cap;
        int m_front;
        int m_rear;
        size_t m_size;//队列元素个数

        public:
        //构造函数
        SeqQueue(size_t cap = 10);
        //析构函数
        ~SeqQueue();
        //出队
        void pop();
        //入队
        void push(ElementType val);
        //获取队头元素
        const ElementType& front() const;
        //获取队尾元素
        const ElementType& rear() const;
        //判断队列是否为空
        bool empty() const;
        //获取队列长度
        size_t size() const;

        private:
        //队列扩容
        void expand(int size);
    };

    //构造函数
    template<typename ElementType>
    SeqQueue<ElementType>::SeqQueue(size_t cap) //判断队满队空方法导致队列容量比传入的小，但是仍然让m_cap = cap，只需提前说明接口问题
    : m_cap(cap)
    , m_front(0)
    , m_rear(0)
    , m_size(0)
    {
        this->m_pQue = new ElementType[this->m_cap];
    }

    //析构函数
    template<typename ElemnetType>
    SeqQueue<ElemnetType>::~SeqQueue()
    {
        delete[] this->m_pQue;
        m_pQue = nullptr;
    }

    //入队
    template<typename ElementType>
    void SeqQueue<ElementType>::push(ElementType val)
    {
        //为了区分队满和队空，让队列容量比数组容量小1
        if((this->m_rear + 1) % this->m_cap == this->m_front)
        {
            expand(2 * this->m_cap);
        }
        this->m_pQue[this->m_rear] = val;
        this->m_rear = (this->m_rear + 1) % this->m_cap; 
        ++this->m_size;
    }

    //出队
    template<typename ElementType>
    void SeqQueue<ElementType>::pop()
    {
        if(this->m_front == this->m_rear)
        {
            throw std::underflow_error("queue is empty!");
        }
        this->m_front = (this->m_front +1) % this->m_cap;
        --this->m_size;
    }

    //判断队空
    template<typename ElementType>
    bool SeqQueue<ElementType>::empty() const
    {
        return (this->m_front == this->m_rear);
    }
    //获取队头元素
    template<typename ElementType>
    const ElementType& SeqQueue<ElementType>::front() const
    {
        if(this->m_front == this->m_rear)
        {
            throw std::underflow_error("queue is empty!");
        }
        return this->m_pQue[this->m_front];
    }

    //获取队尾元素
    template<typename ElementType>
    const ElementType& SeqQueue<ElementType>::rear() const
    {
        if(this->m_front == this->m_rear)
        {
        throw std::underflow_error("queue is empty!");
        }
        return this->m_pQue[(this->m_rear + this->m_cap -1) % this->m_cap];
    }

    //获取队列长度
    template<typename ElementType>
    size_t SeqQueue<ElementType>::size() const
    {
        return this->m_size;
        /*
        if(this->m_front > this-> m_rear)
        {
            return this->m_cap - (this->m_front - this->m_rear );
        }
        else
        {
            return this->m_rear - this->m_front;
        }
        */
       //下面是统一公式
       //return (m_rear - m_front + m_cap) % m_cap
    }

    //队列扩容
    template<typename ElementType>
    void SeqQueue<ElementType>::expand(int size)
    {
        this->m_cap = size;
        ElementType* new_que = new ElementType[this->m_cap];
        size_t index = 0;
        while(this->m_front != this->m_rear)
        {
            new_que[index] = this->m_pQue[m_front];
            ++index;
            this->m_front = (this->m_front + 1) % this->m_cap;
        }
        m_pQue = new_que;
        this->m_rear = index;
        this->m_front = 0;
        delete new_que;
        new_que = nullptr;
    }    
}