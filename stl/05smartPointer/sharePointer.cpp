//
// Created by 78472 on 2022/1/16.
//
#include "sharePointer.h"
#include <memory>
#include <string>
#include <iostream>
#include <utility>
#include <vector>

using namespace std;


/**
 * 可以像使用其它poitner一样地使用shared_ptr
 */
void base(){
    //RAII：获取资源即初始化，可以提供析构函数, 默认的析构是调用delete
    std::shared_ptr<string> sptr1(new string("hello"), [](string* p){
        std::cout << "delete: " << *p << endl;
        delete p;
    });
    std::cout << "sptr1.use_count(): " << sptr1.use_count() << std::endl;

    //为新对象（以传入的实参为初值）建立一个shared pointer
    std::shared_ptr<string> sptr2 = std::make_shared<string>("world");

    //复制shared_ptr对象，内部采用引用计数
    std::vector<shared_ptr<string>> vec;
    vec.push_back(sptr1);
    vec.push_back(sptr1);
    vec.push_back(sptr2);
    std::cout << "sptr1.use_count(): " << sptr1.use_count() << std::endl;

    //像普通pointer一样使用operator *, operator ->来访问管理的对象的内容
    (*sptr1)[0] = 's';
    sptr1->replace(1,1,"b");

    //更改引用指向的对象，每个引用都指向被改动后的对象
    cout << "first element of vec: " << *(vec[0].get()) << endl;
    cout << "second element of vec: " << vec[1]->c_str() << endl;

    //删除vector中存储的对象
    vec.clear();
    cout << "===>vec.clear()" << endl;
    //丧失最后一个引用，没有引用指向该对象，导致对象被析构
    sptr1 = nullptr;
    std::cout << "sptr1.use_count(): " << sptr1.use_count() << std::endl;
}


//数组的析构必须调用delete[]
void arrayDeconstruct(){
    std::shared_ptr<int> sp(new int[10], [](int* p){
        std::cout << "===>array deconstruct" << std::endl;
        delete[] p;
    });
}

/*
 * weak_ptr: 共享但是不拥有对象
 *      1. 一旦最末一个拥有该对象的shared pointer失去了拥有权，任何weak pointer都会自动变为空。
 *      2. 不能使用*和->访问weak_ptr指向的对象，而是必须另外建立一个shared pointer.
 *
 */
class Person{
public:
    string name;
    shared_ptr<Person> mother;
    shared_ptr<Person> father;
    std::vector<shared_ptr<Person>> kids;
public:
    explicit Person(string  n,
                    shared_ptr<Person> m = nullptr,
                    shared_ptr<Person> f = nullptr)
             : name(std::move(n)), mother(std::move(m)), father(std::move(f)){}

    ~Person(){
        cout << "delete " << name << std::endl;
    }
};

//初始化mom, dad, kid, 初始化完成后，父母和孩子已经相互指向
shared_ptr<Person> initFamily(const string& name){
    shared_ptr<Person> mom(new Person(name + "'s mom"));
    shared_ptr<Person> dad(new Person(name + "'s dad"));
    //孩子指向mom, dad
    shared_ptr<Person> kid(new Person(name, mom, dad));
    //mom,dad指向kid
    mom->kids.push_back(kid);
    dad->kids.push_back(kid);

    return kid;
}


void cyclicReference(){
    shared_ptr<Person> kid = initFamily("nico");
    std::cout << "kid.use_count: " << kid.use_count() << std::endl;
    //因为互相指向，kid, mom，dad对象都不会被析构.且没有外部reference指向它们，造成内存泄漏
    kid = initFamily("jim");
    std::cout << "jim family exists" << std::endl;
}


class PersonWeak{
public:
    string name;
    shared_ptr<PersonWeak> mother;
    shared_ptr<PersonWeak> father;
    std::vector<weak_ptr<PersonWeak>> kids;
public:
    explicit PersonWeak (string  n,
                    shared_ptr<PersonWeak> m = nullptr,
                    shared_ptr<PersonWeak> f = nullptr)
            : name(std::move(n)), mother(std::move(m)), father(std::move(f)){}

    ~PersonWeak(){
        cout << "delete " << name << std::endl;
    }
};

shared_ptr<PersonWeak> initFamilyWeak(const string& name){
    shared_ptr<PersonWeak> mom(new PersonWeak(name + "'s mom"));
    shared_ptr<PersonWeak> dad(new PersonWeak(name + "'s dad"));
    //孩子指向mom, dad
    shared_ptr<PersonWeak> kid(new PersonWeak(name, mom, dad));
    //mom,dad指向kid
    mom->kids.push_back(kid);
    dad->kids.push_back(kid);

    return kid;
}


void cyclicReferenceWeak(){
    shared_ptr<PersonWeak> kid = initFamilyWeak("nico");
    std::cout << "kid.use_count: " << kid.use_count() << std::endl;

    //kid被析构导致mom,dad都被析构
    kid = initFamilyWeak("jim");
    std::cout << "jim family exists" << std::endl;
}

void test2(){
    try{
        shared_ptr<string> sp(new string("newstring"), [](string* sp){
            cout << "delete: " << *sp << endl;
            delete sp;
        });
        weak_ptr<string> wp = sp;
        sp.reset();
        cout << "wp.use_count: " << wp.use_count() << endl;
        cout << std::boolalpha << wp.expired() << endl;
        shared_ptr<string> p(wp);

    }catch(const std::exception& e){
        cout << "exception: " << e.what() << endl;
    }
}

























