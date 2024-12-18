

#include <iostream>

using namespace std;
class A
{
public:
	A()
	{
		cout << this << "  constructor  " << endl;
	}
	
	A(const A &another)
	{
		cout << this << "  cp constructor from  " << &another << endl;
	}

	A & operator=(const A &other)
	{
		cout << this << "  operator =  " << &other << endl;
	}

	~A()
	{
		cout << this << "  destructor  " << endl;
	}

public:
	int a;
};



#if 0
void foo(A a)
{

}

int main(void)
{
	A a;	//构造
	foo(a);	//拷贝构造, foo()函数的参数

#if 0	
	0x7ffc45ebe260  constructor  
	0x7ffc45ebe270  cp constructor from  0x7ffc45ebe260
	0x7ffc45ebe270  destructor  
	0x7ffc45ebe260  destructor
#endif

	return 0;
}
#endif


#if 0
void foo(A &a){
	cout << "address:" << &a << endl;
}
int main(void)
{
	A a;	//构造
	foo(a);	//普通赋值

#if 0	
	0x7ffd116f0910  constructor  
	address:0x7ffd116f0910
	0x7ffd116f0910  destructor  
#endif
	return 0;
}
#endif


#if 0
A foo(A &a)
{
	return a;
}
int main(void)
{
	A a;	//构造
	foo(a);	//main中的临时空间地址传到foo函数栈中，将返回的对象拷贝到临时空间中。

#if 0
	0x7ffd24d4f7d0	constructor  
	0x7ffd24d4f7e0	cp constructor from  0x7ffd24d4f7d0
	0x7ffd24d4f7e0	destructor	
	0x7ffd24d4f7d0	destructor
#endif	
	return 0;
}
#endif


#if 0
A foo(A &a)
{
	cout << "in foo:" << &a << endl;
	return a;
}
int main()
{
	A a;	//构造
	A t = foo(a);	//将t的地址传入foo函数栈，拷贝构造
	cout << "in main:" << &t << endl;
	
#if 0
	0x7ffc5d1c1a20  constructor  
	in foo:0x7ffc5d1c1a20
	0x7ffc5d1c1a30  cp constructor from  0x7ffc5d1c1a20
	in main:0x7ffc5d1c1a30
	0x7ffc5d1c1a30  destructor  
	0x7ffc5d1c1a20  destructor
#endif
	return 0;
}
#endif


#if 0
A foo(A &a)
{
	cout << "in foo:" << &a << endl;
	return a;
}
int main()
{
	A a;	//构造
	A t;	//构造
	t = foo(a);	//main函数传递临时空间地址到foo函数里执行拷贝构造，函数返回后再执行赋值。
	cout << "in main:" << &t << endl;
	
#if 0
	0x7ffd2dd2a4b0	constructor  a
	0x7ffd2dd2a4c0	constructor  t
	in foo:0x7ffd2dd2a4b0
	0x7ffd2dd2a4d0	cp constructor from  0x7ffd2dd2a4b0	 拷贝构造
	0x7ffd2dd2a4c0	operator =	0x7ffd2dd2a4d0			 赋值
	0x7ffd2dd2a4d0	destructor	
	in main:0x7ffd2dd2a4c0
	0x7ffd2dd2a4c0	destructor	
	0x7ffd2dd2a4b0	destructor 
#endif
	return 0;
}
#endif


#if 0
A foo()
{
	A a;
	a.a = 20;
	cout << "in foo :" << &a << endl;
	return a;
}

int main(void)
{
	A t = foo();	//对a的构造完成了对t的构造，编译器进行了优化。t和a的地址相同。
	cout << "in main:" << &t << endl;
	cout << "t.a == " << t.a << endl;

#if 0
	0x7ffc29e74d30  constructor  
	in foo :0x7ffc29e74d30
	in main:0x7ffc29e74d30
	t.a == 20
	0x7ffc29e74d30  destructor 
#endif
	return 0;
}
#endif


#if 0
A func()
{
	A b;
	cout << "in func &b:" << &b << endl;
	return b;
}

int main()
{
	A t;	//构造
	cout << "in main &t:" << &t << endl;
	t = func();	//b的构造即临时变量的构造，赋值
#if 0
	0x7fff99730790  constructor  
	in main &t:0x7fff99730790
	0x7fff997307a0  constructor  
	in func &b:0x7fff997307a0
	0x7fff99730790  operator =  0x7fff997307a0
	0x7fff997307a0  destructor  
	0x7fff99730790  destructor 
#endif
	return 0;
}
#endif



