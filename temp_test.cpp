    double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) 
    {
        int sum_num = nums1.size() + nums2.size(); //获得总数
        bool is_odd = (sum_num % 2 == 1); //总数是奇数
        if (nums1.front() > nums2.back())  //nums1最小的比nums2最大的大
        {
            if (sum_num/2 > nums2.size()) //在nums1里
            {

            }
            
        }
        else if (nums2.front() > nums2.back())
        {

        }
        else //存在k，k前比nums2小，后面比nums2大
        {
            size_t j = 0;
            size_t k = bs(nums1, nums2[j]); //获得的k是比nums2首个元素小或者相等的元素下标

            //奇数：k + j + 2不包括中位数；偶数：k + j + 2刚好是一半
            while (k + j + 2 < sum_num/2)
            {
                if (j < nums2.size() - 1)
                { 
                    ++j;
                }
                if (k < nums1.size() - 1)
                {
                    while (nums1[k] <= nums2[j] &&  (k + j + 2 < sum_num/2))
                    {
                        ++k;
                    }
                }
            }

            while (k + j + 2 > sum_num / 2)
            {
                --k;
                
            }

        }
    }

    size_t bs(vector<int> &vec, int &num)
    {
        size_t index {};
        size_t beg = 0;
        size_t end = vec.size() - 1;
        while (beg <= end)
        {
            index = (beg + end) / 2;
            if (vec[index] == num)
            {
                return index;
            }
            else if (vec[index] > num)
            {
                end = index - 1;
            }
            else
            {
                beg = index + 1;
            }
        }
        
        //最后return end肯定比num小
        return end;
    }