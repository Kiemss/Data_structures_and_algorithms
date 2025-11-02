#pragma once
#include<iostream>
#include<vector>
#include<list>

namespace data_structures::my_list_hash_table
{
    //节点结构，用于生成链表
    struct Node
    {
        Node(int val = 0)
        : val_(val)
        , next_(nullptr)
        {}
        Node* next_;//指向下一个节点
        int val_; //存储的元素
    };

    class SeparateChainingHashTable;//前置声明
    using HT = SeparateChainingHashTable;
    class SeparateChainingHashTable//链式哈希表
    {
    private:
        int m_size;//当前元素量
        int m_cap;//容量
        Node** m_table;//表头指针⭐注意是二级指针！
        double m_load_factor;//装载因子

        int m_prime_index;//当前使用素数表中的素数下标
        constexpr static int PRIME_SIZE = 10;//素数表大小
        static constexpr int m_prime_table[PRIME_SIZE]  = {3, 7, 23, 47, 97, 251, 443, 911, 1471, 42773};//素数表

        //扩容函数
        void expand()
        {
            ++this->m_prime_index;
            if (this->m_prime_index >= HT::PRIME_SIZE) throw "hash table is too large,can't expand anymore!";
            
            //新表头指向数组容量为素数表中当前容量下一位
            int new_cap = HT::m_prime_table[m_prime_index];
            Node** new_table = new Node*[new_cap];//注意链式哈希表是**
            for (int i = 0;i < new_cap;++i)
            {
                new_table[i] = new Node();
            }
            
            //提前定义用于遍历数组和拷贝的变量
            Node* cpyptr {};//用于拷贝节点    
            int cpyidx {};//提前定义定位拷贝位置的变量   
            for (int i = 0;i < m_cap;++i)
            {
                //下面是拷贝环节
                while (m_table[i]->next_)//循环条件：头指针的next不为空
                {
                    cpyptr = m_table[i]->next_;//确定拷贝节点
                    m_table[i]->next_ = cpyptr->next_;//移交

                    //下面是插入
                    cpyidx = cpyptr->val_ % new_cap;
                    //头插,使用移交而不是创建新节点
                    cpyptr->next_ = new_table[cpyidx]->next_;
                    new_table[cpyidx]->next_ = cpyptr;
                    cpyptr = nullptr;
                }      
            }
                //链表式释放内存
                for (int i = 0 ;i < this->m_cap;++i)
                {
                    delete m_table[i];//先释放哨兵节点
                }
                delete[] m_table;//再释放指针数组

                //拷贝完移交新表所有权
                this->m_cap = new_cap;
                this->m_table = new_table;  
        }
    public:
        //构造函数
        SeparateChainingHashTable(int cap = HT::m_prime_table[0],double load_factor = 0.75)
        : m_load_factor(load_factor)
        , m_size(0)
        , m_prime_index(0)
        {
            //如果容量比素数表中指定数大，那么++index
            //当index达到素数表最大值时，跳出循环，这里将异常处理交给expand函数
            while (cap >= HT::m_prime_table[m_prime_index] && m_prime_index < HT::PRIME_SIZE - 1)
            {
                ++m_prime_index;
            }
            m_cap = HT::m_prime_table[m_prime_index];

            //创建一个装载Node节点的数组,⭐记得初始化数组
            this->m_table = new Node*[m_cap] ;

            for (int i = 0;i < this->m_cap;++i)
            {
                m_table[i] = new Node();
            }
        }

        //析构函数
        ~SeparateChainingHashTable()
        {
            //先依次释放每个桶中的链表
            Node* delptr {};//提前定义
            for (int i = 0;i < this->m_cap;++i)
            {
                //头删法即可
                while ((this->m_table[i])->next_) 
                {
                    delptr = (this->m_table[i])->next_;
                    this->m_table[i]->next_ = delptr->next_;
                    delete delptr;
                    delptr = nullptr;
                }
                delete this->m_table[i];//记得释放哨兵节点
            }

            delete[] m_table;
            m_table = nullptr;
        }
    
        //插入
        void insert(int key)
        {
            //先判断是否需扩容
            //index为key模运算后要插入的位置下标
            int index = key % this->m_cap;

            //判断是否需要expand
            if ((this->m_size + 1) * 1.0 / this->m_cap > this->m_load_factor) expand();

            Node* new_node = new Node(key);//新建节点
            //⭐直接使用头插法即可，自己写代码用了尾插，笨比完了
            new_node->next_ =  this->m_table[index]->next_;
            this->m_table[index]->next_ = new_node;
            ++m_size;
        }
    
        //删除
        bool erase(int key)
        {
            //index为key模运算后要删除的位置下标
            int index = key % this->m_cap;
            Node* curptr = this->m_table[index];

            //遍历链表进行删除
            while (curptr->next_ != nullptr)//选择对next进行判断，方便删除节点后链接前后节点
            {
                if (curptr->next_->val_ == key)//如果next的下个节点值为key，那么可以删除
                {
                    Node* delptr = curptr->next_;
                    if (delptr->next_) curptr->next_ = delptr->next_;//如果下下个节点不为空，则链接
                    delete delptr;//删除指定节点
                    --m_size;
                    return true;
                }
                curptr = curptr->next_;
            }
            return false;
        }   
        
        //查询
        bool find(int key)
        {
            //index为key模运算后要寻找的位置下标
            int index = key % this->m_cap;
            Node* curptr = this->m_table[index];

            //遍历链表进行查询（套用了删除的代码）
            while (curptr->next_ != nullptr)
            {
                if (curptr->next_->val_ == key)//如果next的下个节点值为key，那么可以删除
                {
                    return true;
                }
                curptr = curptr->next_;
            }
            return false;
        }   

    };
}

namespace data_structures::list_hash_table
{
    using namespace std;
    class SeparateChainingHashTable;//前置声明
    using HT = SeparateChainingHashTable;
    class SeparateChainingHashTable//链式哈希表
    {
    private:
        vector<list<int>> table_;//哈希表的数据结构
        int useBucketNum_; //记录桶的个数
        double loadFactor_; //记录哈希表装载因子

        static constexpr int PRIME_SIZE = 10;//素数表大小
        static constexpr int m_prime_table[PRIME_SIZE]  
        = {3, 7, 23, 47, 97, 251, 443, 911, 1471, 42773};//素数表
        int primeIdx_;//当前使用的素数下标

    public:
        //构造函数
        SeparateChainingHashTable(int size = HT::m_prime_table[0],double loadFactor = 0.75)
        : useBucketNum_(0)
        ,loadFactor_(loadFactor)
        , primeIdx_(0)
        {}
    };


}
