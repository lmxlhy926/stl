
#include <iostream>
using namespace std;

/*
1. 含有纯虚函数的类，称为抽象基类，不可实例化。即不能创建对象，存在的意义就是被继承，提供族类的公共接口，java中称为interface。
2. 纯虚函数只有声明，没有实现，被初始化为0.
3. 一个类中声明了纯虚函数，而在派生类中没有对该函数定义，则该函数在派生类中仍然为纯虚函数，派生类仍然为纯虚基类。

*/

/*
虚函数仅适用于有继承关系的类对象，普通函数不能声明为虚函数。
构造函数不能是虚函数。调用构造函数后，对象的创建才算真正的完成。
析构函数可以是虚函数且通常声明为虚函数。

*/


//抽象基类即接口
class shape
{
public:
	shape(){}
	shape(int a, int b):_a(a),_b(b){}
	virtual ~shape() { cout << "in ~shape()" << endl; }	//虚析构函数，基类指针指向派生类对象，删除基类指针时触发动态绑定。
	
	virtual void dis1() = 0;	//纯虚函数，定义接口，派生类来实现该接口
	virtual void dis2() = 0;

protected:
	int _a;
	int _b;
	
};


//子类
class circle:public shape
{
public:
	circle(){}
	circle(int a, int b):shape(a, b){}
	virtual ~circle(){ cout << "in ~circle" << endl;}
	
	virtual void dis1()	//实现抽象类定义的接口
	{
		cout << "a: " << _a << endl;
	}
	
	virtual void dis2()
	{
		cout << "b: " << _b << endl;
	}
	
};


void func1(shape& spe);

int main(void)
{
	circle c(1, 2);
	func1(c);	//实际参数传递的是一个派生类对象，对抽象类的操作转换为对派生类对象的操作
	cout << "----" << endl;
	
/*
	a: 1
	b: 2
	----
	in ~circle
	in ~shape()
*/
	return 0;
}

void func1(shape& spe)	//参数为抽象基类的引用
{
	spe.dis1();	//在函数体里针对抽象类编写逻辑，即面向接口编程。
	spe.dis2();
	return;
}



















