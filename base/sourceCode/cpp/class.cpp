

#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace std;


/*
参数的默认值
*/
class stack
{
private:
	int *n;
public:
	stack(int size = 100);		//参数默认值，在声明时使用该形式
	~stack();
};

stack::stack(int size)		//函数定义
{
	n = new int[size];
	cout << "size:" << size << endl;
}

stack::~stack()
{
	delete []n;
}





/*
类的构造函数的初始化
初始化列表中的初始化顺序，与声明顺序有关，与前后赋值顺序无关。

*/
class A
{
private:
	
	string name;
	int len;
	
public:
	A(char *str):name(str),len(strlen(name.c_str())){}
	void dis()
	{
		cout << name << endl;
		cout << len << endl;
	}


};


/*
拷贝构造器：利用已有的对象创建新的对象
	1. 参数为对象
	2. 对象的赋值运算
*/
class B
{
private:
	int d;	//没有初始化时，值是不确定的
	char *data;
	
public:
	B(char *str, int num)
	{
		data = new char[strlen(str) + 1];
		strcpy(data, str);
	}
	~B()
	{
		delete []data;
	}

	B(const B &another)
	{
		cout << "kaobeigouzao" << endl;
		data = new char[strlen(another.data) + 1];
		strcpy(data, another.data);
		d = 100;
	}

	void dis()
	{
		cout << d << endl;
		cout << data << endl;
	}


};


/*
this指针
	1. 避免入参和成员名相同
	2. 连续调用
*/
class C
{
private:
	string name;
	int age;
public:
	C(string name, int age)
	{
		this->name = name;
		this->age = age;
	}

	C &addAge()
	{
		this->age++;
		return *this;
	}

	void dis()
	{
		cout << age << endl;
	}

};



int main(void)
{

/*
	stack s;
	stack s1(10);
*/


/*
	A a("china");
	a.dis();
*/


/*
	B a("china", 10);
	a.dis();
	cout << endl;
	
	B b(a);		//调用拷贝构造
	b.dis();
	cout << endl;
	
	B c = a;	//调用拷贝构造
	c.dis();
*/

	C a("sssl", 20);
	a.dis();
	a.addAge().addAge().addAge();
	a.dis();

	return 0;
}

















































