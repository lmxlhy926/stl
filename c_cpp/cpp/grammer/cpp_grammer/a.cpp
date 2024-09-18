
/*
引用：为已有的变量起一个别名
	1.引用没有定义，是一种关系型声明。声明它和原有某一变量的关系。故而类型和原类型保持一致，而不分配内存。与被引用的变量具有相同的地址。
	2.声明的时候必须初始化，一经声明，不可变更。
	3.可对引用再次引用。多次引用的结果，是某一变量具有多个别名。
	
	*const对象的引用必须是const的，将普通变量绑定到const对象是不合法的。
	*const引用可使用相关类型的对象（常量，非同类型的变量或表达式）初始化。

*/



#include <iostream>
#include <stdio.h>

using namespace std;

void swap(int &a, int &b);
void func1(void);
void func2(void);


int main(void)
{
	long a;
	long &r = a;
	printf("%ld\n", sizeof(r));



	return 0;
}


void swap(int &a, int &b)
{
	int temp;
	temp = a;
	a = b;
	b = temp;

	return;
}

void func1(void)
{
	int a = 2, b = 3;
	cout << "a:" << a << " b:" << b << endl;
	swap(a, b);
	cout << "a:" << a << " b:" << b << endl;
	
	return;
}

void func2(void)
{
	double val = 3.14;
	const int &ref = val;	//给常引用赋值
	double &ref2 = val;	//ref2是对val的引用
	cout << "ref:" << ref << "  ref2:" << ref2 << endl;
	val = 4.14;	//val发生了变化，因此ref2的值也发生了变化
	cout << "ref:" << ref << "  ref2:" << ref2 << endl;
	
	return;
}








































