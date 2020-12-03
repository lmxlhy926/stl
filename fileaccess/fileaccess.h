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
 * 如果一定要转换, 必须进行一个seek动作, 到达当前位置, 在转换读写属性
 *
 * 执行文件的打开动作后, 成功与否会反映在stream状态中, 因该在构造stream后检查其状态
 *
 */


/*
 * 打开文件的模式
 * r: 读取, 文件必须存在
 * w: 清空而后涂写, 文件可以不存在
 * a: 追加, 文件可以不存在
 *
 * r+: 读和写, 最初位置在起点, 文件必须存在
 * w+: 先清空, 再读写, 有必要才创建
 * a+: 在尾端读写,有必要才创建
 *
 */

namespace fileaccess{

    void func1();

    void func3();

    void func4();

}














#endif //STL_FILEACCESS_H
