//函数均由AI生成，test()调试函数可以运行
#pragma once
#include<iostream>
#include<string>
#include<list>
#include<unordered_map>
#include<vector>
#include<algorithm>
#include<fstream>
#include<sstream>
#include<cctype>
#include<windows.h>
#undef UNICODE //使用单字节字符串（ASCII)，使用unicode会报错

using std::list;
using std::string;
using std::cout;
using std::endl;
using std::unordered_map;
using std::cerr;
using std::vector;
using std::ifstream;
using std::istringstream;
using std::transform;

namespace data_structures::inverted_index
{

    //倒排项
    struct InvertTerm //标准术语是Posting
    {
        InvertTerm(string docid, int freqs, int location)
            : docid_(docid), freqs_(freqs)
        {
            locations_.push_back(location);
        }
        
        // 添加位置信息
        void add_location(int location) {
            locations_.push_back(location);
            freqs_++;
        }
        
        string docid_;  //单词所在的文档
        int freqs_; //单词出现的次数
        list<int> locations_;   //单词出现的位置
    };

    //倒排列表
    class InvertList //标准术语是Posting List
    {
    public:
        //添加倒排项 - 修复原有bug
        void add_term(const string& docid, int location)
        {
            // 查找是否已存在该文档的倒排项
            for (auto& term : m_term_list) {
                if (term.docid_ == docid) {
                    // docid已经存在，增加频率并添加位置
                    term.add_location(location);
                    return;
                }
            }
            
            // 第一次创建该docid的倒排项
            m_term_list.emplace_back(docid, 1, location);
        }
    
        //获取倒排列表内容
        const list<InvertTerm>& get_invert_list() const {
            return m_term_list;
        }
        
        // 获取文档频率（包含该词的文档数量）
        size_t get_document_frequency() const {
            return m_term_list.size();
        }
        
        // 获取总词频
        int get_total_frequency() const {
            int total = 0;
            for (const auto& term : m_term_list) {
                total += term.freqs_;
            }
            return total;
        }
        
    private:
        list<InvertTerm> m_term_list; //存储当前倒排列表的所有倒排项
    };

    //倒排索引
    class InvertIndex
    {
    public:
        //搜索路径下所有文件
        void set_search_path(const string& path) {
            get_all_file(path);
        }

        // 构建倒排索引
        void build_index() {
            cout << "开始构建倒排索引..." << endl;
            
            for (const auto& file_path : m_file_list) {
                process_file(file_path);
            }
            
            cout << "倒排索引构建完成！总共索引 " << m_invert_map.size() << " 个词项" << endl;
        }
        
        // 查询单词
        vector<string> query(const string& word) {
            vector<string> results;
            string cleaned_word = clean_token(word);
            
            auto it = m_invert_map.find(cleaned_word);
            if (it != m_invert_map.end()) {
                const auto& invert_list = it->second.get_invert_list();
                for (const auto& term : invert_list) {
                    results.push_back(term.docid_);
                }
            }
            
            return results;
        }
        
        // 详细查询 - 返回包含频率和位置信息
        void query_detail(const string& word) {
            string cleaned_word = clean_token(word);
            
            auto it = m_invert_map.find(cleaned_word);
            if (it == m_invert_map.end()) {
                cout << "未找到词项: " << word << endl;
                return;
            }
            
            const auto& invert_list = it->second.get_invert_list();
            cout << "词项: " << cleaned_word << endl;
            cout << "文档频率: " << it->second.get_document_frequency() << endl;
            cout << "总词频: " << it->second.get_total_frequency() << endl;
            cout << "出现在以下文档中:" << endl;
            
            for (const auto& term : invert_list) {
                cout << "  文档: " << term.docid_ << " (频率: " << term.freqs_ << ")" << endl;
                cout << "  位置: ";
                for (int pos : term.locations_) {
                    cout << pos << " ";
                }
                cout << endl;
            }
        }
        
        // 获取所有词项
        vector<string> get_all_terms() const {
            vector<string> terms;
            for (const auto& pair : m_invert_map) {
                terms.push_back(pair.first);
            }
            return terms;
        }
        
        // 获取索引统计信息
        void print_statistics() const {
            cout << "=== 倒排索引统计 ===" << endl;
            cout << "文档数量: " << m_file_list.size() << endl;
            cout << "词项数量: " << m_invert_map.size() << endl;
            
            size_t total_postings = 0;
            for (const auto& pair : m_invert_map) {
                total_postings += pair.second.get_document_frequency();
            }
            cout << "总倒排项数量: " << total_postings << endl;
        }

        // 获取文件列表（供外部访问）
        const list<string>& get_file_list() const {
            return m_file_list;
        }

        // 获取文件数量
        size_t get_file_count() const {
            return m_file_list.size();
        }
        
        // 清空索引
        void clear() {
            m_invert_map.clear();
            m_file_list.clear();
        }

    private:
        //倒排索引 = 词典 + 倒排列表
        unordered_map<string, InvertList> m_invert_map;
        list<string> m_file_list; //存储所有需要建立倒排的文件

        // 递归获取所有文件（Windows版本），只搜索指定类型文件
        void get_all_file(const string& path) {
            string search_path = path + "\\*.*";
            WIN32_FIND_DATAA file_data;
            HANDLE hFind = FindFirstFileA(search_path.c_str(), &file_data);
            
            if (hFind == INVALID_HANDLE_VALUE) {
                return;
            }
            
            do {
                string filename = file_data.cFileName;
                
                // 跳过 "." 和 ".."
                if (filename == "." || filename == "..") {
                    continue;
                }
                
                string full_path = path + "\\" + filename;
                
                if (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                    // 如果是目录，递归遍历
                    get_all_file(full_path);
                } else {
                    // 检查文件扩展名
                    if (is_target_file(filename)) {
                        m_file_list.push_back(full_path);
                    }
                }
            } while (FindNextFileA(hFind, &file_data));
            
            FindClose(hFind);
        }

        // 检查是否为目标文件类型
        bool is_target_file(const string& filename) {
            size_t dot_pos = filename.find_last_of('.');
            if (dot_pos == string::npos) {
                return false;
            }
            
            string extension = filename.substr(dot_pos);
            transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
            
            return extension == ".cpp" || extension == ".c" || 
                   extension == ".h"   || extension == ".hpp" || 
                   extension == ".md"  || extension == ".txt";
        }
        
        // 处理单个文件
        void process_file(const string& file_path) {
            ifstream file(file_path);
            if (!file.is_open()) {
                cerr << "无法打开文件: " << file_path << endl;
                return;
            }
            
            string line;
            int line_number = 0;
            
            while (getline(file, line)) {
                line_number++;
                process_line(line, file_path, line_number);
            }
            
            file.close();
        }
        
        // 处理单行文本
        void process_line(const string& line, const string& file_path, int line_number) {
            istringstream iss(line);
            string token;
            int word_position = 0;
            
            while (iss >> token) {
                word_position++;
                string cleaned_token = clean_token(token);  // 修改变量名
                
                if (!cleaned_token.empty()) {
                    // 添加到倒排索引 - 使用修改后的变量名
                    m_invert_map[cleaned_token].add_term(file_path, line_number * 100 + word_position);
                }
            }
        }
        
        // 清理token：转换为小写，移除标点
        string clean_token(const string& token) {
            string result;
            for (char c : token) {
                if (isalnum(c)) {
                    result += tolower(c);
                }
            }
            return result;
        }
    };

    //测试函数  
    void test() {
        InvertIndex index;
        index.set_search_path("F:\\Codes\\CppCode\\data_structures_and_algorithms");
        
        cout << "找到 " << index.get_file_count() << " 个文件" << endl;
        
        // 构建索引
        index.build_index();
        
        // 显示统计信息
        index.print_statistics();
        
        // 测试查询
        cout << "\n=== 测试查询 ===" << endl;
        index.query_detail("invert");
        index.query_detail("index");
        
        // 获取所有词项示例
        auto terms = index.get_all_terms();
        cout << "\n前10个词项: ";
        for (size_t i = 0; i < std::min(terms.size(), size_t(10)); i++) {
            cout << terms[i] << " ";
        }
        cout << endl;
    }

}