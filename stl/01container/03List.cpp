//
// Created by 78472 on 2021/11/13.
//

#include <list>
#include <iostream>
#include <iterator>
#include <algorithm>

using namespace std;


/*
 *  双向列表list对象自身提供了2个pointer或称anchor，用来指向第一个或最末一个元素。
 *  list不支持随机访问，要访问第5个元素，就必须顺着串链逐一爬过前4个元素。
 *  由于不支持随机访问，list既不提供subscript操作符，也不提供at()。
 *  特别的list提供了去重，拼接，排序，合并方法。
 */

//打印列表元素
void printLists(std::list<int>& list1, std::list<int>& list2){
    std::cout << "list1: ";
    copy(list1.begin(), list1.end(), ostream_iterator<int>(cout, " "));
    std::cout << endl << "list2: ";
    copy(list2.begin(), list2.end(), ostream_iterator<int>(cout, " "));
    std::cout << endl << endl;
}


void listfunctions(){
    list<int> list1, list2;

    for(int i = 0; i < 6; i++){
        list1.push_back(i);
        list2.push_front(i);
    }
    printLists(list1, list2);

    //将list1插移到list2的值==3的元素前面
    list2.splice(find(list2.begin(), list2.end(), 3), list1);
    printLists(list1, list2);

    //将list2的首元素移到末尾
    list2.splice(list2.end(), list2, list2.begin());
    printLists(list1, list2);

    //将list2排序后赋值给list1，list2相邻相同元素去重
    list2.sort();
    list1 = list2;
    list2.unique();
    printLists(list1, list2);

    //将排序好的list2插移到排序好的list1
    list1.merge(list2);
    printLists(list1, list2);

    //移除满足条件的元素
    list1.remove(0);
    list1.remove_if([](int i){
        return i == 1;
    });
    printLists(list1, list2);
}



int main(int argc, char* argv[]){


    return 0;
}




