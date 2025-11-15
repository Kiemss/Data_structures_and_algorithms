#pragma once
#include<iostream>
#include<vector>
#include<queue>
#include<limits>
using namespace std;

const unsigned int INF = INT_MAX; //不使用int，防止溢出

//迪杰斯特拉算法：通过找一个节点到其他所有节点的最短路径找一个节点到任意节点的最短路径->⭐贪心算法的应用
int dijkstra(vector<vector<unsigned int>> &graph,int start, int end)
{
    const int N = graph.size(); //顶点数
    vector<unsigned int> dis(N, 0); //最短距离集合S(solve)
    vector<bool> use(N,false); //标记哪些节点最短距离已经被确定了，逻辑上区分S和U(unsettled)\Q(question)

    //把start放入S集合
    use[start] = true;

    //初始化start到其他顶点的权值
    for (int i = 0; i < N; ++i)
    {
        if (use[i] == false)
        {
            dis[i] = graph[start][i];
        }
    }

    //把U集合中的顶点处理完
    for (int i = 0; i < N; ++i) //O(n)
    {   
        //循环
        unsigned int min = INF;
        int index = -1; //记录最小距离顶点下标
        for (int j = 0;j < N; ++j) //O(n)
        {
            if (use[j] == false & dis[j] < min) //未被确定的
            {
                min = dis[j];
                index = j;
            }
        }
        
        if (index == -1) //循环结束了还没找到最小节点（也就是全是INF），无法到达其他节点了
        {break;}
        //把选出来的顶点加入Q中
        use[index] = true;
        dis[index] = min;

        //把U集合中剩余的顶点的权值信息更新一下
        for (int j = 0; j < N; ++j)
        {
            //通过新节点中转到达其他节点的路径如果比不通过要小，那么就将最短路径更改为前者，否则不变
            if (use[j] == false && min + graph[index][j] < dis[j])
            {
                dis[j] = min + graph[index][j];
            }
        }
    }
    //测试打印
    for (unsigned int d : dis)
    {
        cout << d << " ";
    }
    cout << endl;
    //最后输出顶点的最短路径
    return dis[end];
}


//优化迪杰斯特拉算法：用小根堆作为Q
int dijkstra_better(vector<vector<unsigned int>> &graph,int start, int end)
{
    const int N = graph.size(); //顶点数
    vector<unsigned int> dis(N, 0); //最短距离集合S(solve)
    vector<bool> use(N,false); //标记哪些节点最短距离已经被确定了，逻辑上区分S和U(unsettled)\Q(question)

    //定义小根堆
    priority_queue<pair<unsigned int,int>, vector<pair<unsigned int, int>>, greater<pair<unsigned int, int>>> que;

    //把start放入S集合
    use[start] = true;

    //初始化start到其他顶点的权值
    for (int i = 0; i < N; ++i)
    {
        dis[i] = graph[start][i];
        //把除了start顶点的其他顶点全部放入U集合小根堆中
        if (i != start)
        {
            que.emplace(graph[start][i],i);
        }
    }

    while (!que.empty())
    {
        auto pair = que.top(); //取出小根堆堆顶，距离最小元素
        que.pop();

        if (pair.first == INF) {break;} //小根堆堆顶为INF，说明无其他有效路径

        int index = pair.second;
        int min = pair.first;

        //⭐如果该节点已经被处理过，那以后就不要再处理了。比如{5,2}确认了dist[2]，后面又来了个{8,2}就不处理了
        if (use[index] == true)
        {continue;}

        //把选出的顶点加入到S集合中
        use[index] = true;

        //更新U集合中的剩余顶点权值信息
        for (int j = 0; j < N; ++j)
        {
            //通过新节点中转到达其他节点的路径如果比不通过要小，那么就将最短路径更改为前者，否则不变
            if (use[j] == false && min + graph[index][j] < dis[j])
            {
                dis[j] = min + graph[index][j];
                //更新U集合中顶点的权值
                que.emplace(dis[j],j); //⭐新的dis[j]比旧的dis[j]要小，相当于朴素算法中用更短路径覆盖原路径，只不过这里的旧dis[j]因为比较大所以永远不会到堆顶
            }
        }
    }
    //测试打印
    for (unsigned int d : dis)
    {
        cout << d << " ";
    }
    cout << endl;
    //最后输出顶点的最短路径
    return dis[start];
}

//迪杰斯特拉算法测试函数
void dijkstra_test(int start, int end , bool choose = true) //最后一个参数默认传入true使用优化算法，否则使用朴素算法

{
    vector<vector<unsigned int>> graph = 
    {
        {0, 6, 3, INF, INF, INF},
        {6, 0, 2, 5, INF, INF},
        {3, 2, 0, 3, 4, INF},
        {INF, 5, 3, 0, 2, 3},
        {INF, INF, 4, 2, 0, 5},
        {INF, INF, INF, 3, 5, 0},
    };
    int distance {};
    if (choose)
    {
        distance = dijkstra(graph, start, end);
    }
    else
    {
        distance = dijkstra_better(graph,start,end);
    }
    if (distance == INF)
    {
        cout << "不存在有效路径！" << endl;
    }
    else
    {
        cout << start << " to " << end  <<" distance:" << distance << endl;
    }
}

