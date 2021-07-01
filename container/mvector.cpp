//
// Created by 78472 on 2021/7/1.
//

#include "mvector.h"

/*
 * 构造函数
 *      空容器
 *      n个, 元素的default构造函数
 *      n个, 指定的元素
 *      初值列
 *      区间（前闭后开）
 */
void container::vector_construct(){
    vector<string> emptyVector;     //空vector
    cout << "emptyVecotr.size(): " << emptyVector.size() << endl;
    cout << "emptyVecotr.capicity: " << emptyVector.capacity() << endl;

    vector<string> v1 = {"hello", "world"};     //初值列
    cout << "v1.size(): " << v1.size() << endl;
    cout << "v1.capicity: " << v1.capacity() << endl;
    for(auto &elem : v1){
        cout << "v1: " << elem << endl;
    }

    vector<string> v2(v1.begin(), v1.end());    //区间作为元素初值
    cout << "v2.size(): " << v2.size() << endl;
    cout << "v2.capicity: " << v2.capacity() << endl;
    for(auto &elem : v2){
        cout << "v2: " << elem << endl;
    }

    vector<string> v3(2);   //用元素的default构造函数，生成一个大小为n的vector
    cout << "v3.size(): " << v3.size() << endl;
    cout << "v3.capicity: " << v3.capacity() << endl;
    for(auto &elem : v3){
        cout << "v3: " << elem << endl;
    }

    vector<string> v4(2, "hello");  //建立一个大小为n的vector, 每个元素值都是elem
    cout << "v4.size(): " << v4.size() << endl;
    cout << "v4.capicity: " << v4.capacity() << endl;
    for(auto &elem : v4){
        cout << "v4: " << elem << endl;
    }

}


/*
 * 赋值操作
 *      赋值操作可能调用元素类型的default构造函数，copy构造函数，assignment操作符，或析构函数
 */
void container::vector_assign(){

    vector<string> v = {"hello", "how", "are", "you", "!"};
    for(auto &elem : v){
        cout << "v: " << elem << endl;
    }

    v.assign({"ni", "hao", "hao"});  //初值列赋值
    for(auto &elem : v){
        cout << "1--v: " << elem << endl;
    }

    v = {"hello", "hello"};  //初值列赋值
    for(auto &elem : v){
        cout << "2--v: " << elem << endl;
    }

    v.assign(1, "world");   //n个相同的元素
    for(auto &elem : v){
        cout << "3--v: " << elem << endl;
    }

    vector<string> v1({"hello", "world", "beauty"});
    vector<string> v2({"world", "hello"});
    v1.swap(v2);    //元素交换
    for(auto &elem : v1){
        cout << "v1: " << elem << endl;
    }
    for(auto &elem : v2){
        cout << "v2: " << elem << endl;
    }
}

/*
 * 元素索引： 必须确定范围
 *      [] : 索引必须有效
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

void container::vector_insert_delete(){

    vector<string> v = {"1", "2", "3", "4", "5"};
    v.insert(v.begin() + 1, "hello");
    v.insert(v.begin() + 1, {"ok"});
    v.emplace_back("smart");
    for(auto &elem : v){
        cout << "elem: " << elem << endl;
    }

    v.erase(v.begin()); //移除某个位置的元素
    v.pop_back();   //移除最后一个元素
    for(auto &elem : v){
        cout << "left: " << elem << endl;
    }

    v.resize(2);    //更改容器大小
    for(auto &elem : v){
        cout << "resize: " << elem << endl;
    }

}




/*
 * 对象的构造
 * 元素的索引，插入、删除、赋值
 */
void vector_test1(){
    //构造器
    vector<string> presen = {"hi", "marry."}; //初值列构造
    vector<string> sentence(presen.begin(), presen.end());    //区间构造


    sentence.reserve(50);   //预先保留空间，避免发生内存空间重新分配

    //四种元素访问方式
    cout << "idx0: " << presen[0] << endl;          //按下标索引
    cout << "idx1: " << presen.at(1) << endl;    //安全索引
    cout << "front: " << presen.front() << endl;    //第一个元素
    cout << "back: " << presen.back() << endl;      //最后一个元素

    //元素的插入
    sentence.emplace_back("hello,");    //末尾插入
    sentence.emplace(sentence.end(), "are");    //按位置插入
    sentence.insert(sentence.end(), {"you", "?"});
    copy(sentence.cbegin(), sentence.cend(),
         ostream_iterator<string>(cout, " "));
    cout << endl;


    swap(sentence[3], sentence[5]); //交换元素
    sentence.insert(find(sentence.begin(), sentence.end(), "?"), "always");
    sentence.back() = "!";
    copy(sentence.cbegin(), sentence.cend(),
         ostream_iterator<string>(cout, " "));
    cout << endl;

    //元素的删除
    sentence.pop_back();    //元素的2种删除方式
    sentence.erase(sentence.end());
    copy(sentence.cbegin(), sentence.cend(),
         ostream_iterator<string>(cout, " "));
    cout << endl;

    cout << "size(): " << sentence.size() << endl;
    cout << "capacity(): " << sentence.capacity() << endl;
    cout << "max_size(): " << sentence.max_size() << endl;
}