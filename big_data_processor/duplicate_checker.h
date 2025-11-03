#pragma once
#include<iostream>
#include<vector>
#include<unordered_set>
#include<unordered_map>
#include<string>
using namespace std;

#include <vector>
#include <random>



//获取第一个重复数据
void first_duplicate_finder(vector<int> vec)
{
    //找第一个重复出现的数字
    unordered_set<int> s1;
    for ( auto key : vec)
    {
        auto it = s1.find(key);
        if (it == s1.end())//在s1中找到key
        {
            s1.insert(key);
        }
        else//找到了就输出
        {
            cout << "key=" << key << endl;
            break;//找所有
        }
    }
}

//获取所有重复数据并统计or获取所有非重复数据
void all_duplicate_finder(vector<int> vec)
{
    //找所有重复出现的数字并统计
    unordered_map<int,int> m1;
    for (auto key : vec)
    {
        auto it = m1.find(key);//查找m1中key，it为其上级的指针
        if (it == m1.end())
        {
            m1.emplace(key,1);//没找到就插入key并初始化
            //emplace()是原地构造新对象，insert()需要先构造，再插入
        }
        else
        {
            ++it->second;//找到了就++times
        }
        /*
        下面这行代码的工作原理：
         1. m1[key] 会查找键key：
            - 如果key存在，返回对应值的引用
            - 如果key不存在，自动插入 {key, 0} 并返回值的引用
        2. ++操作符对返回的引用进行自增
            这样就实现了：已存在则计数+1，不存在则插入并设为1
        */
        //++m1[key];
    }

    //打印输出
    for (auto pair : m1)
    {
        if (pair.second > 1)
        cout << pair.first << ":" << pair.second << endl;
    }
}

//获取所有数据，重复数据不会重复输出
void get_no_duplicate(vector<int> vec)
{
    unordered_set<int> s1;
    for (auto key : vec)
    {
        //不用判断该元素是否存在于s1中，直接插入即可
        //set会自动处理重复元素（重复了就不插入）
        s1.emplace(key);
    }

    for (auto key : s1)
    {
        cout << key << endl;
    }
}

//输出第一个不重复的元素
template<typename ElementType>
void get_first_no_duplicate(vector<ElementType> vec)
{
    unordered_map<ElementType,int> m1;
    for (auto key : vec)
    {
        ++m1[key];
    }

    //⭐注意不能直接遍历m1，因为map不是顺序容器!
    for (auto key : vec)
    {
        if (m1[key] == 1)
        {
            cout << key << endl;
            return;
        }
    }
}

//位图
void bit_map_duplicate_finder(vector<int> vec,int size)
{
    char bit_map[(size + 7) / 8] {0};
    int index{};//插入值在位图中的下标
    int offset{};
    for(auto key : vec)
    {
        index = key / 8;
        offset = key % 8;
        //位图神奇的一点：字节内部下标是递减的
        if ((bit_map[index] & (1 << offset)) == 0) //该位未被设置
        {
            bit_map[index] |= (1 << offset);//设置该位为0
        }
        else
        {
            cout << key << endl;
        }
    }
}

//布隆过滤器
class BloomFilter{};
