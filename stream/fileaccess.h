//
// Created by lhy on 2020/12/3.
//

#ifndef STL_FILEACCESS_H
#define STL_FILEACCESS_H


/*
 * 和C的文件访问机制相比, c++ file stream class的最大好处就是文件的自动管理.
 * 文件可在构造时期自动打开, 析构时期自动关闭.
 *
 * 对于可读又可写的stream, 不能在读写动作之间任意转换其读写属性,
 * 如果一定要转换, 必须进行一个seek动作, 到达当前位置, 再转换读写属性
 * 唯一例外是, 如果已经读到end-of-file, 可立即接着写入字符. 如果违背该规则, 将导致不确定性结果.
 *
 * 执行文件的打开动作后, 成功与否会反映在stream状态中, 因该在构造stream后检查其状态
 *
 * 自c++11开始, file stream提供了rvalue和move语义（move构造函数, move assignment操作符）
 * 因此, 可以使用临时创建的stream对象
 * 可以传入一个file stream作为函数实参, 或让某个函数返回一个file stream. 即可以做到file对象被使用超过其“被创建时的作用域”
 *
 */


/*
 * C/C++打开文件的模式
 * r: 读取, 从起点开始, 文件必须存在             in
 * r+: 读和写, 最初位置在起点, 文件必须存在       in | out
 *
 * w: 清空而后涂写, 文件可以不存在               out | trunc
 * w+: 先清空, 再读写, 有必要才创建              in | out | trunc
 *
 * a: 在尾端追加, 文件可以不存在                 out | app
 * a+: 在尾端读写,有必要才创建                   in | out | app
 *
 * c++
 * in out app ate trunc binary
 *
 */

namespace mstream{

    void filetest();

    void filetest1();

    void filetest2();

    void filetest3();

}














#endif //STL_FILEACCESS_H
