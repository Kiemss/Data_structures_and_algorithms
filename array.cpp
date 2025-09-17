#include <iostream>
#include<cstring>
#include<cstdlib>
#include<ctime>

//使用memcpy()函数

using std::cout;
using std::endl;
using std::cin;

template<typename ElementType>//声明模板类
class Array //数组实现
{
    public:
    typedef size_t SizeType;//类外使用需要作用域，方便修改index为int扩展负数索引
    private:
        size_t m_cap; //数组容量
        size_t m_cur; //数组有效元素个数
        ElementType* m_pArr; //指向可扩容的数组内存

    public:
        explicit Array(size_t size = 10);
        //构造函数,默认值应该在类内声明而非类外定义中.
        //类内外同时给默认值语法上是不允许的，会导致二义性问题
        //类内给默认值->使用者看到声明就知道有默认值
        //类外给默认值->使用者不知道有默认值
        ~Array();//析构函数
        
        //尾插元素
        void push_back(ElementType val);
        //尾删元素
        void pop_back();
        //插入元素
        void insert(SizeType pos,ElementType val);
        //按位置删除
        void remove_index(SizeType index);
        //按元素删除
        void remove_element(ElementType val);
        //元素查询
        int find(ElementType val);
        //遍历
        void traverse();
    private:
        //数组扩容
        void expand(size_t size);
};

//构造函数
template<typename ElementType>//类外定义函数对于模板类还需要template依次
Array<ElementType>::Array(size_t size) : m_cap(size) , m_cur(0)
//注意：<ElementType>放在类名后，这个是必须的。对于普通函数，则不需要<ElementType>
{
    this->m_pArr = new ElementType[size];
}

//析构函数
template<typename ElementType>
Array<ElementType>::~Array()
{
    //if(m_pArr != nullptr)
    //这个判断是无意义的，因为delete空指针是空操作，没有坏处。
    //我们需要的是保证指针不是野指针
    delete[] m_pArr;
    m_pArr = nullptr; //释放内存后一定要置空，防止野指针。注意是nullptr而非null(cpp中没有null)
}

//数组扩容
template<typename ElementType>
void Array<ElementType>::expand(size_t size)
{
    ElementType* ptr = new ElementType[size];
    memcpy(ptr,m_pArr,sizeof(ElementType)*m_cur);
    delete[] m_pArr;
    m_pArr = ptr;
    m_cap = size;
}

//尾删元素
template<typename ElementType>
//模板类成员函数无论有没有使用模板参数，类外定义都必须写template<typename T>
void Array<ElementType>::pop_back()
{
    if(this->m_cur != 0)
    {
        m_cur--;
    }
}

//尾插元素
template<typename ElementType>
void Array<ElementType>::push_back(ElementType val)
{
    if(this->m_cur < this->m_cap)
    {
        this->m_pArr[m_cur++] = val;
    }
    else
    {
        this->expand(this->m_cap * 2);
        this->push_back(val);
    }
}

//按位置插入元素
template<typename ElementType>
void Array<ElementType>::insert(Array::SizeType pos,ElementType val)
{
    if( pos > this->m_cur)
    {
        return; //throw "pos invalid!"
    }
    
    if ( m_cap ==  m_cur)
    {
        expand(m_cap * 2);
    }

    //移动元素
    for(size_t i = m_cur; i >= pos + 1;i--)
    //注意size_t越界问题，如果初始化i为m_cur-1,m_cur=0时会越界！
    {
        this->m_pArr[i] = this->m_pArr[i-1];
    }

    //插入
    m_pArr[pos] = val;
    m_cur++;
}

//按位置删除元素
template<typename ElementType>
void Array<ElementType>::remove_index(Array<ElementType>::SizeType pos)
{
    if ( pos >= m_cur )
    {
        return;//抛出异常
    }
    
    //删除元素：直接覆盖
    for(size_t i = pos; i < m_cur - 1; i++)
    {
        this->m_pArr[i] = this->m_pArr[i+1];
    }
    
    m_cur--;
}

//按元素删除
template<typename ElementType>
void Array<ElementType>::remove_element(ElementType val)
{
    for(size_t i = 0;i < m_cur;i ++)
    {
        if(this->m_pArr[i] == val)
        {
            this->remove_index(i);
            i++;
        }
    }
    return;
}

//元素查询
template<typename ElementType>
int Array<ElementType>::find(ElementType val)
{
    for (size_t i = 0; i < m_cur;i ++)
    {
        if(m_pArr[i] == val)
        {
            return i;
        }
    }
    return -1;
}

//遍历
template<typename ElementType>
void Array<ElementType>::traverse()
{
    if(this->m_cur != 0)
    {
        for(size_t i = 0; i < m_cur;i++)
        {
            std::cout << this->m_pArr[i] << " ";
        }
        std::cout << std::endl;
    }
}

int main()
{
    Array<int> arr;
    srand(time(0));
    for(int i = 0; i < 10; i++)
    {
        arr.push_back(rand()%100);
    }
    arr.traverse();
    arr.pop_back();
    arr.traverse();
    arr.insert(0,100);
    arr.traverse();
    std::cout << arr.find(100) << std::endl;
    arr.remove_element(100);
    arr.traverse();
    return 0;
}