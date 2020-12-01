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

using namespace std;

namespace callobj{



    int add(int a, int b){
        cout << "a + b = " << a + b << endl;
        return a + b;
    }

    auto l = [] (int a, int b){
        cout << "a - b = " << a - b << endl;
        return a - b;
    };

    class obj{
    private:
        int c_;
    public:
        obj(int c): c_(c){}

        int operator () (int a, int b){
            cout << "a * b = " << a * b * c_ << endl;
            return a * b;
        }

        int div(int a, int b){
            cout << "a / b = " << a / b * c_ << endl;
            return a/b;
        }

        int func1(int a, int b, int c){
            return a + b + c;
        }
    };



    void callobjTest(){

        std::vector<std::function<int(int, int)>> task;
        task.emplace_back(add);
        task.emplace_back(l);
        for(std::function<int(int, int)> f : task){
            f(100,2 );
        }

        obj ob(100);
        std::function<int(obj &, int, int)> mf = &obj::div;
        mf(ob, 100, 2);



    }


}






#endif //STL_CALLABLEOBJ_H
