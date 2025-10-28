#include<iostream>
#include"binary_search.h"
#include"sort.h"

#include<time.h>
#include<stdlib.h>

using namespace std;

#if 1
int main()
{
    const int SIZE = 10;
    int arr[SIZE];
    srand(time(NULL));
    for(int i = 0;i < SIZE;++i)
    {
        arr[i] = rand() % 100;
        cout << arr[i] << " ";
    }
    cout << endl;
    //下面是算法

    quick_sort(arr,0,9);    

    for(int i = 0;i < SIZE;++i)
    {
        cout << arr[i] << " ";
    }
}
#endif