#pragma once
#include<iostream>
#include<vector>
#include<string>

//时间复杂度O(n * m) 空间复杂度O(1)
int bf(std::string big,std::string small)
{
    if (big.size() <= 0 || small.size() <= 0 || big.size() < small.size())
    {return -1;}
    int i = 0;
    int j = 0;
    while (i < big.size())
    {
        if (big[i] == small[j])
        {
            ++i;
            ++j;
            if (j >= small.size())//加完之后判断是否完全匹配
            {
                return (i - j);
            }
        }
        else if (big[i] != small[j])
        {
            i = i - j + 1; //⭐i要从刚才开始匹配的位置的下一位开始，而不是当前位置的下一位
            j = 0;
        }
    }

    //找不到匹配
    return -1;
}