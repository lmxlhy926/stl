
#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace std;

/*
	const函数可以构成重载，const对象只能调用const函数，非const对象优先调用非const函数。
	const函数只能调用const函数，非const函数可以调用const函数。
	类体外定义的const成员函数，在定义和声明处都需要const修饰符。
*/

/*
	const对象，只能调用const成员函数。
	可以访问const或非const数据成员，不能修改。
*/

class A
{
private:
	const int a;
	int b;
public:
	A():a(100),b(200){}
	void dis() const
	{
		printf("const\n");
	}
	void dis()
	{
		printf("no const\n");
	}
};


class B
{
public:
	int a;
	
	B():a(10){}
	void dis() const
	{
		printf("%d\n", a);
	}
};



int main(void)
{
	
	const A a;		//没有参数时不要加()。
	A b;
	a.dis();
	b.dis();

	const B c;
	cout << c.a << endl;

	return 0;
}



















