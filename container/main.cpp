
#include <iostream>
#include "mvector.h"
#include "mset.h"
#include "mmap.h"
#include <thread>
#include "marray.h"
#include "mlist.h"
#include "stack_queue.h"

using namespace std;



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

int main(int argc, char* argv[]){

    priorityQueueConstructor();

    while(true)
        std::this_thread::sleep_for(std::chrono::seconds(10));

    return 0;

}










