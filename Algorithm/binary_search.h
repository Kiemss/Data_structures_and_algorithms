#pragma once
#include<iostream>

namespace binary_search_algorithm
{
    //迭代二分搜索
    int binary_search_iterative(int arr[],int size,int val)
    {
        int first = 0;
        int last = size-1;
        //注意循环条件是有等号的，因为搜寻范围是[first,last]，没有等号就会漏掉first=last的情况
        int middle{};
        while(first <= last)
        {
            middle = (first + last)/2;
            if(arr[middle] == val)
            {
                return middle;
            }
            else if(arr[middle] > val)
            {
                last = middle - 1;
            }
            else if(arr[middle] < val)
            {
                first = middle + 1;
            }      
        }
        return -1;
    }

    //递归二分搜索
    int binary_search_recursive(int arr[],int first ,int last,int val)
    {
        int middle = (first + last)/2;
        if(first > last)//这个必须放在最前面
        {
            return -1;
        }
        else if(arr[middle] == val)
        {
            return middle;
        }
        else if(arr[middle] > val)
        {
            return binary_search_recursive(arr,first,middle - 1,val);
        }
        else if(arr[middle] < val)
        {
            return binary_search_recursive(arr,middle + 1,last,val);
        }
    }
}
