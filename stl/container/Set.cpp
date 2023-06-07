//
// Created by 78472 on 2021/7/2.
//
#include "mset.h"
#include <iterator>
using namespace std;


/*
 *  set/multiset: 根据特定的排序准则，自动将元素排序。
 *      1. 可依据某排序准则被比较的任意类型T，都可以成为set/multiset的元素类型
 *      2. 模板的第二个参数是排序准则的类型，真正的排序准则是容器依据该类型产生的函数对象
 *      3. 自动排序的主要优点在于令二叉树于查找元素时具有良好性能，其查找函数具有对数复杂度。
 *      4. 自动排序造成的限制：不能直接改变元素值，这样会打乱原本正确的顺序
 *      5. 要改变元素值，必须先删除旧元素，再插入新元素。
 *
 *      排序准则：
 *          1. 以template参数定义之
 *          2. 以构造参数定义之
 *          3. 元素比较动作只适用于类型相同的容器。容器类型相同：元素类型相同，排序准则类型相同
 *          4. 赋值包括元素的赋值和排序准则对象的赋值（排序准则对象的类必须相同，否则无法赋值）
 */

/*
 * 执行动作
 *      插入（insert），删除（erase），查找（find）
 *      赋值（元素赋值，排序准则对象赋值）
 */

#include <set>
#include <iostream>
using namespace std;

namespace container {

    class runtimeCmp{
    public:
        enum cmp_mode {normal, reverse};
    private:
        cmp_mode mode;
    public:
        explicit runtimeCmp(cmp_mode option = normal) : mode(option){}

        template<typename T>
        bool operator()(const T& t1, const T& t2){      //任何元素类型的比较
            return mode == normal ? t1 < t2 : t2 < t1;
        }

        bool operator == (const runtimeCmp& t){     //排序准则对象的比较
            std::cout << "sorting criterion compare..." << endl;
            return mode == t.mode;
        }
    };
    using IntSet = std::set<int, runtimeCmp>;

    //引用、插入、删除
    void set_insert_erase();

    //排序准则类与排序准则对象
    void set_comp_own();

    //set对象赋值：同时赋元素值和排序准则对象
    void set_assign();

    void set_operate();

}


void container::set_insert_erase(){

    std::set<int, greater<>> a({6,7,8,9});
    std::set<int> s({1,2,3,4});
    copy(a.begin(), a.end(), ostream_iterator<int>(cout, " "));
    std::cout << endl;

    s.insert(5);                  //单值插入
    s.insert({10,11,12});         //初值列插入
    s.insert(a.begin(), a.end());   //区间插入（区间只是确定有哪些值，排序还是由当前的排序准则确定）
    copy(s.begin(), s.end(), ostream_iterator<int>(cout, " "));
    std::cout << endl;

    auto count = s.erase(1);              //删除特定值，返回删掉的数量
    s.erase(s.begin(), s.find(8));  //迭代器必须指向合法位置，区间的起始位置不能在结束位置之后
    copy(s.begin(), s.end(), ostream_iterator<int>(cout, " "));
    std::cout << endl;

    std::set<int>::iterator ite;
    ite = s.find(8);
    if(ite != s.end()){   //dereference s.end()会导致未定义错误
        cout << "s.find(8): " << *ite << endl;
    }

}


/*
 * 排序准则类型和排序准则对象：
 *          1. 模板参数指定的是排序准则类型
 *          2. 创建对象时实际传递的是排序准则对象，如果不传递就使用default构造函数生成的对象
 *          3. 赋值操作：同时赋值元素和排序准则对象
 */
void container::set_comp_own(){
    IntSet s({4,3,2,1});           //排序准则是个对象：默认是调用类的default构造函数产生的函数对象
    std::cout << "operator<: ";
    copy(s.begin(), s.end(), ostream_iterator<int>(cout, " "));
    std::cout << endl;

    runtimeCmp reverse_order(runtimeCmp::reverse);
    IntSet s1(reverse_order);   //排序准则类的对象
    s1 = {4,3,2,1};
    std::cout << "operator>: ";
    copy(s1.begin(), s1.end(), ostream_iterator<int>(cout, " "));
    std::cout << endl;


    //模板参数指定的是排序准则类的类型，实际使用的是类产生的对象
    IntSet s2(s1.begin(), s1.end(), runtimeCmp());
    std::cout << "runtime op object: ";
    copy(s2.begin(), s2.end(), ostream_iterator<int>(cout, " "));
    std::cout << endl;

    s = s1;
    if(s.value_comp() == s1.value_comp()){
        cout << "s and s1 have the same sorting criterion" << endl;
    }
}


//set对象的赋值操作： 同时赋值元素和排序准则的对象
void container::set_assign(){
    IntSet s1({1,2,3}, runtimeCmp(runtimeCmp::normal));
    IntSet s2({6,5,4}, runtimeCmp(runtimeCmp::reverse));

    copy(s1.begin(), s1.end(), ostream_iterator<int>(cout, " "));
    std::cout << endl;
    s1 = s2;
    copy(s1.begin(), s1.end(), ostream_iterator<int>(cout, " "));
    std::cout << endl;
}


void container::set_operate(){
    set<int, greater<>> coll1;

    coll1.insert({1,2,3,4,5,6});
    coll1.insert(5);        //set不支持元素值重复
    for(auto &elem : coll1){
        cout << elem << " ";
    }
    cout << endl;

    auto status = coll1.insert(4);
    if(status.second){
        cout << "4 insertd as element " << distance(coll1.begin(), status.first) + 1 << endl;
    }else{
        cout << "4 is already exists" << endl;
    }

    set<int> coll2(coll1.begin(), coll1.end());
    copy(coll2.begin(), coll2.end(), ostream_iterator<int>(cout, " "));
    cout << endl;

    coll2.erase(coll2.begin(), coll2.find(3));
    unsigned long num = coll2.erase(5);
    cout << num << " element(s) removed" << endl;

    copy(coll2.begin(), coll2.end(), ostream_iterator<int>(cout, " "));
    cout << endl;
}



int main(int argc, char* argv[]){


    return 0;
}




