
#include <iostream>
using namespace std;
/*
虚继承：
	主要用来解决多继承时可能发生的对同一基类继承多次而产生的二义性问题。
	为最远的派生类提供唯一的基类成员，而不重复产生多次拷贝

*/


class m
{
public:
	m(int i):data(i){
		cout << "m()" << endl;
	}
	int data;
	
	virtual void display()
	{
		cout << "m dispaly" << endl;
	}
};

class x:virtual public m
{
public:
	x(int i):m(i){
		cout << "x()" << endl;
	}
	void setdata(int i)
	{
		data = i;
	}
	virtual void display()
	{
		cout << "x display" << endl;
	}
};

class y:virtual public m
{
public:
	y(int i):m(i){
		cout << "y()" << endl;
	}
	int getdata()
	{
		return data;
	}
	virtual void display()
	{
		cout << "y display" << endl;
	}
};

class z:public x,public y
{
public:
	z(int i, int n):x(i),y(n),m(1200){	//要调用共同基类的构造器
		cout << "z()" << endl;
	}
	void dis()
	{
		cout << "x::data " << x::data << endl;
		cout << "y::data " << y::data << endl;
		cout << "m::data " << m::data << endl;
	}
	virtual void display()
	{
		m::display();
		x::display();
		y::display();	//父类继承的函数的调用方法
		cout << "z display" << endl;
	}
};



int main(void)
{
	
	z zz(3,4);
	zz.dis();
	zz.setdata(100);
	zz.dis();
	cout << "---" << endl;
//	zz.m::display();
//	zz.x::display();
//	zz.y::display();
	zz.display();

	
//	m()
//	x()
//	y()
//	z()	//只调用了一次共同基类构造器
//	x::data 1200
//	y::data 1200
//	m::data 1200
//	x::data 100
//	y::data 100
//	m::data 100	//3种访问方式指向的是同一个对象
//	---
//	m dispaly
//	x display
//	y display
//	z display
	
	return 0;
}






































































