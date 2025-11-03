#pragma once
#include<iostream>
#include<vector>

//优化获取素数函数
void get_primes_optimized(int num)
{
    if(num < 2) return;

    std::vector<bool> vec(num + 1,true);
    vec[0] = vec[1]  = false;

    // 提前将偶数置为false，减少后面的内层循环
    // 从4开始（2是素数要保留），步长为2
    for(int i = 4;i <= num;i += 2)
    {
        vec[i] = false;
    }

    //循环，将素数的倍数置为false
    for(int i = 3;i * i <= num;i += 2)
    //优化：i*i<num，比i*i大的会被比i小的数筛选过
    {
        if(vec[i])
        {
            for(int j = i * i;j <= num;j += 2 * i)
            // 优化1：j=i*i，比i*i小的倍数已被更小的素数筛选过
            // 优化2：j+=2*i，只标记奇数倍（偶数已被提前排除）
            {
                vec[j] = false;
            }
        }
    }

    //输出
    for(int i = 2;i <= num;++i)
    {   
        if(vec[i]) std::cout << vec[i] << " ";//只输出素数
    }
    std::cout << std::endl;
}

//图求最短路径的佛洛依德算法
void floyd_algorithm(int v ,int** dist) 
{
   
    const int INF = 10000000;
    int path[v][v]{};
    // 初始化path数组
    for(int i = 0; i < v; i++) {
        for(int j = 0; j < v; j++) {
            if(i == j) {
                path[i][j] = i;  // 自己到自己的前驱是自己
            } else if(dist[i][j] < INF) {
                path[i][j] = i;  // 直接路径，j的前驱是i
            } else {
                path[i][j] = -1; // 无路径
            }
        }
    }

    for(int k = 0;k < v;++k)
    {
        for (int i = 0;i < v;++i)
        {
            for (int j = 0; j < v;++j)
            {
                if (dist[i][k] + dist[k][j] < dist[i][j])
                {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    //i到j的路径的最后一段复用k到j的路径的最后一段，因为此时k作为被插入的节点
                    path[i][j] = path[k][j];
                }
            }
        }
    }
}