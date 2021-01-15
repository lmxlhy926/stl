//
// Created by lhy on 2020/12/3.
//

#include <fstream>
#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>

using namespace std;

namespace mstream{

/*
文件的输入/输出
    打开文件, 判断打开是否成功
    从文件读取信息
    向文件写入信息
 */
    void filetest(){

       std::ifstream ifs("/home/lhy/project/learnrepository/stl/stream/input.txt"); //创建对象时, 构造函数里会打开文件
       char c;
       if(!ifs){    //判断文件是否成功打开
          cout << "wrong in open file" << endl;
          exit(-1);
       }

       while(ifs.get(c))    //从文件中逐个读取字符, 直到读取到文件末尾, 置位eofbit和failbit.
           cout.put(c);

       ifs.clear();     //读到文件末尾时会设置eofbit, 如果要对stream执行后续操作, 必须清除错误标志, 否则操作无法进行
       ifs.seekg(0, std::ios::beg);     //将位置设置回文件开头
       cout << ifs.rdbuf() << endl;     //再次将文件内容打印出来


       std::ofstream ofs("/home/lhy/project/learnrepository/stl/stream/output.txt");  //打开文件用于输出
       if(!ofs){    //判断文件是否成功打开
           cout << "wrong in open file" << endl;
           exit(-1);
       }

       for(int i = 49; i < 54; i++){
           ofs << i << endl;    //向stream中写入内容
           ofs << static_cast<char>(i) << endl;
           ofs << "---------" << endl;
       }
    }


/*
Rvalue和Move语义
    可以使用临时创建的stream对象
    可以传入一个file stream作为函数实参, 或让某个函数返回一个file stream.

用于打开或关闭文件的成员函数
    如果file stream创建时未被初始化, 后续可以显示调用这些成员函数
 */
    std::ofstream openfile(const string& filename){
        std::ofstream ofs(filename);
        return ofs;
    }

    void outputtofile(std::ofstream ofs, const string str){
        ofs << str << endl;
    }

    void filetest1(){
        std::ofstream ofs;
        ofs = openfile("/home/lhy/project/learnrepository/stl/stream/output.txt");
        ofs << "hello world" << endl;
        ofs.close();

        std::ofstream("/home/lhy/project/learnrepository/stl/stream/output.txt", std::ios::app) << "hello" << endl;

        std::ofstream ofs1;
        ofs1.open("/home/lhy/project/learnrepository/stl/stream/output.txt", std::ios::app);
        if(ofs1.is_open())
            outputtofile(std::move(ofs1), "aaa");
    }


/*
位置
    tellg()                 获取当前位置
    seekg(pos)              位置类型：std::ios::pos_type     设置绝对位置
    seekg(offset, rpos)     使用整数作为偏移值                设置相对位置
 */
    void filetest2(){
        std::ifstream ifs("/home/lhy/project/learnrepository/stl/stream/input.txt");
        char buf[10]{};
        std::ios::pos_type pos;

        cout << "pos: " << ifs.tellg() << endl; //最开始的位置
        ifs.get(buf, 3);    //从stream中读取2个字符
        cout << "pos: " << ifs.tellg() << endl; //显示当前位置

        ifs.seekg(0, std::ios::beg);    //重新定位到stream开头
        ifs.get(buf + 2, 3);       //从stream中再次读取2个字符
        cout << "pos: " << ifs.tellg() << endl; //显示当前位置

        ifs.seekg(-2, std::ios::end);
        ifs.get(buf + 4, 3);
        cout << "pos: " << ifs.tellg() << endl;

        cout << "buf: " << buf << endl;
    }


/*
打开模式
    起始位置：
        r           in
        r+          in | out

    先清空：
        w           out 或 out | trunc
        w+          in | out | trunc

    尾端添加：
        a           app 或 out | app
        a+          in | app 或 in | out | app

    可读可写时, 当切换读写时必须调用一个seek操作
 */
    void filetest3(){
        std::fstream fs("/home/lhy/project/learnrepository/stl/stream/inout.txt", std::ios::in | std::ios::out); //起始位置读写
        char buf[10]{};

        fs << "--" << std::flush;
        fs.close();

        std::fstream fs1("/home/lhy/project/learnrepository/stl/stream/inout.txt", std::ios::in | std::ios::app); //尾端读写
        fs1 << "--" << std::flush;
        fs1.close();

        std::this_thread::sleep_for(chrono::milliseconds(5000));
        std::fstream fs2("/home/lhy/project/learnrepository/stl/stream/inout.txt", std::ios::out | std::ios::in | std::ios::trunc); //删除, 读写
        fs2 << "****";

        fs2.seekg(0, std::ios::beg);
        fs2.get(buf, 10);
        cout << buf << std::flush << endl;
    }

}
