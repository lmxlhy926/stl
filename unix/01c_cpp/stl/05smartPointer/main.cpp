#include <iostream>
#include <thread>
#include <chrono>
#include <vector>

using namespace std;



/*
 *  1. 使用pointer的理由之一是在惯常的作用域边界之外拥有reference语义。
 *  2. 如何确保“pointer的寿命”和“其所指向的对象的寿命”一致？
 *      shared_ptr: 实现共享式拥有的概念，多个smart pointer可以指向相同的对象，该对象和其相关资源
 *                  会在“最后一个reference被销毁”时释放。
 *
 *      unique_ptr: 实现独占式拥有或严格拥有的概念，保证同一时间内只有一个smart pointer指向该对象，
 *                  可以移交所有权。
 *
 *  所有smart pointer class都被定义于头文件<memory>内
 */


/**
 * shared_ptr<>包含的操作：
 *      1. 指向一个对象，指定对象被释放时调用delete还是delete[]
 *      2. 重新指向一个对象，或者不指向任何对象
 *      3. 判断是否指向一个对象
 *      3. 操作指向的对象
 * 
 * 只有当没有任何shared_ptr<>指向某个对象时，才会释放该对象;
*/


void sharedPtr(){
    //RAII：获取资源即初始化，可以提供析构函数, 默认的析构是调用delete
    std::shared_ptr<string> sptr1(new string("hello"), [](string* p){
        std::cout << "shared_ptr deconstructor delete: " << *p << endl;
        delete p;
    });
    std::cout << "sptr1.use_count(): " << sptr1.use_count() << std::endl;
    std::cout << "--------------------------" << std::endl;

    //复制shared_ptr对象，内部采用引用计数
    std::vector<shared_ptr<string>> vec;
    vec.push_back(sptr1);
    vec.push_back(sptr1);
    std::cout << "sptr1.use_count(): " << sptr1.use_count() << std::endl;
    std::cout << "vec[0].use_count(): " << vec[0].use_count() << std::endl;
    std::cout << "vec[1].use_count(): " << vec[1].use_count() << std::endl;
    std::cout << "--------------------------" << std::endl;

    //像普通pointer一样使用operator*, operator->来访问管理的对象的内容
    //从shared_ptr对象中获取指针
    (*sptr1)[0] = 's';
    (*vec[0])[1] = 'b';
    vec[0]->replace(2, 1, "b");
    std::cout << "content: " << sptr1->c_str() << std::endl;
    std::cout << "content: " << *(vec[0].get()) << std::endl;
    std::cout << "content: " << *(vec[1].get()) << std::endl;
    std::cout << "--------------------------" << std::endl;

    //丧失最后一个引用，没有引用指向该对象，导致对象被析构
    sptr1 = nullptr;
    vec[1].reset();
    std::cout << "sptr1.use_count(): " << sptr1.use_count() << std::endl;
    std::cout << "vec[0].use_count(): " << vec[0].use_count() << std::endl;
    std::cout << "vec[1].use_count(): " << vec[1].use_count() << std::endl;
    std::cout << "--------------------------" << std::endl;

    std::cout << "content: " << *(vec[0].get()) << std::endl;
    std::cout << "content: " << *(vec[1].get()) << std::endl;
    std::cout << "content: " << sptr1->c_str() << std::endl;
}

//数组的析构必须调用delete[]
void arrayDeconstruct(){
    std::shared_ptr<int> sp(new int[10], [](int* p){
        std::cout << "===>array deconstruct" << std::endl;
        delete[] p;
    });
}

/**
 * shared_ptr<>是一个对象，对象内包含的信息指明其是否指向其它对象，以及指向哪个对象。
 * 
 * 
*/
void sharedPtr2(){
    //指定指针对象 + 指定析构函数
    std::shared_ptr<string> sptr{ new string("hello"), [](string* ptr){
        std::cout << "1__shared ptr deconstruct: " << ptr->c_str() << std::endl;
        delete ptr;
    }};

    std::vector<std::shared_ptr<string>> vec;
    vec.push_back(sptr);
    vec.push_back(sptr);

    sptr.reset(new string("world"), [](string* ptr){
        std::cout << "2__shared ptr deconstruct: " << ptr->c_str() << std::endl;
        delete ptr;
    });
    std::cout << "-----------------------" << std::endl;

    //赋值新的对象；会导致先前的对象被释放；使用默认的析构函数
    sptr.reset(new string("again"));
    std::cout << "-----------------------" << std::endl;

    //清空指针；会导致先前的对象被释放；
    sptr.reset();
    sptr.reset();
    if(!sptr){  //是否指向一个对象
        std::cout << "sptr is nullptr" << std::endl;
    }
}


int main(int argc, char* argv[]){

    sharedPtr2();

    return 0;
}























