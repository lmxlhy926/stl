
#include <iostream>
#include "mvector.h"
#include "mset.h"
#include "mmap.h"
#include <thread>
#include "marray.h"
#include "mlist.h"

using namespace std;


template<typename T1, typename T2>
class test{

public:
    void copy(T1 s1, T2 s2){
        s1.hello() = "helll";
        s1.world() == s2.ttt();
    }
};



int main(int argc, char* argv[]){

//    string add = "add";
//    string sub = "sub";
//    string mul = "mul";
//
//    container::mmap_functor(1, 2, add);
//    container::mmap_functor(1, 2, sub);
//    container::mmap_functor(1, 2, mul);

    container::set_comp_own();


    while(true)
        std::this_thread::sleep_for(std::chrono::seconds(10));

    return 0;

}



/*

容器：
	0. 容器元素
	1. 创建一个容器（空，初值列，拷贝/move构造，指定区间）：空，指定序列，其它容器的全部或者部分
	 . 赋值
	 . 交换
	2. 插入元素，删除元素，引用元素，查找元素，清空容器
	3. 属性，比较
	4. 容器元素的遍历


序列式容器：连续内存
关联式容器：不连续内存




 */







