//
// Created by lhy on 2021/1/3.
//

#ifndef STL_MATOMIC_H
#define STL_MATOMIC_H

/*
 * 1. 一般而言即使面对基本数据类型, 读和写也不是atomic的（不可切割）.因此可能读到一个被写一半的值, 这会带来不明确行为.
 * 2. 编译器生成的代码有可能改变操作次序, 所以供应端线程有可能在供应数据之前就设置ready flag, 而消费端线程亦有可能在侦测到ready flag之前就处理该数据。
 * 借由mutex, 这2个问题迎刃而解. 但是从必要的资源和潜藏的独占访问来看, mutex也许是个相对昂贵的操作.
 *
 * 线程之中atomic操作一定保证像"代码出现的次序"那样地发生.
 *
 *
 */


namespace mthread{

    void atomictest();

}





#endif //STL_MATOMIC_H
