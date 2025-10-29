#pragma once
#include<iostream>

//使用两个队列实现栈
#include<queue>
using namespace std;
class MyStack {
public:
    MyStack() {
        qc = new queue<int>;
        qe = new queue<int>;
    }
    
    void push(int x) {
        qe->push(x);
        while(!qc->empty())
        {
            qe->push(qc->front());
            qc->pop();
        }
        queue<int>* temp = qc;
        qc = qe;
        qe = temp;
        temp = nullptr;
    }
    
    int pop() {
        int temp = qc->front();
        qc->pop();
        return temp;
    }
    
    int top() {
        return qc->front();
    }
    
    bool empty() {
        return (qc->empty());
    }
private:
    queue<int>* qc;
    queue<int>* qe;
};
