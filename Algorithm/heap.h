#pragma once
#include<iostream>
#include<functional>
//优先队列
namespace data_structures::priority_queue
{
    //大根堆
    class PriorityQueue
    {
        public:
        using Comp = std::function<bool(int,int)>;

        private:
        int* m_que;//指向动态扩容的数组
        int m_cap;//动态数组的容量
        int m_size;//数组元素的个数
        Comp m_comp;//比较器对象

        //下沉函数
        void sift_down()
        {
            int val = this->m_que[0];//提前记录用来下沉的值
            int index = 0;
            //循环条件：还有左孩子
            while(index * 2 + 1 < this->m_size)
            //另一种循环条件：index < (size - 1)/2这是判断index是否比能有子节点的节点最大下标小
            {
                int child = index * 2 + 1;//index的左子节点
                //判断右子节点是否存在的方法：和size作比较
                if(child + 1 <= this->m_size && this->m_comp(this->m_que[child], this->m_que[child + 1]))
                {
                    ++child;//右节点记录为child,表示更大
                }
                //子节点比父节点更大，子节点上浮
                if(this->m_comp(this->m_que[child] , val))//注意不要用else if
                //注意this->m_que[val]可能会被覆盖，应该用val比较
                {
                    this->m_que[index] = this->m_que[child];
                    index = child;
                }
                //父节点大，直接break
                else if(this->m_que[child] <= this->m_que[index])
                {
                    break;
                }
            }
            this->m_que[index] = val;
        }

        //上浮函数
        void sift_up(int index)
        {
            int val = this->m_que[index];
            while(index > 0)
            {
                int father_index = (index - 1)/2;
                if(this->m_comp(val,this->m_que[father_index]))//判断父节点是否比val小
                 {
                    this->m_que[index] = this->m_que[father_index];//父节点下沉
                    index = father_index;
                }
                else
                {
                    //确定新节点位置
                    break;
                }            
            }
            this->m_que[index] = val;
        }

        //扩容函数
        void expand(int new_cap)
        {
            int* new_que = new int[new_cap];
            memcpy(new_que,this->m_que,this->m_size * sizeof(int));
            delete[] this->m_que;
            this->m_que = new_que;
            this->m_cap = new_cap;
        }

        public:
        //遍历数组
        void traverse()
        {
            for(int i  = 0;i < m_size;++i)
            {
                std::cout << m_que[i] << " ";
            }
            std::cout << std::endl;
        }
        
        //构造函数
        explicit PriorityQueue(int cap = 20,Comp comp = std::greater<int>())//小根堆换成这个[](int a,int b){return a < b;}
        : m_cap(cap)
        , m_size(0)
        , m_comp(comp)
        {
            this->m_que = new int[m_cap];
        }

        //析构函数
        ~PriorityQueue()
        {
            delete[] this->m_que;
            m_que = nullptr;
        }

        //查看堆顶元素
        const int top() const
        {
            return this->m_que[0];
        }

        //判断堆空
        bool empty() const
        {
            return !this->m_size;
        }

        //入堆
        void push(int val)
        {
            //扩容
            if(this->m_size == this->m_cap)
            {
                expand(2 * this->m_size);
            }

            //如果堆空则直接入到堆顶
            if(this->empty())
            {
                this->m_que[0] = val;
                ++this->m_size;
                return;
            }
            else
            {
            this->m_que[m_size] = val;
            int index = this->m_size;
            sift_up(index);
            }
            ++this->m_size;
        }
    
        //出堆
        void pop()
        {
            if(!m_size) throw std::underflow_error("heap is empty!");
            //先将末尾元素移到堆顶，防止只有一个元素时下标越界
            this->m_que[0] = this->m_que[this->m_size - 1];
            --this->m_size;
            if(this->m_size > 0)//如果堆里还有元素再调整堆
            sift_down();//调用下沉函数，将拉到最顶的末尾元素调整到合适位置
        }

        //获取元素数量
        int size() const
        {
            return this->m_size;
        }
    };
}