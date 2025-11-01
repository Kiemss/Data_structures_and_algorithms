#pragma once
#include<iostream>
#include<algorithm>
#include<string>
#include<vector>

//冒泡排序
void bubble_sort(int arr[],int size)
{
    for(int i = 0;i < size - 1;++i)
    {
        bool flag = false;
        for(int j = 0;j < size - 1 - i;++j) 
        {
            if(arr[j] > arr[j+1])
            {
                int temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
                flag = true;
            }
        }

        //如果这一趟没有做过任何数据交换，说明已经有序
        if(!flag) return;
    }
}

//选择排序
void selection_sort(int arr[],int size)
{
    int min_index {};
    int temp {};
    for(int i = 0;i < size -1 ; ++i)
    {
        min_index = i;
        for(int j = i + 1;j < size;++j)
        {
            if(arr[j] < arr[min_index])
            {
                min_index = j;
            }
        }
        if(min_index != i)
        {
            temp = arr[i];
            arr[i] = arr[min_index];
            arr[min_index] = temp;
        }
    }
}

#if 0
//下面是自己写的烂代码
void insertion_sort(int arr[],int size)
{
    int cur {};
    for(int i = 1;i < size;++i)
    {
        cur = arr[i];
        for(int j = i - 1;j >= 0; --j)
        {
            if(cur < arr[j])
            {
                arr[j+1] = arr[j];
                if(j == 0)
                {
                    arr[0] = cur;//冗余判断
                }
            }
            else if(cur >= arr[j])
            {
                arr[j+1] = cur;
                break;
            }
        }
    }
}
#endif

//插入排序
void insertion_sort(int arr[],int size)
{
    int cur {};
    for(int i = 1;i < size;++i)
    {
        cur = arr[i];

        //将比cur大的元素往后移动,使用for循环+if也是可以实现的
        int j= i -1 ;
        while(j >= 0 && arr[j] > cur)
        {
            arr[j+1] = arr[j];
            j--;
        }

        //插入cur到正确位置
        arr[j+1]  = cur;
    }
}

//希尔排序
void shell_sort(int arr[],int size)
{
    //⭐实际上就是在插入排序的基础上，添加一个额外的gap循环，然后将原来循环中的部分"1"改成gap
    for(int gap = size/2;gap > 0;gap /= 2)
    {
        for(int i = 0;i < size/gap;i++)//遍历各个分组的循环
        {
            //跳过每组第一个元素开始排序，注意不是先排完一组再排下一组
            for(int i = gap;i < size;i++)
            {

                int val = arr[i];//用来插入的元素
                int j = i - gap;//已经排序完的元素组的最后一个元素的index
                //
                for(;j >= 0;j -= gap)
                {
                    if(arr[j] <= val) break;
                    arr[j + gap] = arr[j];
                }
                //插入cur到正确位置
                arr[j + gap] = val;
            }
        }
    }
}

//快速排序
int quick_sort_partation(int arr[],int begin,int end)
{
    //使用三数取中法进行优化,这里的三数取中使用了冒泡排序
    int mid = arr[(begin + end)/2];
    if(arr[begin] > arr[mid]) std::swap(arr[begin],arr[mid]);
    if(arr[mid] > arr[end]) std::swap(arr[mid],arr[end]);
    if(arr[begin] > arr[mid]) std::swap(arr[begin],arr[mid]);
    if(arr[begin] != arr[mid]) std::swap(arr[begin],arr[mid]);//最后将中间值和第一个元素互换
    //选取第一个元素作为基准元素
    int val = arr[begin];

    while(begin < end)
    {
        //额外的判定，防止自己和自己换
        /*
        ⭐不需要额外设置i或者j从begin\end开始遍历，
        因为直接用begin\end遍历过的地方就说明是比基准元素小\大的
        这样就相当于优化
        */
        while(arr[end] >= val && begin < end)//额外判定begin < end，防止基准元素自己和自己交换
        {
            --end;
        }

        if(begin < end)//每次的额外判定，理清边界范围
        {
            arr[begin] = arr[end];   
            ++begin; //记得变更begin\end位置
        }
        
        while(arr[begin] < val && begin < end)
        {
            ++begin;
        }

        if(begin < end)
        {
            arr[end] = arr[begin];
            --end;
        }
    }
    arr[begin] = val;//最后将基准元素放到中间位置
    return begin;
}

void quick_sort(int arr[],int begin,int end)
{
    if(begin >= end) return; //递归结束的条件

    if((end - begin) <= 100) 
    {
        selection_sort(arr,end-begin+1);//优化
        return;
    }
    int mid = quick_sort_partation(arr,begin,end);

    quick_sort(arr,mid + 1,end);
    quick_sort(arr,begin,mid - 1);
}

//归并排序
void merge(int arr[],int left,int mid,int right);
//归并排序接口
void merge_sort(int arr[],int begin,int end)
{
    //递归结束条件
    if(begin >= end)
    {
        return;
    }

    //先递
    int mid = (begin + end)/2;
    merge_sort(arr,begin,mid);
    merge_sort(arr,mid + 1,end);

    //⭐再归并(理解归并排序代码启用的关键！)
    merge(arr,begin,mid,end);
}

//归并排序
void merge(int arr[],int left,int mid,int right)
{
    int* new_arr = new int[right - left + 1];//申请合并后的数组内存空间
    int arr_index = 0;
    //排序
    int i = left;
    int j = mid + 1;
    while(i <= mid && j <= right)//注意等号，边界条件！
    {   
        if(arr[i] <= arr[j])//等号情况也判，保持稳定性
        {
            new_arr[arr_index++] = arr[i++]; 
        }
        else 
        {
            new_arr[arr_index++] = arr[j++];
        }
    }

    //使用while而不是if处理剩余元素（虽然逻辑上不会剩下多余一个的元素）
    while(i <= mid)
    {
        new_arr[arr_index++] = arr[i++];
    }
    while(j <= right)
    {
        new_arr[arr_index++] = arr[j++];
    }

    //将已排序数据拷贝进原数组
    for(i = 0, j = left;j <= right;++i,++j)
    {
        arr[j] = new_arr[i];
    }

    //记得delete
    delete[] new_arr;

    return;
}

//堆排序
void sift_down(int arr[],int index, int size)
{
    int val = arr[index];//先记录要调整的元素
    while(index * 2 + 1 < size)//有左孩子就继续
    //如果使用 index < (size - 1)/2 就是表示到最后一个父节点就结束
    {
        int child = index * 2 + 1;//默认为左子节点
        if(child + 1 < size && arr[child] < arr[child + 1])
        {
            ++child;
        }
        if(arr[child] > val)//子节点比父节点大，父节点下沉
        {
            arr[index] = arr[child];
            index = child;  
        }
        else
        {
            break;
        }
    }
    arr[index] = val;
}

void heap_sort(int arr[],int size)
{
    for(int i = (size - 1)/2;i >= 0;--i)//从最大的父节点开始
    {
        sift_down(arr,i,size);//单次调用是让单个节点上浮到合适位置
        //for循环调用就是构建大根堆
    }

    //把堆顶元素和末尾元素进行交换，从堆顶开始下沉操作
    for(int i = size - 1;i > 0;--i)
    {
        std::swap(arr[0],arr[i]);//交换堆顶和堆尾

        sift_down(arr,0,i);//将被交换到堆顶的元素重新下沉到合适的位置
    }
}

//基数排序
void radix_sort(int arr[],int size)
{
    if(size <= 0) throw std::underflow_error("array is empty!");
    int max = arr[0];
    for(int i = 1;i < size;++i)
    {
        if(arr[i] > max) max = arr[i];//找到数组中元素最大值 
    }
    int len = std::to_string(max).size();//获取最大位数

    std::vector<std::vector<int>> vecs(10);//二维数组当“桶”,必须先初始化，否则无法直接下标访问

    int dev = 1;
    for(int i = 0;i < len;++i,dev *= 10)//最外层循环：从个位到最高位
    {
        vecs.resize(10);//清空后需要resize(10),相当于创建10个桶，否则无法访问下标
        //如果想不重新创建，就需要单独清空每个小桶而不是整个大桶
        int index {};
        for(int j = 0;j < size;++j)//遍历数组
        {
            index = (arr[j] /dev) % 10;//得到相应位的数字
            vecs[index].push_back(arr[j]);//将该数填进相应的桶中
        }

        //将桶中元素再次放回数组中
        int idx = 0;
        for(auto& vec : vecs)
        {
            for(int v : vec)
            {
                arr[idx++] = v;
            }
        }

        vecs.clear();//记得清空vecs，这种相当于清空大桶      
    }
}
