#pragma once
#include <iostream>
#include<cstring>
#include<cstdlib>
#include<ctime>
#include<type_traits>
#include"LinearList.hpp"

//使用memcpy()函数

using std::cout;
using std::endl;
using std::cin;

namespace ArrayNameSpace
{
template<typename ElementType>//声明模板类
class Array : public LinearList<ElementType>//数组实现.⭐注意继承的是类模板而不是类，要写<ElementType>
{
    public:
    using SizeType = typename LinearList<ElementType>::SizeType;  
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
        
        //基类虚函数实现
        //⭐求表最大体积
        size_t size() const override;
        //⭐求表长度
        size_t length() const override;
        //⭐判断表是否为空
        bool is_empty() const override;
        //⭐将线性表置空
        void make_empty()  override;
        //⭐判断表是否已满
        bool is_full() const override;
        //⭐排序
        void sort() override{}
        //⭐输入表项
        void input() override{}
        //⭐输出表项 
        void output()override{}
        //⭐插入元素
        bool insert(SizeType pos,ElementType val) override;
        //⭐按位置删除
        bool remove_index(SizeType index) override;
        //⭐元素查询
        int find(ElementType val) const override;

        //Array特有的方法
        //尾插元素
        void push_back(ElementType val);
        //尾删元素
        void pop_back(); 
        //按元素删除
        void remove_element(ElementType val);
        //遍历
        void traverse();
        //逆序元素
        void reverse();
        //对于整形数组，让奇数到左边，偶数到右边
        void int_sort();
        void int_sort_better();
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

//⭐求表最大体积
template<typename ElementType>
size_t Array<ElementType>::size() const
{
    return this->m_cap;
}

//⭐求表长度
template<typename ElementType>
size_t Array<ElementType>::length() const
{
    return this->m_cur;
}

//⭐判断表是否为空
template<typename ElementType>
bool Array<ElementType>::is_empty() const
{
    return this->m_cur == 0;
}

//⭐将表置空
template<typename ElementType>
void Array<ElementType>::make_empty() 
{
    this->m_cur = 0;
}

//⭐判断表是否已满
template<typename ElementType>
bool Array<ElementType>::is_full() const
{
    return this->m_cur == this->m_cap;
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

//⭐按位置插入元素
template<typename ElementType>
bool Array<ElementType>::insert(SizeType pos,ElementType val)
{
    if( pos > this->m_cur)
    {
        return false; //抛出异常
    }
    
    if ( this->m_cap ==  this->m_cur)
    {
        expand(this->m_cap * 2);
    }

    //移动元素
    for(size_t i = this->m_cur; i >= pos + 1;i--)
    //注意size_t越界问题，如果初始化i为m_cur-1,m_cur=0时会越界！
    {
        this->m_pArr[i] = this->m_pArr[i-1];
    }

    //插入
    this->m_pArr[pos]  = val;
    this->m_cur++;
    return true;
}

//⭐按位置删除元素
template<typename ElementType>
bool Array<ElementType>::remove_index(SizeType pos)
{
    if ( pos >= m_cur )
    {
        return false;//抛出异常
    }
    
    //删除元素：直接覆盖
    for(size_t i = pos; i < m_cur - 1; i++)
    {
        this->m_pArr[i] = this->m_pArr[i+1];
    }
    m_cur--;
    return true;
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
            i--;
        }
    }
    return;
}

//⭐元素查询
template<typename ElementType>
int Array<ElementType>::find(ElementType val) const
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

//逆序元素
template<typename ElementType>
void Array<ElementType>::reverse()
{
    if(this->m_cur != 0)
    {
        ElementType* start_ptr = this->m_pArr;
        ElementType* end_ptr = this->m_pArr + this->m_cur -1;
        //下面的版本不如上面的版本清晰明了
        // ElementType* left = &this->m_pArr[0];         
        //ElementType* right = &this->m_pArr[this->m_cur - 1]; 
        
        ElementType temp;
        //ElementType* temp_ptr; 不要使用指针存储临时变量！
        
        while(start_ptr < end_ptr)
        //for(size_t i = 0;i < this->m_cur/2 ;i++)
        //这个条件判断过于复杂了，不够好
        {
            temp = *start_ptr;
            *start_ptr = *end_ptr;
            *end_ptr = temp;
            start_ptr ++;
            end_ptr --;
        }
    }
}

//算法：整形数组奇偶排序问题
//该算法效率低下，后面有更好的算法
template<typename ElementType>
void Array<ElementType>::int_sort()
{
    static_assert(std::is_same_v<ElementType,int> || std::is_same_v<ElementType,size_t>,"编译失败，只有int或size_t类型Array才能调用int_sort函数");
    size_t start = 0;
    size_t end = this->m_cur - 1;
    ElementType temp;
    while(start < end)
    {
        if(this->m_pArr[start] % 2 == 1)
        {
            temp = m_pArr[start];
            for(size_t i = start;i < end;i++)
            {
                m_pArr[i] = m_pArr[i+1];
            }
            m_pArr[end] = temp;
            end--;
            start = 0;
        }
        start++;
    }
}

template<typename ElementType>
void Array<ElementType>::int_sort_better()
{
    static_assert(std::is_same_v<ElementType,int> || std::is_same_v<ElementType,size_t>,"编译失败，只有int或size_t类型Array才能调用int_sort函数");
    size_t start = 0;
    size_t end = this->m_cur -1;
    ElementType temp {};
    while(start < end)
    {
        while(this->m_pArr[start] %2 == 0)
        {
            start++;
        }
        while(this->m_pArr[end] %2 ==1)
        {
            end--;
        }
        if(start <end )//这个是防止排完序后再次调换，使得中间两奇偶对调了
        {
            temp = m_pArr[start];
            m_pArr[start] = m_pArr[end];
            m_pArr[end] = temp;
        }
    }
}

//给一个数组，删除指定元素，返回剩余元素数，空间复杂度O(1)
template<typename ElementType>
int delete_element(ElementType* arr,size_t arr_size,ElementType& val)
{
    ElementType* l_ptr = arr;
    ElementType* r_ptr = arr + arr_size - 1;
    while(l_ptr <= r_ptr)
    { 
        if(*r_ptr == val)
        {
            r_ptr--;
            continue;
        }

        if(*l_ptr == val)
        {
            *l_ptr = *r_ptr;
            r_ptr--;
        }
        l_ptr++;
    }
    return l_ptr - arr ;
}

//快慢指针法删除元素
template<typename ElementType>
int better_delete_element(ElementType* arr, size_t arr_size, const ElementType& val)
{
    ElementType* slow = arr;  // 慢指针：指向下一个有效元素位置
    ElementType* fast = arr;  // 快指针：遍历所有元素
    
    for(size_t i = 0; i < arr_size; i++)
    {
        if(*fast != val)      // 如果不是要删除的元素
        {
            *slow = *fast;    // 保留该元素
            slow++;           // 慢指针前进
        }
        fast++;               // 快指针始终前进
    }
    
    return slow - arr;        // 返回新长度
}
}