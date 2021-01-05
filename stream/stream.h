//
// Created by lhy on 2021/1/5.
//

#ifndef STL_STREAM_H
#define STL_STREAM_H

/*
    用于I/O的各个class, 是c++标准库的重要组成部分. IOStream程序库提供了一系列I/O class.

    stream是由某个class定义出来具有特定性质的对象. 输出动作被解读为"数据流入stream", 输入动作则是"数据流出stream".

标准的stream操作符： << 和 >>
    Operator >> 和 Operator << 被相应的stream class重载, 分别用于输入和输出. 于是c++的移位操作符变成了I/O操作符.
basic_ostream将<<定义为output操作符, 对所有基础类型（语言内建）均重载, 不包括void和nullptr_t, 但包括char*和void*
输出机制的可扩展性, 让用户自定义类型可以方便的融入I/O系统. 程序员不需要再指定打印的类型, 只要针对不同类型进行重载, 就可
保证编译器可以自动推断出正确的打印函数. 按规定<<会返回第一实参, 也就是output stream, 这使得可以将输出动作串成一串.
    basic_istream将>>定义为input操作符, 也几乎对所有基础类型重载了operator >>, 不包括void和nullptr_t, 但包括char*
和void*. 也可以对任意类型重载input操作符, 并串连运用它们.


全局性的stream对象
    如cin,cout等, 在默认情况下这些stream都和标准c stream同步, 即"混合使用c++ stream和c stream"时, 保证
任何标准c++ stream缓冲区在写数据前, 都会先刷新其所对应的c stream缓冲区, 反之亦然。在多个并行线程中使用它们并
不会引发不确定行为。但可能导致字符交错或读到一个不确定的字符.

stream的状态：
    Stream定义了一些类型为iostate的常量, 用以反映stream的状态。这些常量分别是：goodbit, eofbit, failbit, badbit.
同时定义了一些成员函数来检测状态、返回状态或主动设置、清除状态。


*/


namespace mstream{

    void statetest();


}





#endif //STL_STREAM_H
