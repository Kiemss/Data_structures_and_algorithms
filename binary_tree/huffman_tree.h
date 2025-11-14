#pragma once
#include<iostream>
#include<string>
#include<unordered_map>
#include<vector>
#include<functional>
#include<queue>

using std::vector;
using std::unordered_map;
using std::string;
using std::cout;
using std::endl;

namespace data_structures::huffman_tree
{
    class HuffmanTree
    {
    public:
        HuffmanTree() //对于小根堆对象，重载其比较函数，用weight而不是默认的指针大小进行比较
        : m_min_heap([](Node* n1, Node* n2)->bool
            {return n1->weight_ > n2->weight_;})
        , m_pRoot(nullptr)
        {}


        //创建哈夫曼树
        void create_huffman_tree(string str)
        {
            //先统计字符的权值
            unordered_map<char, size_t> data_map;
            for (char ch : str)
            {
                ++data_map[ch];
            }

            //生成节点，放入小根堆中
            for (auto &pair : data_map)
            {
                m_min_heap.push(new Node(pair.first, pair.second));//放入字符和权值
            }

            while (m_min_heap.size() > 1)
            {
                //取出最小的两个节点生成树
                Node *n1 = m_min_heap.top();
                m_min_heap.pop();
                Node *n2 = m_min_heap.top();
                m_min_heap.pop();

                //生成父节点
                Node *father = new Node('\0', n1->weight_ + n2->weight_);
                father->left_ = n1;
                father->right_ = n2;

                //将新节点放回堆中比较
                m_min_heap.push(father);
            }

            //最后堆中只剩一个节点或者没有节点，作为根
            this->m_pRoot = m_min_heap.top();
            m_min_heap.pop();
        }
    
        //编码函数
        string encode(string str)
        {
            get_huffman_code();//调用获取哈夫曼编码函数将其装进codemap中才能用
            string encode_str;
            for (char ch : str)
            {
                encode_str.append(m_code_map[ch]);
            }
            return encode_str;
        }

        //自己写的解码函数，笨笨的
        string my_decode(string encode)
        {
            string decode_str;
            size_t index = 0;
            while (index < encode.size())
            {
                Node *cur = this->m_pRoot;
                while (cur->left_ != nullptr && cur->right_ != nullptr)
                {
                    if (encode[index] == '1') //编码是1往右走
                    {
                        cur = cur->right_;
                    }
                    else
                    {
                        cur = cur->left_;
                    }
                    ++index;
                }
                //跳出循环说明到达叶子节点,输出对应字母
                decode_str += cur->data_;
            }
            return decode_str;
        }

        //解码函数
        string decode(string encode)
        {
            string decode_str;
            Node *cur = this->m_pRoot;
            for (char ch : encode) //不用额外设置index
            {
                if (ch == '0') //注意是'0'而不是0
                {
                    cur = cur->left_;
                }

                if (ch == '1')  //⭐不用else，因为向左后又可能又向右，减少循环次数
                {
                    cur = cur->right_;
                }

                if (cur->left_ == nullptr && cur->right_ == nullptr) //if判断遍历到叶子节点
                {
                    decode_str += cur->data_;
                    //内部重置。总之不用嵌套循环
                    cur = this->m_pRoot;
                }
            }
            return decode_str;
        }

        //获取编码规则
        void get_rule()
        {
            for (auto &pair : m_code_map)
            {
                cout << pair.first << ":" << pair.second << endl;
            }
            cout << "===============" << endl;
        }

    private:
        struct Node
        {
            Node(char data, size_t weight)
            : data_(data)
            , weight_(weight)
            , left_(nullptr)
            , right_(nullptr)
            {}

            char data_; //字符数据
            size_t weight_; //节点权值
            Node *left_; //左子节点
            Node *right_; //右子节点
        };

        Node *m_pRoot; //指向根节点的指针
        unordered_map<char, string> m_code_map; //存储字符对应的哈夫曼编码
        using min_heap = std::priority_queue<Node*, vector<Node*>, std::function<bool(Node*, Node*)>>;//vector中装指针，方便手动管理；要求传入一个该类型的函数对象；
        min_heap m_min_heap;


        //获取哈夫曼编码
        void get_huffman_code()
        {
            string code;
            //这里其实是将已经生成好的哈夫曼树输出
            get_huffman_code_impl(this->m_pRoot,code); 
        }

        //获取哈夫曼编码递归函数
        void get_huffman_code_impl(Node* node, string code) //传入当前节点和前段的编码
        {
            if (node->left_ == nullptr && node->right_ == nullptr)
            {
                m_code_map[node->data_] = code; //将哈夫曼编码装进成员变量codemap中
                return;
            }
            
            get_huffman_code_impl(node->left_, code + "0");
            get_huffman_code_impl(node->right_, code + "1");
        }

        
    };
}