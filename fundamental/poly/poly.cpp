//
// Created by lhy on 2020/11/30.
//
#include "poly.h"

    void memoryDevice::open() {
        std::cout << "device open" << endl;
    }
    void memoryDevice::read() {
        std::cout << "device read" << endl;
    }
    void memoryDevice::write() {
        std::cout << "device write" << endl;
    }
    void memoryDevice::close() {
        std::cout << "device close" << endl;
    }


    void PolyClassTest::polytest(){
        usbDevice usbDevice;
        diskDevice diskDevice;
        memoryDevice &usb_device_reference = usbDevice;    //派生类对象初始化基类对象的引用
        memoryDevice *disk_device_pointer = &diskDevice;   //派生类对象的地址赋值给指向基类对象的指针

        File::memmoryOperationRef(usbDevice);
        cout << "**********" << endl;
        cout << "**********" << endl;
        File::memmoryOperationPtr(disk_device_pointer);
    }


    void PolyClassTest::destructorPtr(){
        polyBase* basicptr;            //声明为指向基类对象的指针
        basicptr = new polyInherit;    //实际上指向派生类对象
        delete basicptr;               //删除对象会调用析构函数

       /*
        * 删除指针指向的对象，由于是虚析构函数，会触发动态绑定，先调用派生类析构函数
        * 再调用基类析构函数。如果析构函数不是virtual函数，则只会调用基类的析构函数
        * 这样派生类中申请的内存就没有被释放，从而造成内存泄漏。
        */
        cout << "--------" << endl;
    }






