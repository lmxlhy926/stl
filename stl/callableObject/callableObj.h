//
// Created by lhy on 2020/12/1.
//

#ifndef STL_CALLABLEOBJ_H
#define STL_CALLABLEOBJ_H

/*
 * callable object
 *      1.一个函数, 接受额外传入的args作为实参
 *      2.一个指向成员函数的指针,当通过对象调用它, 该对象被传递为第一实参(必须是个reference或pointer), 其它实参则一一对应成员函数的参数
 *      3.一个函数对象, 附带的args被传递作为实参
 *      4.一个lamda, 严格的说它是一种函数对象
 */

#include <functional>
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

namespace callobj{

    int add(int a, int b){
        cout << "a + b = " << a + b << endl;
        return a + b;
    }

    auto sub = [] (int a, int b) -> int{
        cout << "a - b = " << a - b << endl;
        return a - b;
    };

    class Multi{
    private:
        int c_;
    public:
        explicit Multi(int c): c_(c){}

        int operator () (int a, int b){
            cout << "a * b * c = " << a * b * c_ << endl;
            return a * b * c_;
        }

        int getValue(int a){
            return a + c_;
        }
    };


    void callobjTest(){
        /*
         *  class std::function<>， 声明于<functional>， 提供多态外覆器，可以概括function pointer记号。
         *  这个class允许你把可调用对象当作最高级对象
         */
        std::vector<std::function<int(int, int)>> task; //存储函数
        task.emplace_back(add);
        task.emplace_back(add);
        task.emplace_back(sub);
        task.emplace_back(Multi(2));

        for(const std::function<int(int, int)>& f : task){
            f(100, 2);
        }

    }

/*
    类型模板参数/非类型模板参数: 既可以传入类型参数，也可以传入非类型参数
    模板参数默认值： 默认的template实参可以其前一个实参为依据而定义


 */
    template<typename T, int loopNumber = 1>    //类型参数， 非类型参数，参数默认值
    class addValue{
    private:
        T theValue;
    public:
        explicit addValue(int value) : theValue(value){}

        template<typename U>    //成员函数模板， 不可以是virtual
        explicit addValue(const addValue<U>& other){
            theValue = other.getValue();
        }

        template<typename U>
        void assign(const addValue<U>& other){  //实参的类型和函数调用对象的类型不同，因此不可以直接访问private和protected成员
            theValue = other.getValue();    //参数声明为const, 只能调用const函数
        }

        void operator() (int& elem){
            for(int i = 0; i < loopNumber; i++){
                elem += theValue;
            }
        }

        T getValue() const{
            return theValue;
        }
    };


    template<typename Iterator, typename Operation>
    void show_each(Iterator begin, Iterator end, Operation opr){
       for(; begin < end; begin++){
           opr(*begin); //函数对象的使用
       }
    }

    void memberTemplateTest(){
        addValue<int> s(20);
        addValue<float> u(30);
        addValue<float> v(s);

        s.assign(u);

        cout << "s.theValue: " << s.getValue() << endl;
        cout << "v.tehValue: " << v.getValue() << endl;
    }


    void functionobjtest(){
       vector<int> v;
       v.reserve(50);
       v.assign({1, 2, 3});

       show_each(v.begin(), v.end(), addValue<int>(10));    //传入一个临时函数对象

       for(const auto& elem : v){
           cout << elem << endl;
       }
    }




}






#endif //STL_CALLABLEOBJ_H
