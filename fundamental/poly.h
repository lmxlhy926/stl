//
// Created by lhy on 2020/11/28.
//

#ifndef STL_POLY_H
#define STL_POLY_H

#include "basic_class.h"

namespace fundamental{

    class poly : public model
    {
    public:
        poly(int a, float b, string cmstr) :model(a, b, cmstr){
            cout << "==>poly constructor" << endl;

        }

        void messageShow() override{
            cout << "---in poly messageShow" << endl;
        }

        ~poly(){
            cout << "==>poly deconstructor" << endl;
        }
    };

    void polyclassTest();
}



#endif //STL_POLY_H
