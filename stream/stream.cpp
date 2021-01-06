//
// Created by lhy on 2021/1/5.
//

#include <iostream>
#include <limits>

using namespace std;

namespace mstream{


/*
 * 如果设置了failbit, 除非明确予以清除, 否则无法进入下一个操作.
 */
    void statetest(){

        int k{100};

        if(!(std::cin >> k))
            cout << "error happens" << endl;
        cout  << "k: " << k << endl;

        if(cin.rdstate() & std::ios::eofbit){
            cout << "eofbit was set" << endl;

        }else if(cin.rdstate() & std::ios::failbit){
            cout << "failbit was set" << endl;

        }else if(cin.rdstate() & std::ios::badbit)
            cout << "badbit was set" << endl;

        std::cin.clear();
        if(std::cin.good())
            cout << "清除所有errors" << endl;

    }


/*
I/O相关函数
    功能：读取一个或多个字符,将其返回或存储到指定的变量中
    读取停止位置： 由个数或delim字符限定
    结尾null字符： 末尾有的主动添加null字符有的不添加
    delim字符：有的不读取delim字符即下一个字符是delim则停止读取, 有的会读取delim字符但是会将其舍弃掉, 不会将其存储到变量中
*/
    void inputfunctest(){   //测试字符序列： 12aaaqqqwwww

        int a = std::cin.get();     //读取下一个字符
        cout << "a: " << a << endl;

        char c;
        if(std::cin.get(c)){    //读取下一个字符到指定的变量中
            cout << "c数字: " << static_cast<int>(c) << endl;
            cout << "c字符: " << c << endl;
        }

        char buf[20]{};
        if(std::cin.get(buf, 20, 'q')){  //读取count-1个字符或下一个字符是delim则停止, 结尾添加null字符, 不读取delim字符
            cout << "buf: " << buf << endl;
        }

        char buf1[20]{};
        if(std::cin.getline(buf1, 20 , 'w')){   //其它的和上面的get一样, 例外：读取delim字符但是会直接丢弃不会存储
            cout << "buf1: " << buf1 << endl;
        }

        char buf2[10]{};
        if(std::cin.read(buf2, 3)){  //读取count个字符存入str中, 末尾不会添加null字符. 因确保str有足够的空间, 而且要关注str内的内容。
            cout << "buf2: " << buf2 << endl;
        }
    }

/*
舍弃字符：
    功能：提取字符并舍弃不用
    舍弃停止位置：由个数或者delim字符限定
    delim字符：如果在个数限定之前遇到delim字符, delim字符会被提取并舍弃
 */
    void inputfunctest1(){
        char buf[3]{}, buf1[10]{};
        std::cin.ignore(3);     //提取并舍弃3个字符
        std::cin.get(buf, 3);
        std::cin.ignore(numeric_limits<std::streamsize>::max(), '\n');  //舍弃这一行的剩余字符,包括'\n'
        std::cin.get(buf1, 10);
        cout << "buf: " << buf << endl;
        cout << "buf1: " << buf1 << endl;
    }

/*
放回字符：
    可放回的最大字符数，由实现版本决定. c++标准保证在2次读取之间允许调用该函数一次, 并具有可携性。
 */
    void inputfunctest2(){
        char buf[10]{};

        int a = std::cin.peek();  //返回stream内"下一个将被读取的字符", 但是不真的将它读取出来.
        int b = std::cin.peek();
        int c = std::cin.get();

        std::cin.putback('a').putback('b').putback('c');    //将字符放回stream, 后放置的字符在前面
        std::cin.get(buf, 10);
        if(!std::cin)
            cout << "error happens" << endl;
        cout << "a: " << a << endl << "b: " << b << endl << "c: " << c << endl << "buf: " << buf << endl;

        std::cin.unget();   //将上一次读取的最后一个字符放回stream中.
        int d = std::cin.get();
        cout << "d: " << d << endl;
    }


}