#include<iostream>
#include"binary_search.h"
#include"sort.h"
#include "heap.h"

#include<time.h>
#include<stdlib.h>

using namespace std;

#if 1
int main()
{
    using namespace data_structures::priority_queue;
    PriorityQueue pqmax();//大根堆
    PriorityQueue pq(20,[](int a,int b){return a < b;});//小根堆

    const int SIZE = 10;
    int arr[SIZE];
    srand(time(NULL));
    int temp{};
    for(int i = 0;i < SIZE;++i)
    {
        temp  = rand() % 100;
        cout << temp << endl;
        pq.push(temp);
        pq.traverse();
    }
    pq.pop();
    pq.traverse();



    
}
#endif