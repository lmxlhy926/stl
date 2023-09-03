
#include <iostream>
using namespace std;

/*
同名成员的隐藏规则:	子类成员隐藏父类成员
父类成员的引用：携带父类作用域限定

*/

/*
多个父类中重名的成员，继承到子类中后，为了避免冲突，携带了各父类的作用域信息，
子类中要访问继承下来的重名成员，则会产生二义性，为了避免冲突，访问时需要还有父类的作用域信息。

*/

class base1
{
public:
	base1():a(1){}
	void func1()
	{
		cout << "base1 func1()" << endl;
	}

public:
	int a;
};

class base2
{
public:
	base2():a(2){}
	void func1()
	{
		cout << "base2 func1()" << endl;
	}
	
public:
	int a;
};

class derive:public base1, public base2
{
public:
	derive():a(3){}
	void func1()
	{	
		base1::func1();
		base2::func1();
		cout << "derive func1()" << endl;
	}

public:
	int a;
};


/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/


class x
{
public:
	x():data(1){}
	void setdata(int a)
	{
		this->data = a;
	}
	
public:
	int data;
};

class y
{
public:
	y():data(2){}
	int getdata()
	{
		return this->data;
	}

public:
	int data;
};

class z:public x, public y
{
public:
	z(){}
	void dis()
	{
		cout << x::data << endl;
		cout << y::data << endl;
	}

};




int main(void)
{
	derive d;
	d.base1::func1();
	d.base2::func1();
	cout << "----" << endl;
	d.func1();
	cout << "----" << endl;
	
	cout << d.base1::a << endl;
	cout << d.base2::a << endl;
	cout << d.a << endl;
	
/*
base1 func1()
base2 func1()
----
base1 func1()
base2 func1()
derive func1()
----
1
2
3
*/

cout << "----" << endl;
cout << "----" << endl;
	z a;
	a.dis();
	a.setdata(10);
	cout << a.getdata() << endl;
/*
1
2
2
*/
	return 0;
};





















































