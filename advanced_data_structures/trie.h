#pragma once
#include<iostream>
#include<string>
#include<map>
#include<vector>
#include<queue>

namespace data_structures::trie
{
    //实现trie字典树
    class Trie
    {
    public:
        //构造函数
        Trie()
        {
            this->m_pRoot = new TrieNode('\0', 0);
        }

        //析构函数->对于树状结构，应该用队列进行层序遍历的删除方式
        ~Trie()
        {
            TrieNode *cur {};
            std::queue<TrieNode*> que;
            que.push(this->m_pRoot);
            while (!que.empty()) //不为空
            {
                cur = que.front();
                que.pop();
                //把当前节点的孩子节点全部入队
                for (auto pair : cur->node_map_)
                {
                    que.push(pair.second);
                }
                delete cur;
            }
        }

        //添加单词
        void add(const std::string &word)
        {
            TrieNode *cur = this->m_pRoot;
            for (int i = 0; i < word.size(); ++i)
            {
                if (cur->node_map_.find(word[i]) == cur->node_map_.end())
                {
                    //相应字符的节点没有，创建它
                    TrieNode *child = new TrieNode(word[i], 0);
                    //不要额外添加循环命令式创建新节点，而是使用外层循环自然创建
                    cur->node_map_.emplace(word[i], child);
                    cur = child;
                }
                else
                {
                    cur = cur->node_map_.find(word[i])->second;
                }
            }
            //跳出循环时，cur指向单词的最后一个节点，要++freqs_
            ++cur->freqs_;

        }
    
        //查询单词
        int query(const std::string &word)
        {
            TrieNode *cur = this->m_pRoot;
            for (int i = 0;i < word.size(); ++i)
            {
                if (cur->node_map_.find(word[i]) == cur->node_map_.end())
                {
                    return 0;//未找到：结束
                }
                else
                {
                    //找到：找下一个
                    cur = cur->node_map_.find(word[i])->second;                   
                }
            }
            return cur->freqs_;
        }

        //前序遍历字典树
        void pre_order()
        {
            std::vector<std::string> wordlist;
            pre_order_impl(this->m_pRoot, std::string(), wordlist);
            for (auto word : wordlist)
            {
                std::cout << word << std::endl;
            }
        }
        
        //字符串的前缀查询
        std::vector<std::string> prefix_query(std::string &prefix)//传入变量
        {  
            TrieNode *cur = this->m_pRoot;
            for (int i = 0; i < prefix.size(); ++i)
            {
                if (cur->node_map_.find(prefix[i]) == cur->node_map_.end())
                {
                    return {};//未找到：返回空字符串
                }
                else
                {
                    cur = cur->node_map_.find(prefix[i])->second;//找到：移动cur令其指向下一个节点
                }
            }

            //现在cur指向prefix中最后一个节点
            std::vector<std::string> wordlist;
            pre_order_impl(cur, prefix.substr(0, prefix.size() - 1), wordlist);
            
            return wordlist;
        }
        //字符串的前缀查询，传入右值重载
        std::vector<std::string> prefix_query(std::string &&prefix)//传入字面量
        {
            return prefix_query(prefix);
        }
        
        //删除单词
        void remove(const std::string &word)
        {
            TrieNode *cur = this->m_pRoot;
            TrieNode *del = this->m_pRoot;
            char delch = word[0];

            for (int i = 0; i < word.size(); ++i)
            {
                auto child_it = cur->node_map_.find(word[i]);
                if (child_it == cur->node_map_.end())
                {   
                    //未找到，直接返回
                    return;
                }

                //单词word内包含了另一个独立的单词，或者多个单词共享前缀，改变开始删除的位置
                if (cur->freqs_ > 0  || cur->node_map_.size() > 1) 
                { 
                    del = cur;
                    delch = word[i]; //⭐delch并不是del的ch_，而是del的子节点的！（从上面用于判断的chiild_it可以看出来！！！
                }
                //cur移动到子节点
                cur = child_it->second;
            }

            //遍历完毕，说明word单词存在
            if (cur->node_map_.empty()) //后面没有节点
            {
                //直接删除
                TrieNode *child = del->node_map_[delch];
                del->node_map_.erase(delch);

                //释放相应的节点内存
                std::queue<TrieNode*> que;
                que.push(child);
                while (!que.empty()) //不为空
                {
                    TrieNode* front = que.front();
                    que.pop();
                    //把当前节点的孩子节点全部入队
                    for (auto pair : front->node_map_)
                    {
                        que.push(pair.second);
                    }
                    delete front;
                }
            }
            else //该单词是另一个单词的前缀，改变frequency即可
            {
                cur->freqs_ = 0;//不是--，删除就是删除所有了
            }
        }

       /* void my_remove(const std::string &word)
        {
            TrieNode *head = this->m_pRoot;//最后一个存在单词的节点
            TrieNode *tail = this->m_pRoot;//用于遍历判断是否有该单词
            int index = -1;//记录head所在下标
            for (int i = 0; i < word.size(); ++i)
            {
                auto child_it = tail->node_map_.find(word[i]);
                if (child_it == tail->node_map_.end())//未找到
                {
                    return;
                }
                else//找到了：下一个
                {
                    tail = child_it->second;
                    //分支的情况容易漏掉
                    if (tail->freqs_ > 0 || tail->node_map_.size() > 1)//有单词以当前节点的下一个节点结尾,也就是避免了判断根节点
                    {
                        head = tail;
                        index = i;
                    }
                }
            }
            //如果后面还有字母，那么直接改变该节点的frequency就行了
            if (!tail->node_map_.empty())
            {
                --tail->freqs_;
            }
            else
            {
                //最后tail到达了最后一个位置说明存在该单词，可以删除了
                TrieNode *next {};

                head = head->node_map_.find(word[index + 1])->second;

                for (int i = index + 2; i < word.size(); ++i)
                {
                    //先加再删，方便计算
                    next = head->node_map_.find(word[i])->second;
                    delete head;
                    head = next;
                }
                delete head;
            }
        }
        */
    private:
        //节点类型定义
        struct TrieNode
        {   
            TrieNode(char ch, int freqs)
            : ch_(ch)
            , freqs_(freqs)
            {}
            //节点存储的字符数据
            char ch_; 
            /*
            frequency的简写，表示以当前节点结尾的单词的频率（数量），也就是说trie允许存储多个相同单词
            注意每个单词在trie中的路径都是独一无二的，因此可以统计词频，不要认为 hell和 lol会在相同节点结尾！
            */
            int freqs_; 
            //存储孩子节点字符数据和节点指针的对应关系
            std::map<char, TrieNode*> node_map_; 
        }; 
        TrieNode *m_pRoot; //指向树的根节点


        //前序遍历字典树
        void pre_order_impl(TrieNode *node, std::string word, std::vector<std::string> &vec)
        {
            if (node != this->m_pRoot)
            {
                word.push_back(node->ch_);
                if (node->freqs_ > 0)
                {
                    vec.emplace_back(word);
                }
            }
            for (auto pair : node->node_map_)
            {
                //没有当节点为空就返回：因为遍历子节点时如果是空的就不会遍历到了
                pre_order_impl(pair.second, word, vec);
            }
        }
    };
}