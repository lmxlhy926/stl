
#include <iostream>
#include "poly.h"

using namespace fundamental;

/*
 * 面向接口编程
 * 逻辑针对接口类进行，但是实际传递的是接口实现类的对象
 * 基类引用指向派生类的对象
 * 基类指针指向派生类的对象
 */

void memmoryOperationRef(memoryDevice& file){
    file.open();
    file.read();
    file.write();
    file.close();
    file.printMessage();
}

void memmoryOperationPtr(memoryDevice* file){
    file->open();
    file->read();
    file->write();
    file->close();
    file->printMessage();               //如果对象是派生类，优先调用派生类的函数
    cout << "************" << endl;
    file->memoryDevice::printMessage(); //明确指定调用基类的函数，多态失效
}


void polytest(){
    usbDevice usbDevice;
    diskDevice diskDevice;
    memoryDevice &usb_device_reference = usbDevice;    //派生类对象初始化基类对象的引用
    memoryDevice *disk_device_pointer = &diskDevice;   //派生类对象的地址赋值给指向基类对象的指针

    memmoryOperationRef(usbDevice);
    cout << "**********" << endl;
    cout << "**********" << endl;
    memmoryOperationPtr(disk_device_pointer);
}


/*
 * 删除指针指向的对象，由于是虚析构函数，会触发动态绑定，先调用派生类析构函数
 * 再调用基类析构函数。如果析构函数不是virtual函数，则只会调用基类的析构函数
 * 这样派生类中申请的内存就没有被释放，从而造成内存泄漏。
 */
void destructorPtr(){
    polyBase* basicptr;            //声明为指向基类对象的指针
    basicptr = new polyInherit;    //实际上指向派生类对象
    delete basicptr;               //删除对象会调用实际对象的析构函数
}


int main(int argc, char* argv[]){


    return 0;
}










