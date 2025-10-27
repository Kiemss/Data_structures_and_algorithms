#include<vector>
#include<iostream>

using std::cout;
using std::endl;

class Solution {
public:
    bool isPalindrome(int x) 
    {
        if(x < 0) return false;
        std::vector<int> nums = {};
        while(x > 0)
        {
            nums.push_back(x%10);
            x /= 10;
        }
        auto lit = nums.begin();
        auto rit = nums.end() - 1;
        while(lit < rit)
        {
            if(*lit != *rit) return false;
            ++lit;
            --rit;
        }
        return true;
    }
};

#if 0
int main()
{
    Solution sol;
    cout << sol.isPalindrome(121);
}
#endif