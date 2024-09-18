
#include <iostream>

using namespace std;
/*
赋值兼容规则：
	在需要基类对象的任何地方都可以使用公有派生类的对象来替代。
		* 派生类的对象可以赋值给基类对象
		* 派生类的对象可以初始化基类的引用
		* 派生类对象的地址可以赋值给指向基类的指针
	在替代之后，派生类对象就可以作为基类的对象使用，但只能使用从基类继承的成员。

*/

/*
多态形成的条件：
	1. 父类中有虚函数
	2. 子类覆写父类中的虚函数
	3. 通过已被子类对象赋值的父类指针或引用，调用公用接口。

*/

/*
虚函数：
	1. 在基类中用virtual声明成员函数为虚函数。类外实现虚函数时，不必再加virtual。
	2. 在派生类中重新定义此函数称为覆写，要求函数名，返回值类型，函数参数个数及类型全部匹配。并根据派生类的需要重新定义函数体。
	3. 当一个成员函数被声明为虚函数后，派生类中对其覆写的函数也为虚函数。可以添加virtual明示。
	4. 定义一个指向基类对象的指针，并使其指向其子类的对象，通过该指针调用虚函数，此时调用的就是该指针变量指向对象的同名函数。
	5. 子类中覆写的函数，可以为任意访类型，依子类需求决定。

*/

/*
遮盖原则：
	派生类的同名成员遮盖基类的同名成员，可以附加类名修饰符来访问基类的成员。

*/


class base
{
public:
	base(){}
	base(int x, int y):_x(x),_y(y){}
	base(const base& another)
	{
		cout << "in base copy constructer" << endl;
		this->_x = another._x;
		this->_y = another._y;
	}
	virtual void dis();
	
protected:
	int _x;
	int _y;
};

void base::dis()
{
	cout << "in base" << endl;
	cout << "x:" << _x << " y:" << _y << endl;
}

/*-------------------------------------------------------------------------------*/


class derive:public base
{
public:
	derive(){}	//默认自动调用基类的空参构造器
	derive(int x, int y, int z):base(x, y),_z(z){}	//显示指定非空参构造器
	derive(const derive& another):base(another)
	{
		cout << "in derive copy constructer" << endl;
		this->_z = another._z;
	}
	
	virtual void dis();
	void dis1()
	{
		cout << "in derive dis1" << endl;
	}
	
protected:
	int _z;
};

void derive::dis()
{
	cout << "in derive dis" << endl;
	cout << "x:" << _x << " y:" << _y << " z:" << _z << endl;
}

/*-------------------------------------------------------------------------------*/


class deriveagain:public derive
{
public:
	deriveagain(){}
	virtual void dis()
	{
		cout << "in deriveagain dis" << endl;
	}
	void dis1()
	{
		cout << "in deriveagain dis1" << endl;
	}
	void distest()
	{
		dis();
		dis1();
	}
};

/*-------------------------------------------------------------------------------*/


void func1();

int main(void)
{
	base b(1,2);
	derive d(3,4,5);
/*
	derive d1;
	d1 = d;	//这是赋值，不会调用拷贝构造函数
*/

/*
	derive d1 = d;	//这里会调用拷贝构造，即拷贝构造是创建一个新的对象。赋值是为已经创建好的对象赋予新值。
*/

	b = d;	//派生类对象赋值给基类对象,调用了赋值操作。这种方式不会形成多态，而是会使子类对象丢弃新增改变，完全变为父类对象。
	b.dis();
	cout << "------" << endl;
	
	base &rs = d;	//派生类对象初始化基类的引用
	rs.dis();
	cout << "------" << endl;

	base *ps = &d;	//派生类对象的地址赋值给指向基类的指针
	ps->dis();
	cout << "------" << endl;


	deriveagain da;
	derive *dapointer = &da;
	dapointer->dis();	//该函数在基类中声明时加virtual修饰，在派生类中覆写该函数。实际调用的是派生类的函数。
	dapointer->dis1();	//该函数在基类中声明时未加virtual修饰，在派生类中覆写该函数。实际调用的是基类的函数。
	cout << "------" << endl;

/*
***结果
	in base
	x:3 y:4
	------
	in derive dis
	x:3 y:4 z:5
	------
	in derive dis
	x:3 y:4 z:5
	------
	in deriveagain dis
	derive dis1
***
*/

	func1();

	return 0;
}

void func1()
{
	deriveagain db;
	db.dis1();
	db.deriveagain::dis1();
	db.derive::dis1();
	db.distest();
	
/*结果
	deriveagain dis1
	deriveagain dis1
	derive dis1
*/
	return;
}
































































