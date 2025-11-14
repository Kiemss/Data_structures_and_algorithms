#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<sstream>
#include<stack>

using  namespace std;

//十字链表实现有向图
class MyDirectedGraph
{
public:
    //构造函数
    MyDirectedGraph()
    {
        vertices.emplace_back(new VexNode("0"));
    }
    //析构函数
    ~MyDirectedGraph()
    {}

    //根据配置文件生成十字链表
    void create_graph(string file_name)
    {
        ifstream infile(file_name, ios::in); //创建文件流对象并打开相应文件
        if (!infile.is_open()) //文件打开失败
        {
            cerr << "文件打开失败！" << endl;
            throw runtime_error("file " + file_name + " not found!");
        }

        //先创建节点
        string vex_str;
        while (getline(infile,vex_str) && vex_str != "#") //用#分割定义顶点区和创建边区
        {
            vertices.emplace_back(new VexNode(vex_str));
        }

        //然后再根据节点进行插入
        bool is_vertex_line = true;//true时处理顶点，false时处理边
        string head {}; //用来暂时记录该边的头
        string line {}; //用来读取数据到该变量中
        size_t index = 1; //记录head在vertices的第几位，不用查找
        //使用infile的全局函数getline读取每一行到line中，如果到达末尾或者读取失败返回false
        while (getline(infile, line)) 
        {
            if (is_vertex_line) //处理顶点
            {
                //如果不匹配，说明配置文件错误，抛异常
                if (line != vertices[index]->data_)
                {
                    cerr << "line = " << line << endl <<
                    "index = " << index << endl
                    << " vertices[index]->data = " << vertices[index]->data_ << endl;
                    throw runtime_error("data error!");
                }
                if (index >= vertices.size()) 
                {
                    cerr << "index = " << index << " vertices.size() = " << vertices.size() << endl;
                    throw runtime_error("index out of edge!");
                }
            }
            else //处理边
            {
                //特殊情况：该顶点没有出边
                if (line == "0") 
                {
                    is_vertex_line = !is_vertex_line; //不要忘记反转flag
                    ++index; //⭐
                    continue;
                } 
                stringstream ss(line);//将line转换成流对象
                string target_index_str; //该边的末尾顶点的str格式
                while (getline(ss, target_index_str, ','))//取每一个数字
                {
                    int target_index = stoi(target_index_str);
                    //代码安全性：夹杂着0也要退出
                    if (target_index_str == "0") 
                    {
                        ++index;//⭐
                        break;
                    }
                    //检查目标顶点索引是否有效
                    if (target_index < 1 || target_index >= vertices.size())
                    {
                        cerr << "错误：顶点索引" << target_index_str << "无效" << endl
                        << "vertices.size() = " << vertices.size() << endl;
                        throw runtime_error("index is not useful!");
                    }
                    //获取边的终点（目标顶点）
                    VexNode *head_vex = vertices[target_index]; 
                    //获取边的起点（当前顶点）
                    VexNode *cur_vex = vertices[index]; 
                    //创建新的边节点
                    ArcNode *new_node = new ArcNode(cur_vex->data_, head_vex->data_);

                    //⭐使用❗头插法❗
                    new_node->tail_link_ = cur_vex->first_out_;
                    cur_vex->first_out_ = new_node;

                    new_node->head_link_ = head_vex->first_in_;
                    head_vex->first_in_ = new_node;
                }
                ++index;
            }
            is_vertex_line = !is_vertex_line; //最后反转flag
        }
        infile.close();//记得关闭文件
    }

    //展示内容
    void show()
    {
        for (size_t i = 1; i < vertices.size(); i++) // 从1开始，跳过vertices[0]
        {
            VexNode* vex = vertices[i];
            cout << vex->data_ << " -> ";
            
            ArcNode* cur = vex->first_out_;
            while (cur != nullptr)
            {
                cout << cur->head_vex_ << " "; //应该显示边的终点
                cur = cur->tail_link_; //移动到下一条出边
            }
            cout << endl;
        }
    }

    //深度优先遍历递归外部函数⭐该函数无法处理单独的顶点！
    void recursive_depth_first_search()
    {
        //先重置所有节点
        remark();
        int have_search = 0;
        int index = 1;
        while (have_search < vertices.size() - 1 && index < vertices.size())
        {
            recursive_DFS_impl(vertices[index],have_search);
            ++index;
        }
        cout << endl;
    }

    /*糟糕的迭代dfs代码
    void iterative_depth_first_search()
    {
        remark();
        int have_search = 0;
        int index = 1; //从第一个开始找，已经验证过是true的不用再从1开始遍历
        std::stack<VexNode*> st;
        VexNode *vex = nullptr;
        while (index < vertices.size() && have_search < vertices.size() - 1)
        {
            //从栈中提取vex，否则找第一个未被遍历的vex
            if (!st.empty())//不为空：提出来
            {
                vex = st.top();
                st.pop();
            }
            else
            {
                while (vertices[index]->mark_){++index;} //跳过已经被标记的
                vex = vertices[index];//将第一个未被标记的赋值给vex
                //这里不++index是因为如果以后不用再遍历找，可以节省一行代码b(￣▽￣)d
            }
            
            if (vex->mark_ == false) //如果未被遍历，就输出并标记
            {
                vex->mark_ = true; //标记
                cout << vex->data_ << " ";
                st.push(vex); //入栈
            }
            
            ArcNode *cur = vex->first_out_;
            if (cur == nullptr) {continue;} //特殊情况：该节点没有任何出边
            
            //找未被遍历的下一个节点
            while (cur != nullptr 
                && vertices[this->find(cur->head_vex_)]->mark_ == true)  //已被遍历：下一个
            {
                cur = cur->tail_link_;
            } 
            if (cur != nullptr)
            {
            st.push(vertices[this->find(cur->head_vex_)]);
            }
        }
    }
    //⭐现在不会报错，可是非常低效
    //深度优先遍历迭代函数
    void iterative_depth_first_search()
    {
        remark();
        std::stack<VexNode*> st;
        st.push(vertices[1]);
        VexNode *vex = nullptr;
        while (!st.empty())
        {
            vex = st.top();
            st.pop();
            if (vex->mark_ == false) //如果未被遍历，就输出并标记
            {
                vex->mark_ = true; //标记
                cout << vex->data_ << " ";
                st.push(vex); //入栈
            }
            
            ArcNode *cur = vex->first_out_;
            if (cur == nullptr) {continue;} //特殊情况：该节点没有任何出边
            
            //找未被遍历的下一个节点
            while (cur != nullptr 
                && vertices[this->find(cur->head_vex_)]->mark_ == true)  //已被遍历：下一个
            {
                cur = cur->tail_link_;
            } 
            if (cur != nullptr)
            {
            st.push(vertices[this->find(cur->head_vex_)]);
            }
        }
    }
    */

    //深度优先遍历迭代函数
    void iterative_depth_first_search()
    {
        remark();
        std::stack<VexNode*> st;
        VexNode *vex = nullptr;
        for (int i = 1; i < vertices.size(); ++i) //遍历所有顶点，防止存在单独顶点
        {
            VexNode *cur_vex = vertices[i];
            if (cur_vex->mark_ ==false)//只将未被遍历的顶点入栈处理
            {
                st.push(cur_vex);
                while (!st.empty())
                {
                    vex = st.top();
                    st.pop();
                    if (vex->mark_ == false) //如果未被遍历，就输出并标记
                    {
                        vex->mark_ = true; //标记
                        cout << vex->data_ << " ";

                        ArcNode *cur = vex->first_out_;
                        
                        //找未被遍历的下一个节点
                        while (cur != nullptr)
                        {
                            VexNode *temp_vex = vertices[this->find(cur->head_vex_)];
                            if (temp_vex->mark_ == false)
                            {
                                st.push(temp_vex);
                            }
                            cur = cur->tail_link_;
                        } 
                    }
                }
            }
        }
        cout << endl;
    }

private:
    //边节点
    struct ArcNode
    {
        ArcNode(string tvex, string hvex, ArcNode *tail_link = nullptr, ArcNode *head_link = nullptr) 
        : tail_vex_(tvex)
        , head_vex_(hvex)
        , tail_link_(tail_link)
        , head_link_(head_link)
        {}
        string tail_vex_; //边的起点
        string head_vex_; //边的终点
        ArcNode *tail_link_; //同一个起点的下一条出边
        ArcNode *head_link_; //同一个终点的下一条入边
        
    };

    //顶点节点
    struct VexNode
    {
        VexNode(string data)
        : data_(data)
        , first_in_(nullptr)
        , first_out_(nullptr)
        , mark_(false)
        {}
        string data_;
        ArcNode *first_in_; //入边的首指针
        ArcNode *first_out_; //出边的首指针   
        bool mark_; //标记，方便进行遍历 
    };

    vector<VexNode*> vertices; //顶点数组

    //深度优先遍历递归算法
    void recursive_DFS_impl(VexNode *vex, int &have_search)
    {
        
        if (vex->mark_ == true)
        {
            return;//如果已经被遍历过，直接返回
        }

        vex->mark_ = true; //遍历：标记该节点，防止被多次遍历
        ++have_search;//同时说明该节点已被遍历
        cout << vex->data_ << " ";

        ArcNode *cur = vex->first_out_; //当前被遍历节点
        while (cur != nullptr)
        {
            recursive_DFS_impl(vertices[this->find(cur->head_vex_)], have_search); //深度优先
            cur = cur->tail_link_;//下一个边
        }
        return;
    }

    //重置节点状态函数
    void remark()
    {
        for (VexNode* vex : vertices)
        {
            vex->mark_ = false;
        }
        return;
    }

    //find函数，返回下标
    size_t find(string &str) const
    {

        for(int i = 1; i < vertices.size(); ++i)
        {
            if (vertices[i]->data_ == str)
            {
                return i;
            }
        }
        return 0;
    }
};


