//
// Created by 78472 on 2021/7/1.
//

#include "mvector.h"

/*
 * 构造函数
 *      空容器
 *      拷贝构造/move构造
 *      n个, 元素的default构造函数
 *      n个指定的元素
 *      初值列
 *      区间（前闭后开）
 */
void container::vector_construct(){
    vector<string> emptyVector;     //空vector
    cout << "emptyVecotr.size(): " << emptyVector.size() << endl;
    cout << "emptyVecotr.capicity: " << emptyVector.capacity() << endl;

    vector<int> defaultN(2);   //用元素的default构造函数，生成一个大小为n的vector
    cout << "defaultN.size(): " << defaultN.size() << endl;
    cout << "defaultN.capicity: " << defaultN.capacity() << endl;
    for(auto &elem : defaultN){
        cout << "defaultN: " << elem << endl;
    }

    vector<string> defaultNElem(2, "hello");  //建立一个大小为n的vector, 每个元素值都是elem
    cout << "defaultNElem.size(): " << defaultNElem.size() << endl;
    cout << "defaultNElem.capicity: " << defaultNElem.capacity() << endl;
    for(auto &elem : defaultNElem){
        cout << "defaultNElem: " << elem << endl;
    }

    vector<string> initlist = {"hello", "world!"};      //初值列
    cout << "initlist.size(): " << initlist.size() << endl;
    cout << "initlist.capicity: " << initlist.capacity() << endl;
    for(auto &elem : initlist){
        cout << "initlist: " << elem << endl;
    }

    vector<string> range(initlist.begin(), initlist.end());    //区间作为元素初值
    cout << "range.size(): " << range.size() << endl;
    cout << "range.capicity: " << range.capacity() << endl;
    for(auto &elem : range){
        cout << "range: " << elem << endl;
    }

    vector<string> copyConstructor(initlist);   //拷贝构造
    cout << "copyConstructor.size(): " << copyConstructor.size() << endl;
    cout << "copyConstructor.capicity: " << copyConstructor.capacity() << endl;
    for(auto &elem : copyConstructor){
        cout << "copyConstructor: " << elem << endl;
    }

    vector<string> reserveVector;
    reserveVector.reserve(80);      //保留固定的空间，避免内存重新分配
    cout << "reserveVector.size(): " << reserveVector.size() << endl;
    cout << "reserveVector.capacity(): " << reserveVector.capacity() << endl;
}


/*
 * 赋值操作
 *      赋值操作可能调用元素类型的default构造函数，copy构造函数，assignment操作符，或析构函数
 *
 *      1. 给容器赋特定的值（n个相同的值）
 *      2. 一个初值列
 *      3. 一个区间的值
 *      4. 将一个容器赋值给另一个容器
 *      5. 交换2个容器的值
 */
void container::vector_assign(){

    vector<string> sample0 = {"hello", "how", "are", "you", "!"};
    vector<string> sample1{"world", "hello"};
    vector<string> assignment;

    assignment.assign(2, "hello");       //n个元素
    for(auto &elem : assignment){
        cout << "0--v: " << elem << endl;
    }

    assignment.assign({"ni", "hao", "hao"});  //初值列赋值
    for(auto &elem : assignment){
        cout << "1--v: " << elem << endl;
    }

    assignment.assign(sample0.begin(), sample0.end());  //区间赋值
    for(auto &elem : assignment){
        cout << "2--v: " << elem << endl;
    }

    assignment = sample0;    //整个容器赋值
    for(auto &elem : assignment){
        cout << "4--v: " << elem << endl;
    }

    sample0.swap(sample1);    //交换容器内容
    for(auto &elem : sample0){
        cout << "sample0: " << elem << endl;
    }
    for(auto &elem : sample1){
        cout << "sample1: " << elem << endl;
    }
}



/*
 * 元素索引： 必须确定范围
 *      [] : 索引必须有效，如果越界会导致未定义错误
 *      front, back : 容器必须不为空
 *      at : 越界会报异常
 */
void container::vector_access(){
    vector<string> v;
    cout << "idx0: " << v[0] << endl;          //按下标索引
    cout << "idx1: " << v.at(1) << endl;    //安全索引, 越界会抛出异常
    cout << "front: " << v.front() << endl;    //第一个元素
    cout << "back: " << v.back() << endl;      //最后一个元素
}


/*
 *  1. 元素的插入
 *  2. 元素的删除
 *  3. 清空容器
 *  4. 更改容器大小
 */
void container::vector_insert_delete(){

    vector<string> sample = {"welcome", "to", "here"};
    vector<string> v = {"1", "2", "3", "4", "5"};

    v.insert(v.begin() + 1, "hello");           //插入一个元素
    v.insert(v.end(), 2, "world");                   //插入n个元素
    v.insert(v.begin(), {"huan", "ying"});              //插入一个初值列
    v.insert(v.end(), sample.begin(), sample.end());       //插入一个区间

    v.push_back("smart");   //尾端插入
    v.pop_back();           //尾端删除

    for(auto &elem : v){
        cout << "elem: " << elem << endl;
    }

    v.erase(v.begin());                             //移除某个位置的元素
    v.erase(v.begin(), v.end() - 1);            //移除一个区间
    for(auto &elem : v){
        cout << "left: " << elem << endl;
    }

    v.resize(2);                            //更改容器大小（可能会调用析构函数或default构造函数）
    for(auto &elem : v){
        cout << "resize: " << elem << endl;
    }

    v.clear();
    std::cout << "v.capacity(): " << v.capacity() << std::endl;

}


void container::deque_operate(){

    //创建类的对象并赋初值
    string s1 = "s1";
    std::vector<string> v = {"1", "2", "3"};
    std::deque<string> coll;
    coll = {"first", "second", "third"};

    //插入方法
    coll.insert(coll.begin(), "1");
    coll.insert(coll.begin(), 2, "2");
    coll.insert(coll.end(), {"end1", "end2"});
    coll.insert(coll.end(), v.begin(), v.end());

    //首端，尾端插入
    coll.push_front("hello");
    coll.emplace_back("world");

    copy(coll.begin(), coll.end(), ostream_iterator<string>(cout, " "));
    cout << endl;

    //删除单个，删除一个区间
    coll.erase(coll.end());
    coll.erase(coll.begin(), std::next(coll.begin(), 2));
    copy(coll.begin(), coll.end(), ostream_iterator<string>(cout, " "));
    cout << endl;

    //元素访问：首端，末尾，任意位置
    cout << "coll.front(): " << coll.front() << endl;
    cout << "coll.back(): " << coll.back() << endl;
    coll.pop_front();
    coll.pop_back();
    cout << "firt element of coll: " << coll[0] << endl;
    copy(coll.begin(), coll.end(), ostream_iterator<string>(cout, " "));
    cout << endl;

    //变更存储的元素的数量
    coll.resize(2);
    coll.clear();
}







