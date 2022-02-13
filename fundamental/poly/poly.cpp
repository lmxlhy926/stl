//
// Created by lhy on 2020/11/30.
//
#include "poly.h"


    void PolyClassTest::test(){
        usbDeviceOperation usbDevice;
        diskDeviceOperation diskDevice;
        deviceOperation &usb_device_reference = usbDevice;    //派生类对象初始化基类对象的引用
        deviceOperation *disk_device_pointer = &diskDevice;     //派生类对象的地址赋值给指向基类对象的指针

        File::fileReference(usbDevice);
        cout << "**********" << endl;
        cout << "**********" << endl;
        File::filePointer(&diskDevice);
    }

    void PolyClassTest::test1(){
        virtualDestructor* basicptr;    //声明为指向基类对象的指针
        basicptr = new inheritFromVirtualDestructor;    //实际上指向派生类对象
        delete basicptr;    //删除对象会调用析构函数
       /*
        * 删除指针指向的对象，由于是虚析构函数，会触发动态绑定，先调用派生类析构函数
        * 再调用基类析构函数。如果析构函数不是virtual函数，则只会调用基类的析构函数
        * 这样派生类中申请的内存就没有被释放，从而造成内存泄漏。
        */
        cout << "--------" << endl;
    }








