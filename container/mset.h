//
// Created by 78472 on 2021/7/2.
//

#ifndef STL_MSET_H
#define STL_MSET_H


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






















#endif //STL_MSET_H
