#pragma once
#include<iostream>
#include<queue>
#include<vector>
#include<algorithm>
#include<unordered_map>
#include<functional>
using std::swap;
using std::pair;
using std::vector;
using std::priority_queue;//优先级队列，默认是大根堆
using std::unordered_map;

//使用大根堆处理最小k个元素
void topk_big(const vector<int>& vec,int topk)
{
    if (topk <= 0 || vec.size() < (size_t)topk) throw "topk bigger than size!";

    priority_queue<int> max_heap;
    //先填入k个元素
    for ( int i = 0;i < topk;++i)
    {
        max_heap.push(vec[i]);
    }

    //再整个遍历，如果比堆顶小则出堆顶并入堆
    for (int i = topk; i < vec.size(); ++i)
    {
        if (vec[i] < max_heap.top())
        {
            max_heap.pop();
            max_heap.push(vec[i]);
        }
    }

    //最后遍历
    while(!max_heap.empty())
    {
        std::cout << max_heap.top() << " ";
        max_heap.pop();
    }

    std::cout << std::endl;
    return;
}

//使用小根堆处理最大k个元素
void topk_small(const vector<int>& vec,int topk)
{
    if (topk <= 0 || vec.size() < (size_t)topk) throw "topk bigger than size!";

    priority_queue<int,vector<int>,std::greater<int>> min_heap;//将less改为greater，变成小根堆
    //先填入k个元素
    for ( int i = 0;i < topk;++i)
    {
       min_heap.push(vec[i]);
    }

    //再整个遍历，如果比堆顶大则出堆顶并入堆
    for (int i = topk; i < vec.size(); ++i)
    {
        if (vec[i] > min_heap.top())
        {
           min_heap.pop();
           min_heap.push(vec[i]);
        }
    }
 
    //最后遍历
    while(!min_heap.empty())
    {
        std::cout <<min_heap.top() << " ";
       min_heap.pop();
    }
    std::cout << std::endl;
    return;
}

//求重复次数最少的前k个元素,该实现未重载比较器
void duplicate_min_topk(const vector<int>& vec,int topk)
{
    unordered_map<int,int> m1;
    for (auto key : vec)
    {
        ++m1[key];
    }

    int k = topk;
    priority_queue<pair<int,int>> max_heap;//默认为less，大根堆
    for (pair<int,int> key : m1)
    {
        if (k > 0)
        {
            max_heap.push(key);
            --k;
            continue;
        }

        if (key.second < max_heap.top().second)
        {
            max_heap.pop();
            max_heap.push(key);
        }
    }

        for (int i = 0;i < topk;++i)
        {
            std::cout << max_heap.top().first << ":" << max_heap.top().second << std::endl;
            max_heap.pop();
        }
}

//求重复次数最多的前k个元素，该实现重载了比较器
void duplicate_max_topk(const vector<int>& vec,int topk)
{
    //先获取重复次数
    unordered_map<int,int> m1;
    for (auto key : vec)
    {
        ++m1[key];
    }

    //构建小根堆，保证每次排除的都是堆中最小元素
    auto less = [](const pair<int,int>& a,const pair<int,int>& b)->bool
    {
        return a.second > b.second; //保证堆顶元素最小，构建小根堆
    };
    //参数列表不能传入变量，因此传入的的是对象decltype(less)，因为缺少变量，所以后面还要用lambda变量less初始化pq1
    priority_queue<pair<int,int>,vector<pair<int,int>>,decltype(less)> min_heap(less);//初始化一个比较重复次数的小根堆

    //将前topk个元素填入堆
    auto it = m1.begin();//遍历无序容器的方法：使用迭代器
    for (int i = 0;i < topk;++i )
    {
        min_heap.push(*it);
        ++it;
    }

    //再遍历比较
    for (;it != m1.end();++it)
    {
        if (min_heap.top().second < it->second)
        {
            min_heap.pop();
            min_heap.push(*it);
        }
    }

    //出堆+打印
    while (!min_heap.empty())
    {
        std::cout << min_heap.top().first << ":" << min_heap.top().second << std::endl;
        min_heap.pop(); 
    }
    return;
}

//使用快排分割法找最小topk.快排分割运算函数,仅排序不输出
int Partation(vector<int>& vec,int beg,int end)
{
    int i = beg;
    int j = end;
    //三数取中法找基准数
    int mid_index = (i + j)/2;
    if (vec[beg] > vec[mid_index]) swap(vec[beg],vec[mid_index]);
    if (vec[mid_index] > vec[end]) swap(vec[mid_index],vec[end]);
    if (vec[beg] > vec[mid_index]) swap(vec[beg],vec[mid_index]);
    //交换中间值与基准数
    swap(vec[beg],vec[mid_index]);

    //确定基准数
    int base_val = vec[beg];

    //开始快排
    while (i < j)
    {
        while (vec[j] >= base_val && i < j) --j;
        vec[i] = vec[j];
        while (vec[i] <= base_val && i < j) ++i;
        vec[j] = vec[i];
    }
    vec[i] = base_val;//结束快排
    return i;
}

//快排封装函数
void select_topk(vector<int>& vec,int beg,int end,int topk)
{
    int pos = Partation(vec,beg,end);
    if (pos == topk - 1) //刚好能取前topk个数
    {
        return;
    }
    else if (pos < topk - 1) //topk落在pos的右边，右边进行快排分割
    {
        select_topk(vec, pos + 1, end, topk);
    }
    else //左边进行快排分割
    {
        select_topk(vec, beg, pos - 1, topk);
    }
}