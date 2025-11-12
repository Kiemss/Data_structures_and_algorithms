#pragma once
#include<iostream>
#include<vector>

void subset_impl(int *arr, int index, int length, std::vector<bool>& subset)
{
    if (index == length)
    {
        // 打印当前子集
        for (int j = 0; j < length; ++j)
        {
            if (subset[j])
            {
                std::cout << arr[j] << " ";
            }
        }
        std::cout << std::endl;
        return;
    }
    
    // 选择1：包含当前元素
    subset[index] = true;
    subset_impl(arr, index + 1, length, subset);
    
    // 选择2：不包含当前元素
    subset[index] = false;
    subset_impl(arr, index + 1, length, subset);
}

void generate_subsets(int *arr, int length)
{
    std::vector<bool> subset(length, false);
    subset_impl(arr, 0, length, subset);  // 从索引0开始
}