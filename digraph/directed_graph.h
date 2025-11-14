#pragma once
#include<iostream>
#include<list>
#include<vector>
#include<string>
#include<string.h>

namespace data_structures::directed_graph
{
    //用邻接表实现有向图
    class DirectedDigraph    
    {
    public:
        //从配置文件读入顶点和边的信息，生成邻接表 分隔符为#
        void read_file(std::string file_path)
        {
            FILE *pf = fopen(file_path.c_str(), "r");//只读模式
            if (pf == nullptr)
            {
                throw file_path + "no exists!";
            }

            //提前占用vector的第零号位置，让编号和下标一一对应
            vertics.emplace_back(""); 

            while (!feof(pf))
            {
                char line[1024] = { 0 };
                fgets(line, 1024, pf);
                //增加一个节点信息
                vertics.emplace_back(line);

                char *vertic_num = strtok(line, ",");
                while (vertic_num != nullptr)
                {
                    vertics.back().adj_list_.emplace_back(atoi(vertic_num));
                    vertic_num = strtok(nullptr, ",");
                }
            }

            fclose(pf);
        }

        //输出邻接表信息
        void show() const
        {
            for  (int i = 1;i <vertics.size();++i)
            {
                std::cout << vertics[i].data_ << ":";
                for (auto num : vertics[i].adj_list_)
                {
                    std::cout << num << " ";
                }

            }
            std::cout << std::endl;
        }

        //深度优先遍历
        void depth_first_search()
        {}

        //广度优先遍历
    private:
        struct Vertic
        {
            Vertic(std::string data)
            : data_(data)
            {}
            std::string data_; //存储顶点的信息
            std::list<int> adj_list_; //邻接链表结构   
        };
    
        std::vector<Vertic> vertics; //邻接表结构
    };
}