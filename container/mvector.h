//
// Created by lhy on 2021/1/30.
//

#ifndef STL_MVECTOR_H
#define STL_MVECTOR_H
/*
通用操作
    1. 构造函数：创建一个对象
        default构造函数, copy构造函数, move构造函数
        针对初值列构造函数, 针对某给定区间构造函数

    2. 大小相关：
        是否为空, 有几个元素, 最大可以容纳多少元素, 不重新分配内存空间时最多可以容纳多少元素

    3. 赋值操作：
        copy赋值, move赋值

    4. 数据交换
        本质上交换的是指针，并不是拷贝内存, 开销较小

    5. 比较操作：
        ==, !=

    5. 迭代器函数
        返回相应的迭代器
 */


/*
vector
    功能：将元素置于dynamic array中加以管理的一个抽象概念

    vector支持随机访问，因此只要知道位置，你可以在常量时间内访问任何一个元素。

    Vector的容量：
        1. 一旦内存重新分配，vector元素相关的所有reference, pointer, iterator都会失效
        2. 内存重新分配很耗时间
        3. 可以使用reserve()保留适当容量, 避免重新分配内存

    元素访问：
        对于non-const vector, 这些函数都返回元素的reference， 因此可以使用这些操作函数更改元素内容。

    元素的插入、删除

 */

#include <vector>
#include <string>
#include <iostream>
#include <iterator>
#include <algorithm>

using namespace std;

namespace container{

    /*
     * 对象的构造
     * 元素的索引，插入、删除、赋值
     */
    void vector_test(){
        vector<string> presen = {"hi", "marry."}; //初值列构造
    //四种元素访问方式
        cout << "idx0: " << presen[0] << endl;  //按下标索引
        cout << "idx1: " << presen.at(1) << endl;
        cout << "front: " << presen.front() << endl;
        cout << "back: " << presen.back() << endl;


        vector<string> sentence(presen.begin(), presen.end());    //区间构造
        sentence.reserve(50);   //预先保留空间，避免发生内存空间重新分配
    //元素的插入
        sentence.emplace_back("hello,");    //末尾插入
        sentence.push_back("how");
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

}













#endif //STL_MVECTOR_H
