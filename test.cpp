#include <iostream>
#include <ctime>
#include "array.hpp"
#include"SinglyLinkList.hpp"

using std::cout;
using std::endl;

void arrayTest()
{
        Array<int> arr;
    srand(time(0));
    for(int i = 0; i < 10; i++)
    {
        arr.push_back(rand()%100);
    }
    arr.traverse();
    arr.pop_back();
    arr.traverse();
    arr.insert(0,100);
    arr.traverse();
    std::cout << arr.find(100) << std::endl;
    arr.remove_element(100);
    arr.traverse();
    arr.reverse();
    arr.traverse();
    arr.int_sort_better();
    arr.traverse();
    arr.size();
    arr.insert(3,100);

    int tarr[] = { 1, 2, 3, 2, 3, 1, 3, 2, 1};
    int val = 3;
    std::cout << delete_element(tarr,9 , val) << endl;
}

void singlyLinkListTest()
{
    SinglyLinkList<int> SLL;
    srand(time(0));
    for(int i = 0;i < 10;i++)
    {
        int val = rand() % 100;
        SLL.push_back(val);
        cout << val << " ";
    }
    cout << endl;
    SLL.show_list();
    SLL.push_back(10);
    SLL.show_list();
    SLL.remove_once(10);
    SLL.show_list();
    SLL.reverse();
    SLL.show_list();
}

int main()
{
    singlyLinkListTest();
    return 0;
}