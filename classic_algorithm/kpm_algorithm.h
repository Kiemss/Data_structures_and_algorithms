#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<memory>

int* get_next(std::string str);
//时间复杂度O(m + n) 空间复杂度O(m)
int kmp(std::string text,std::string pattern) //text是长串，pattern是子串
{   
    int i = 0;
    int j = 0;

    //计算一个子串对应的next数组
    int* next = get_next(pattern);
    while (i < static_cast<int>(text.size()) && j < static_cast<int>(pattern.size()) )
    {
        if (j == -1)
        {
            i++;
            j++;
        }
        else if ((text[i] == pattern[j]))
        {
            i++;
            j++;
        }
        else 
        {
            //kmp的核心是不回退i，只回退j值
            j = next[j];    //如果首字母匹配失败，j = -1表示要++i
        }
    }
    delete next;
    if (j == pattern.size())
    {
        return i - j;
    }
    else 
    {
        return -1;
    }
}

//kpm算法求解子串的next数组
int* get_next(std::string str)
{
    int *next = new int[str.size()];
    int j = 0; //⭐用于遍历子串，表示当前用来比较的元素下标而不是要填入next的下标
    int k = -1; //⭐设一个变量表示公共前后缀的长度，同时也表示了用于对比的str下标
    next[j] = k;
    while (j < str.size() - 1)//j = str.size() - 1时就初始化好了next数组中最后一个元素
    {
        if (k == -1 || str[k] == str[j])//⭐不能漏掉k == -1并且要放在前面
        {
            ++k;//先自增，逻辑清晰
            ++j;
            if (str[k] == str[j])//kmp算法优化
            {
                next[j] = next[k];
            }
            else
            {
                next[j] = k; //不使用 next[j] = next[j - 1] + 1;更优雅
            }
        }
        else 
        {
            k = next[k]; //⭐关键
        }
    }
    return next;
}

