#include <iostream>
#include <cstdio>

void func(int &ref);
void func1(void);
void func2(void);
void func3(void);

int main(void)
{

	func3();
	
	return 0;
}

void func1(void)
{
//静态类型转换：在一个方向上可以做隐式转换，在另一个方向上就可以做静态转换。
	int *a;
	void *p;
	//a = p;	//不可以
	p = a;	//可以
	a = static_cast<int *>(p);
	
	return;
}

void func2(void)
{
//重解释类型转换：通常为操作数的位模式提供较低层的重新解释，在双方向上都不可以做隐式类型转换的，则需要重解释类型转换。
	int x = 0x123456;
	//char *p = static_cast<char*>(&x);	//使用静态类型转换报错
	char *p = reinterpret_cast<char*>(&x);
	printf("%x\n", *p);
	
	// int a[5] = {1,2,3,4,5};	
	// int *q = reinterpret_cast<int*>(reinterpret_cast<int>(a)+1);	//error: cast from ‘int*’ to ‘int’ loses precision [-fpermissive]
	// printf("%d\n", *q);
	
	return;
}

void func3(void)
{
//用来移除对象的常量性，使用const_cast去除const限定的目的不是为了修改它的内容，使用const_cast去除const限定，通常是为了函数能够接受这个实际参数。
	const int m = 444;
	func(const_cast<int &>(m));
	
//不要这样使用，会发生未定义行为。
	const int x = 200;
	int &a = const_cast<int &>(x);
	a = 300;
	std::cout << "x==" << x << " a==" << a << std::endl;
	std::cout << "&x==" << &x << " &a==" << &a << std::endl;
	
	
	
	return;
}


void func(int &ref)
{
	std::cout << ref << std::endl;
	
	return;
}

























