#include<iostream>
#include"binary_search.h"
#include"sort.h"
#include "heap.h"
#include "linear_hash_table.h"

#include<time.h>
#include<stdlib.h>
#include<algorithm>

using namespace std;

//获取随机数组并输出其元素，返回数组指针
int* get_random_arr
(int size //数组大小
,int range//随机数范围：0~range
)
{
    int* arr = new int[size];
    srand(time(NULL));
    int temp{};
    for(int i = 0; i < size;++i)
    {
        arr[i] = rand() % range;
        cout << arr[i] << " ";
    }
    cout << endl;
    return arr;
}


#if 1
int main()
{
    using namespace data_structures::priority_queue;
    using namespace data_structrues::linear_hash_table;

    const int SIZE = 10;
    const int RANGE = 100;
    int* arr = get_random_arr(SIZE,RANGE);

    HT ht;;
   ht.insert(3);
   ht.insert(10);
   ht.insert(17);
   cout << ht.find(11);
   cout << ht.erase(10);
   cout << ht.find(17);
   ht.insert(66);
   ht.find(66);
    for(int i = 0;i < SIZE;++i)
    {
        ht.insert(arr[i]);
        cout << ht.get_load_factor() << endl;
    }
}
#endif