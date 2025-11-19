/*
 * [4] 寻找两个正序数组的中位数 ❌
 * 
 * 题目描述：给定两个大小分别为 m 和 n 的正序（从小到大）数组 nums1 和 nums2。
 * 请你找出并返回这两个正序数组的中位数。
 * 算法的时间复杂度应该为 O(log (m+n))。
 * 
 * 解题思路：使用二分查找在较短的数组中寻找分割点，使得：
 * - 左半部分所有元素 <= 右半部分所有元素
 * - 左半部分元素个数 = (m+n+1)/2
 * 然后根据总元素个数的奇偶性计算中位数。
 * 
 * 分割示意图：
 * 数组A: [a0, a1, ..., a(i-1)] | [ai, a(i+1), ..., a(m-1)]
 * 数组B: [b0, b1, ..., b(j-1)] | [bj, b(j+1), ..., b(n-1)]
 * 左半部分: A[0..i-1] + B[0..j-1]  → 共half个元素
 * 右半部分: A[i..m-1] + B[j..n-1]  → 共剩余元素
 */
#pragma once
#include<iostream>
#include<vector>
using namespace std;

class Solution 
{
public:
    double findMedianSortedArrays(vector<int>& A, vector<int>& B) 
    {
        int m = A.size(), n = B.size();
        // 保证 A 是较短的数组，减少二分查找范围
        if (m > n) return findMedianSortedArrays(B, A); 

        int imin = 0, imax = m;
        // half: 左半部分应该包含的元素个数
        // 奇数情况：half包含中位数；偶数情况：half是左半部分元素数
        int half = (m + n + 1) / 2; 
        
        while (imin <= imax) 
        {
            // i: 在数组A中的分割点，A[0..i-1]属于左半部分
            int i = (imin + imax) / 2; 
            // j: 在数组B中的分割点，j = half - i，B[0..j-1]属于左半部分
            int j = half - i;   

            // 情况1：i 太小，需要右移
            // B左半部分的最后一个元素 > A右半部分的第一个元素
            if (i < m && j > 0 && B[j - 1] > A[i]) 
            {
                imin = i + 1;
            } 
            // 情况2：i 太大，需要左移
            // A左半部分的最后一个元素 > B右半部分的第一个元素
            else if (i > 0 && j < n && A[i - 1] > B[j]) 
            {
                imax = i - 1;
            } 
            // 情况3：找到合适的分割点
            else 
            {
                // 计算左半部分的最大值
                int maxLeft;
                if (i == 0) 
                {
                    maxLeft = B[j - 1];      // A左半部分为空
                }
                else if (j == 0) 
                {
                    maxLeft = A[i - 1];      // B左半部分为空
                }
                else 
                {
                    maxLeft = max(A[i - 1], B[j - 1]);
                }

                // 奇数长度，中位数就是左半部分的最大值
                if ((m + n) % 2 == 1) 
                {
                    return maxLeft; 
                }

                // 偶数长度，需要计算右半部分的最小值
                int minRight;
                if (i == m) 
                {
                    minRight = B[j];         // A右半部分为空
                }
                else if (j == n) 
                {
                    minRight = A[i];         // B右半部分为空
                }
                else 
                {
                    minRight = min(A[i], B[j]);
                }

                // 偶数情况，中位数是左右两部分中间值的平均数
                return (maxLeft + minRight) / 2.0;
            }
        }
        
        return 0.0; // 理论上不会执行到这里
    }
};
