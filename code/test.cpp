#include <iostream>
#include <ctime>
#include <stdexcept> 
#include "Array.hpp"
#include "SinglyLinkList.hpp"
#include "SinglyCircularLinkList.hpp"
#include "DoublyLinkList.hpp"

using std::cout;
using std::endl;

void arrayTest()
{
    using namespace ArrayNameSpace;
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
    using namespace SinglyLinkListNameSpace;
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
    SLL.push_front(10);
    SLL.show_list();
    SLL.remove_once(10);
    SLL.show_list();
    SLL.reverse();
    cout << "遍历SLL链表:";
    SLL.show_list();
    try {
        cout << "SLL链表倒数第2个节点的数值:" << SLL.get_last_k(2) << endl;
    }
    catch(const std::out_of_range& e)
    {
        cout << "发现异常：" << e.what() << endl;
    }

    SinglyLinkList<int> sort2;
    SinglyLinkList<int> sort1;
    sort1.push_back(1);
    sort1.push_back(4);
    sort1.push_back(5);
    sort2.push_back(2);
    sort2.push_back(3);
    sort1.show_list();
    sort2.show_list();
    merge_sort_list(sort1,sort2);
    sort1.show_list();
    sort2.show_list();

    cout << "SLL链表长度：" << SLL.get_length() << endl;
    SLL.rotate_backward(3);
    SLL.show_list();
    SLL.rotate_backward(0);
    SLL.show_list();
}

void singlyCircularLinkListTest()
{
    using namespace SinglyCircularLinkListNameSpace;
    SinglyCircularLinkList<int> SCLL1;
    SCLL1.show_list();
    cout << SCLL1.delete_first(1) << endl;
        for(int i = 0;i < 10;i++)
    {
        int val = rand() % 100;
        SCLL1.push_back(val);
        cout << val << " ";
    }
    cout << endl;
    SCLL1.push_back(114514);
    SCLL1.push_front(1919180);
    SCLL1.show_list();
    cout << SCLL1.delete_first(114514) << endl;
    cout << SCLL1.delete_first(99999) << endl;
    SCLL1.show_list();
    SCLL1.joseph(3);
}

void doublyLinkListTest()
{
    using namespace DoublyLinkListNameSpace;
    DoublyLinkList<int> DLL1;
    DLL1.push_back(8);
    DLL1.push_front(10);
    DLL1.push_front(114514);
    DLL1.show_list();
    cout << DLL1.search(114514) << endl;
    cout << DLL1.delete_first(114514) << endl;
    DLL1.show_list();

}

int main()
{
    doublyLinkListTest();
    return 0;
}