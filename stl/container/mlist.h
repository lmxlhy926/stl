//
// Created by 78472 on 2021/11/13.
//

#ifndef STL_MLIST_H
#define STL_MLIST_H

/*
 *  双向列表list对象自身提供了2个pointer或称anchor，用来指向第一个或最末一个元素。
 *  list不支持随机访问，要访问第5个元素，就必须顺着串链逐一爬过前4个元素。
 *  由于不支持随机访问，list既不提供subscript操作符，也不提供at()。
 *  特别的list提供了去重，拼接，排序，合并方法。
 */


namespace container{

    void listfunctions();
}


#endif //STL_MLIST_H
