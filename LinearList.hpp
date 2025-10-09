#pragma once
#include<iostream>

template<typename ElementType>
class LinearList
{
    public:
    typedef size_t SizeType;//类外使用需要作用域，方便修改index为int扩展负数索引
    public:
    //构造函数
    LinearList(){};
    //析构函数
    ~LinearList(){};
    //求表最大体积
    virtual size_t size()  const = 0; 
    //求表长度
    virtual size_t length() const = 0; 
    //在表中搜索val的位置，返回位置序号
    virtual int find(ElementType val) const = 0; 
    //在表中定位第index个元素的位置。链表函数
    //virtual size_t locate(size_t index) const = 0; 
    //取第index个函数的值。链表函数
    //virtual ElementType* getData(size_t index) const = 0; 
    //修改第index个元素的值为val。链表函数
    //virtual void setData(size_t index, ElementType& val) = 0;
    //插入元素
    virtual bool insert(SizeType pos,ElementType val) = 0;
    //删除第index个表项，通过val返回->最优雅：定义最小接口，分离删除和返回元素
    //这里是按位置删除
    virtual bool remove_index(SizeType index) = 0;
    //判断表是否为空
    virtual bool is_empty()const = 0;
    //将线性表置为空表
    virtual void make_empty() = 0;
    //判断表是否已满
    virtual bool is_full() const = 0;
    //排序
    virtual void sort() = 0;
    //输入表项,插入等操作
    virtual void input() = 0;
    //输出表项，查找等操作
    virtual void output() = 0;
    //排序，输入输出表项过于宽泛，子类解决方案为空实现或者复用子类代码
    //不使用通过传入不同参数的方法来决定调用不同的子类函数
    //拷贝构造
    //virtual LinearList<ElementType> operator=(LinearList<ElementType>& left_List,LinearList<ElementType>& right_List) = 0;
    
};
