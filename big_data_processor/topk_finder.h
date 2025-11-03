#pragma once
#include<iostream>
#include<queue>
#include<vector>
#include<algorithm>
using std::vector;
using std::priority_queue;//优先级队列，默认是大根堆

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

//求重复次数最多的前k个元素
void duplicate_topk(const vector<int>& vec,int topk)
{
    
}