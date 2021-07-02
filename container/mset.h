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
 *
 *
 *
 *
 */

#include <set>
#include <iostream>

namespace container {
    void set_comp(void);

}






















#endif //STL_MSET_H
