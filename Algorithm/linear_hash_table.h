#include<iostream>
#include<vector>

namespace data_structrues::linear_hash_table
{
    //桶的状态
    enum class State
    {
        STATE_UNUSE,//从未使用过的桶
        STATE_USING,//正在使用的桶
        STATE_DEL,//元素被删除的桶
    };

    //桶的类型
    struct Bucket
    {
        Bucket(int key = 0,State state = State::STATE_UNUSE)
        : key_(key)
        , state_(state)
        {}
        int key_;//存储的数据
        State state_;//桶的状态
    };


    class LinearProbingHashTable;//前置声明
    using HT = LinearProbingHashTable;//类别名需要放在类定义后
    //如果不这样组合操作，而是将类别名放在完整类定义后，使用类内定义的函数就无法使用类别名
    //线性探测哈希表
    class LinearProbingHashTable
    {
    private:
        Bucket* m_table;//指向动态开辟的哈希表(数组)
        int m_size;//哈希表元素量
        int m_cap;//哈希表容量
        double m_load_factor;//哈希表装载因子

        static constexpr int PRIME_SIZE = 10; //素数表的大小(整形)
        static constexpr int m_prime_table[PRIME_SIZE]  = {3, 7, 23, 47, 97, 251, 443, 911, 1471, 42773};//素数表
        int m_prime_index; //当前使用的素数下标

        //哈希表扩容
        void expand()
        {
            //先自增素数表下标，如果超范围则抛出异常
            ++this->m_prime_index;
            if (this->m_prime_index >= PRIME_SIZE) throw "hash_table is too large,can't expand anymore!";
            int new_cap = HT::m_prime_table[this->m_prime_index];
            Bucket* new_table = new Bucket[new_cap];

            //将旧表元素填入新表
            for(int i = 0;i < this->m_cap;++i)
            {
                if (this->m_table[i].state_ == State::STATE_USING)
                {
                    //index是向新table中填入的位置
                    int index = this->m_table[i].key_ %  new_cap;
                    int max_index = index;
                    do
                    {
                        //如果新桶未被使用
                        if(new_table[index].state_ != State::STATE_USING)
                        {
                            //将旧桶填入新桶，并改变状态
                            new_table[index].key_ = this->m_table[i].key_;
                            new_table[index].state_ = State::STATE_USING;
                            break;
                        }
                        //不要漏了++index
                        index = (index + 1) % new_cap;
                    } while (index != max_index); 
                }
            }

            //最后将旧表内存释放，指针指向新表
            delete[] this->m_table;
            this->m_table = new_table;
            this->m_cap = new_cap;
        }
    
    public:
    //构造函数
        LinearProbingHashTable(int size = HT::m_prime_table[0],double load_factor = 0.75)
        : m_load_factor(load_factor)
        , m_prime_index(0)
        , m_size(0)
        {
            if(size >= this->m_prime_table[0])
            {
                //遍历素数表
                for(; this->m_prime_index < HT::PRIME_SIZE;++ this->m_prime_index)
                {
                    //如果用户传的size比素数表中当前素数小，break;
                    if(size < this->m_prime_table[this->m_prime_index]) break;
                }
                //跳出后，就找到了size对应表中的下标

                if(this->m_prime_index == HT::PRIME_SIZE)
                {
                    //用户传入的size值过大，已经超过最后一个素数，调整到最后一个素数
                    --m_prime_index;
                }
            }

            this->m_cap = HT::m_prime_table[this->m_prime_index];//容量为素数表中第一个比size大的素数
            this->m_table = new Bucket[this->m_cap];//创建一个数据类型为Bucket的数组
        }

        //析构函数
        ~LinearProbingHashTable()
        {
            delete[] this->m_table;
            m_table = nullptr;
        }

        //插入元素
        bool insert(int key)
        {
            //考虑扩容
            //先size+1再计算装载因子，先算再++会导致当前装载因子大于阈值
            double factor = (this->m_size + 1) * 1.0 / this->m_cap;//细节：隐式转换成浮点数运算，结果不会为0
            if (factor > this->m_load_factor)
            {
                expand();
            }

            //判断能不能插入
            int insert_index = key % this->m_cap;
            int max_index = insert_index;
            do{
                if(this->m_table[insert_index].state_ != State::STATE_USING)
                {
                    this->m_table[insert_index].key_ = key;
                    this->m_table[insert_index].state_ = State::STATE_USING;
                    ++this->m_size;
                    return true;
                }
                insert_index = (insert_index + 1) % this->m_cap;//这样处理循环，巧妙！
                //不用if哈希表已满，因为在此之前哈希表已经扩容了
            }
            while(insert_index != max_index);

            return false;
        }

        //删除元素
        bool erase(int key)
        {
            if (! this->m_size) return false;
            int max_index = (key % this->m_cap);
            int index = max_index;
            
            do
            {
                if (this->m_table[index].key_ == key && this->m_table[index].state_ == State::STATE_USING)
                {
                    this->m_table[index].state_ = State::STATE_DEL;
                    --this->m_size;  
                    return true;
                }
                index = (index + 1) % this->m_cap;
            } while (index != max_index && this->m_table[index].state_ != State::STATE_UNUSE);
            return false;
        }

        //查询
        bool find(int key) const
        {
            if (! this->m_size) return false;
            int max_index = (key % this->m_cap);
            int index = max_index;
            
            do
            {
                if (this->m_table[index].key_ == key && this->m_table[index].state_ == State::STATE_USING)
                {
                    return true;
                }
                index = (index + 1) % this->m_cap;
            } while (index != max_index && this->m_table[index].state_ != State::STATE_UNUSE);
            return false;            
        }
    
        //打印当前装载因子
        const double get_load_factor() const
        {
            return static_cast<double>(this->m_size) / static_cast<double>(this->m_cap);
        }
    };

    

}

