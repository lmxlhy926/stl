
#include <vector>
#include <list>
#include <set>
#include <iostream>
#include <algorithm>
#include <iterator>

using namespace std;


template<typename conType>
void PRINT_ELEMENTS(conType& cont){
    for(auto &elem : cont){
        std::cout << elem << " ";
    }
    std::cout << std::endl;
}


/**
 * output迭代器：
 *      output迭代器允许一步一步前行并搭配write动作。
 *      不保证将一个value赋值2次而其迭代器却不累进
 *      operator*只有在赋值语句的左边才有效。
 *      
 *          *iter=val       :将val写至迭代器所指位置
 *          ++iter          :向前步进，返回新位置
 *          iter++          :向前步进，返回旧位置
 *          TYPE(iter)      :复制迭代器
 * 
 * insert迭代器：
 *      insert迭代器，也称为inserter，用来将“赋予新值”动作转换为“安插新值”动作。借由这种迭代器，算法将执行安插行为而非覆写行为。
 *      所有insert迭代器都隶属于output迭代器种类，所以它只提供赋值能力。
 *          *iter           :No-op(返回iter)
 *          iter = value    :安插value
 *          ++iter          :No-op(返回iter)
 *          iter++          :No-op(返回iter)
 * 
*/


/**
 * back insert iterator借由成员函数push_back()将一个value附加于容器尾部。
 * push_back()只存在于vector, deque, list, string
 * Back inserter生成时，其初始化过程必须确知其所属容器。函数back_inserter()为此提供了一条捷径。
 * 
 * Back inserter对象中包含容器的引用，通过使用Back inserter对象的操作符，来间接操作容器。
 * 主要的应用是和算法结合在一起，改变算法的效果，将赋值操作变为插入操作。
 * 这也体现了类/函数模板+类操作符的强大。模板固定形式，操作符的形式相同但实际操作可以自定义。
*/
void back_insert_iterator_test(){
    vector<int> coll;
    back_insert_iterator<vector<int>> iter(coll);

    //直接使用类对象，调用对象的操作符
    *iter = 1;
    iter++;
    *iter = 2;
    iter++;
    *iter = 3;
    iter++;
    (iter = 4) = 5;
    PRINT_ELEMENTS(coll);

    //快捷方式，返回一个back_insert_iterator。使用了函数模板，可以不用输入容器类型。
    back_inserter(coll) = 44;
    back_inserter(coll) = 55;
    PRINT_ELEMENTS(coll);

    //使用back_inserter安插元素时，可能会造成指向该vector的其它迭代器失效。
    //因此，如果不保留足够空间，这个算法可能会形成“源端迭代器失效”状态。
    coll.reserve(2 * coll.size());
    std::cout << "coll.capacity(): " << coll.capacity() << std::endl;
    auto begin = coll.begin();
    copy(begin, coll.end(), back_inserter(coll));
    PRINT_ELEMENTS(coll);

    //迭代器是一个对象，该对象中包含了容器中元素的位置信息。如果容器没有发声realloc，则迭代器对象指示的位置信息依然是有效的。
    for(auto pos = begin; pos != coll.end(); ++pos){
        std::cout << *pos << " ";
    }
    std::cout << endl;
}


/**
 * front insert iterator是借由成员函数push_front()将一个value加于容器头部。
 * push_front()只存在于 deque，list, forward list
*/
void front_insert_iterator_test(){
    list<int> coll;
    front_insert_iterator<list<int>> iter(coll);

    *iter = 1;
    iter++;
    *iter = 2;
    iter++;
    *iter = 3;
    iter++;
    (*iter = 4) = 5;
    PRINT_ELEMENTS(coll);

    front_inserter(coll) = 44;
    front_inserter(coll) = 55;
    PRINT_ELEMENTS(coll);

    //list通过链式结构将分配的内存串联起来，不会发生realloc
    copy(coll.begin(), coll.end(), front_inserter(coll));
    PRINT_ELEMENTS(coll);
}



/**
 * 从指定的位置开始进行插入
 * 
 * general insert iterator，根据2个实参完成初始化：容器，待安插位置。迭代器内部以“待安插位置”为实参调用成员函数insert()。
 * General inserter对所有标准容器均适用(只有array和forward list除外)，因为那些容器都提供有insert()成员函数。
 * 
 * 安插动作完成后，general inserter获得刚被安插的那个元素的位置。调用赋值相当于调用以下语句
 *      pos = containter.insert(pos, value);
 *      ++pos;
 *      为什么要将insert()的返回值赋予pos呢？这是为了确保该迭代器的位置始终有效。如果没有这一赋值动作，在deque，vector,和string中，
 *      该general inserter本身可能会失效。因为每一次安插动作都会(或至少可能会)使指向容器的所有迭代器失效。
*/
void insert_iterator_test(){
    //容器是关联式容器，传入的位置并不起作用
    set<int> coll;
    insert_iterator<set<int>> iter(coll, coll.begin());

    *iter = 3;
    iter++;
    *iter = 2;
    iter++;
    *iter = 1;
    iter++;
    PRINT_ELEMENTS(coll);

    inserter(coll, coll.end()) = 55;
    inserter(coll, coll.end()) = 44;
    PRINT_ELEMENTS(coll);

    //容器是序列式容器，传入的位置起作用
    list<int> coll2;
    copy(coll.begin(), coll.end(), inserter(coll2, coll2.begin()));
    PRINT_ELEMENTS(coll2);

    copy(coll.begin(), coll.end(), inserter(coll2, ++coll2.begin()));
    PRINT_ELEMENTS(coll2);
}

/**
 * vector内部申请了一片内存空间来保存数据，当插入的数据超过该内存片大小，则会发声realloc。
 * 当发生realloc时，数据的存储位置发生变换，之前的所有iterator全部失效。
*/
void iterator_invalid(){
    //内存大小为2
    std::vector<int> coll;
    coll.reserve(2);
    std::cout << coll.capacity() << std::endl;

    std::vector<int>::iterator pos = coll.begin();

    coll.insert(coll.begin(), 1);
    std::cout << "*pos: " << *pos << std::endl;
    ++pos;

    coll.insert(coll.end(), 2);
    std::cout << "*pos: " << *pos << std::endl;
    ++pos;

    //此次插入会发生realloc，pos失效
    coll.insert(coll.end(), 3);
    std::cout << "*pos: " << *pos << std::endl;
    ++pos;

    //每次插入都会发生有效的迭代器，即使发生realloc依然会返回有效的内存位置
    auto posValuable = coll.insert(coll.end(), 4);
    posValuable++;
    for(auto pos = coll.begin(); pos != posValuable; ++pos){
        std::cout << *pos << std::endl;
    }
}

/**
 * Stream迭代器
 *      stream迭代器是一种迭代器适配器，借由它，你可以把stream当成算法的来源端和目的端。
 *      更明确的说，一个istream迭代器可用来从input stream读取元素，而一个ostream迭代器可以用来对output stream写入元素。
 * 
 * 
*/
void ostream_iteraotr_test(){
    ostream_iterator<int> intWriter(cout, "\n");
    *intWriter = 1;
    intWriter++;
    *intWriter = 2;
    intWriter++;
    *intWriter = 3;
    intWriter++;
   
    vector<int> coll{1, 2, 3, 4, 5, 6, 7, 8, 9};
    copy(coll.begin(), coll.end(), ostream_iterator<int>(cout));
    cout << endl;

    copy(coll.begin(), coll.end(), ostream_iterator<int>(cout, " < "));
    cout << endl;
}


void istream_iterator_test(){
    istream_iterator<int> intReader(cin);
    istream_iterator<int> intReaderEOF;

    while(intReader != intReaderEOF){
        cout << "once: " << *intReader << endl;
        cout << "once again: " << *intReader << endl;
        ++intReader;
    }
}



int main(int argc, char* argv[]){
    istream_iterator_test();


    return 0;
}






















