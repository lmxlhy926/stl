//
// Created by 78472 on 2022/1/15.
//

#include "stack_queue.h"
#include <iostream>
#include <stack>
#include <queue>
#include <vector>

using namespace std;

void stacktest(){

    std::stack<int> st;
    st.push(1);
    st.push(2);
    st.push(3);     // 1 2 3

    cout << st.top() << endl;
    st.pop();
    cout << st.top() << endl;
    st.pop();

    st.top() = 77;    //77

    st.push(4);
    st.push(5);    //77 4 5

    while(!st.empty()){
        cout << st.top() << endl;
        st.pop();
    }
    cout << endl;

    stack<pair<string, string>> st1;
    auto p = std::make_pair("hello", "world");
    st1.push(std::move(p));
    st1.emplace(make_pair("hello1", "world"));
    while(!st1.empty()){
        std::cout << st1.top().first << " : " << st1.top().second << endl;
        st1.pop();
    }
}


void queuetest(){
    std::queue<string> q;
    q.push("These ");
    q.push("are ");
    q.push("more than ");   //These are more than

    cout << q.front();
    q.pop();
    cout << q.front();
    q.pop();

    q.push("four ");
    q.push("words!");

    q.pop();

    cout << q.front();
    q.pop();
    cout << q.front() << endl;
    q.pop();

    cout << "number of elements in the queue: " << q.size() << endl;
}


void priorityQueuetest(){
    priority_queue<float> pqf;
    pqf.push(11.1);
    pqf.push(22.2);
    pqf.push(33.3);

    cout << pqf.top() << endl;
    pqf.pop();
    cout << pqf.top() << endl;
    pqf.pop();
    cout << pqf.top() << endl;
    pqf.pop();
    cout << endl;

    priority_queue<int, vector<int>, greater<>> pqi;
    pqi.push(3);
    pqi.push(2);
    pqi.push(1);

    cout << pqi.top() << endl;
    pqi.pop();
    cout << pqi.top() << endl;
    pqi.pop();
    while(!pqi.empty()){
        cout << pqi.top() << endl;
        pqi.pop();
    }
}


void stackQueueConstructor(){
    std::deque<int> v{3,2,1};
    //explicit contadapt::contadapt(const Container& cont)
    //创建一个stack或者queue，以容器cont的元素为初值，或者必须是container adapter的container_type之下的一个对象
    std::stack<int> s(std::move(v));

    while(!s.empty()){
        cout << s.top() << endl;
        s.pop();
    }
}



void priorityQueueConstructor(){
    vector<int> v{1,2,3};
    priority_queue<int> pq(v.begin(), v.end());
    while(!pq.empty()){
        cout << pq.top() << endl;
        pq.pop();
    }
}






























