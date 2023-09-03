
#include <iostream>

using namespace std;

/*
继承：实现可重用性
继承：新的类从已有的类那里得到已有的属性。或者说，从已有类产生新类的过程叫做派生。
类的称呼：基类/父类，派生类/子类

*/

/*
派生类中从基类继承而来的成员的初始化工作还是由基类的构造函数完成，派生类新增的成员在派生类的构造函数中初始化。
派生类构造函数执行顺序：先执行基类构造函数-->再执行子类构造函数
	1. 调用基类构造函数时，调用顺序按照它们被继承时的声明顺序（从左到右, 多继承情形下）
	2. 如果基类中没有默认构造参数(无参)，那么在派生类的构造函数中必须显示调用基类构造函数，以初始化基类成员。
	3. 调用内嵌成员对象的构造函数，调用顺序按照它们在类中的声明顺序。


*/


class A
{
public:
	A()
	{
		cout << "in A" << endl;
		cout << this << endl;	//显示的是该对象的地址
	}
	int a;
};

class B:public A
{
public:
	B()
	{
		cout << "in B" << endl;
		cout << this << endl;
	}
	int b;
};

class C:public B
{
public:
	C()
	{
		cout << "in c" << endl;
		cout << this << endl;	//显示的是该对象的地址
	}
	void dis()
	{
		cout << "----" << endl;
		cout << "&a " << &a << endl;
		cout << "&b " << &b << endl;
		cout << "&c " << &c << endl;
	}
	int c;
};


int main(void)
{
	C c;
	c.dis();	//自动调用基类的默认构造器
	
/*		
	in A
	0x7ffc6ca4cf50
	in B
	0x7ffc6ca4cf50
	in c
	0x7ffc6ca4cf50
	----
	&a 0x7ffc6ca4cf50
	&b 0x7ffc6ca4cf54
	&c 0x7ffc6ca4cf58
*/

	return 0;
};
























































