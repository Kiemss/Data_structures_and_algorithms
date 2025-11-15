#pragma once
#include<iostream>
#include<algorithm>
#include<vector>
#include<sstream>
#include<fstream>
#include<string>

using std::cout;
using std::endl;
using std::cin;
using std::vector;
using std::string;
using std::fstream;
constexpr int SIZE = 1000;

namespace data_structures::union_find
{
    class UnionFind
    {
    public:
        //构造函数
        UnionFind(int size = SIZE)
        {
            parent.resize(size);
            rank.resize(size);
            fill(rank.begin(), rank.end(), 1); //初始化权值数组
            for (int i = 0;i < size; ++i)   //初始化父节点数组
            {
                parent[i] = i;
            }
        }

        //初始化边数组
        void initialize(const string& file_name)
        {
            fstream fs(file_name);
            if (!fs.is_open())
            {
                throw std::runtime_error("fail not found!");
            }
            int max_node = 0; //最大节点数决定了parent数

            int start, end, cost {};
            while (fs >> start >> end >> cost) //读取👍
            {
                edges.emplace_back(start, end, cost);
                max_node = std::max({max_node, start, end});//注意用{}
            }
            
            //根据最大节点编号调整并查集大小
            int node_count = max_node + 1;
            parent.resize(node_count);
            rank.resize(node_count);


            return;
        }

        //kruskal算法解决最小生成树问题（贪心算法）
        void kruskal()
        {
            //按照权值进行从小到大的排序
            std::sort(edges.begin(),edges.end(),[](Edge &edge1, Edge &edge2)
            {return edge1.cost_ < edge2.cost_;}); 

            //重置并查集
            for (int i = 0; i < parent.size(); ++i)
            {
                parent[i] = i;
                rank[i] = 1;
            }

            vector<Edge> mst_edges; //⭐存储最小生成树的边
            int edge_count {}; //已选边数
            int total_cost {}; //总成本

            //遍历所有边（经过排序后是从小到大排序）
            for (auto &edge : this->edges)
            {
                if (edge_count >= parent.size() - 1) {break;}//⭐如果已经选了n-1条边，就结束

                int s_root = find(edge.start_);
                int e_root = find(edge.end_);
                if (s_root == e_root) //根相同：不用合并
                {
                    continue;
                }
                else //否则合并边顶点（⭐注意不是上面的根）
                {
                    merge(edge.start_, edge.end_); //合并顶点
                    mst_edges.push_back(edge); //将被选边推入vec
                    ++edge_count;
                    total_cost += edge.cost_; //统计总成本
                }
            }

            //最后打印输出被选择的边和总成本
            cout << "total cost:" <<  total_cost << endl;
            for (auto choose : mst_edges)
            {
                cout << choose.start_ << "->" << choose.end_ << " cost:"<<  choose.cost_ << endl;
            }
        }



        //寻找根节点迭代函数
        int find(int num)
        {
            int root = num;
            while (root != this->parent[root])
            {
                root = this->parent[root];
            }

            //路径压缩优化（将所有节点的根节点指向当前根节点）
            while (num != root)
            {
                int next = this->parent[num];
                this->parent[num] = root;
                num = next;
            }
            return root;
        }

        //合并函数
        void merge(int x, int y)
        {
            x = find(x);
            y = find(y);
            //用rank进行加权优化
            if (x != y)
            {
                if (rank[x] > rank[y]) 
                {parent[y] = x;}
                else if (rank[x] < rank[y]) 
                {parent[x] = y;} //层高相同时，将x的父节点设置为y
                else 
                {
                    parent[y] = x;
                    ++rank[x];
                } //那么y的层高就会提高
            }
        }
    private:
        struct Edge//带权边（尽管是无向图，用起点终点表示是为了方便）
        {   
            Edge(int start, int end, int cost)
            : start_(start)
            , end_(end)
            , cost_(cost)
            {}
            int start_; //起点
            int end_; //终点
            int cost_; //权值
        };

        vector<int> parent; //记录每个节点的父节点
        vector<int> rank; //记录每个节点的层高
        vector<Edge> edges; //记录所有边节点
    };

    //并查集测试函数
    void union_find_test1()
    {
        UnionFind uf(12);
        int x,y;
        for (int i = 0; i < 6; ++i)
        {
            cout << "请输入0-12以内的数字" << endl;
            cin >> x >> y;
            while (x > 11 || x < 0 || y > 11 || y < 0)
            {
                cin >> x >> y;
            }
            uf.merge(x,y);
        }
        cout << (uf.find(8) == uf.find(2) ? "yes" : "no") << endl;
        cout << (uf.find(4) == uf.find(2) ? "yes" : "no") << endl;
    }

    //kruskal算法测试函数
    void kruskal_test()
    {
        UnionFind uf;
        uf.initialize("graph_data.txt");
        uf.kruskal();
    }
}