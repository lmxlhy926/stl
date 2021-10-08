//
// Created by 78472 on 2021/7/2.
//

#ifndef STL_MSET_H
#define STL_MSET_H


/*
 *  set/multiset: 根据特定的排序准则，自动将元素排序。
 *      1. 可依据某排序准则被比较的任意类型T，都可以成为set/multiset的元素类型
 *      2. 模板的第二个参数是排序准则的类型，真正的排序准则是容器依据该类型产生的函数对象
 *      3. 自动排序的主要优点在于令二叉树于查找元素时具有良好性能。
 *      4. 自动排序造成的限制：不能直接改变元素值，这样会打乱原本正确的顺序
 *      5. 赋值包括元素的赋值和排序准则对象的赋值（排序准则对象的类必须相同，否则无法赋值）
 */

/*
 * 执行动作
 *      插入（insert），删除（erase），查找（find）
 *      赋值（元素赋值，排序准则对象赋值）
 */

#include <set>
#include <iostream>

namespace container {

    class runtimeCmp{
    private:
        int mode = 0;
    public:
        explicit runtimeCmp(int option = 0) : mode(option){}

        template<typename T>
        bool operator()(const T& t1, const T& t2){
            return mode == 0 ? t1 < t2 : t2 < t1;
        }

        bool operator == (const runtimeCmp& t){
            return mode == t.mode;
        }
    };

    typedef std::set<int, runtimeCmp> IntSet;

    void set_insert_erase();

    void set_comp_own();

    void set_assign();

}






















#endif //STL_MSET_H
