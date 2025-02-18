
#include <iostream>
using namespace std;

/*
虚析构函数：
	c++中基类采用virtual析构函数是为了防止内存泄漏。
	如果派生类中申请了内存空间，并在其析构函数中对这些内存空间进行释放。
	假设基类中采用的是非虚析构函数，当删除基类指针指向的派生类对象时就不会触发动态绑定，
	因而只会调用基类的析构函数，而不会调用派生类的析构函数。
	那么在这种情况下，派生类中申请的空间就得不到释放从而产生内存泄漏。
	
*/

class animal
{
public:
	animal()
	{
		cout << "animal:animal() constructor" << endl;
	}
	virtual ~animal()		//虚析构函数
	{
		cout << "animal:~animal() deconstructor" << endl;	
	}
	virtual void voice() = 0;
};


class dog:public animal
{
public:
	dog()
	{
		cout << "dog:dog() constructor" << endl;
	}
	~dog()
	{
		cout << "dog:~dog() deconstructor" << endl;
	}
	virtual void voice()
	{
		cout << "dog:voice" << endl;
	}
};


class cat:public animal
{
public:
	cat()
	{
		cout << "cat:cat() constructor" << endl;
	}
	~cat()
	{
		cout << "cat:~cat() deconstructor" << endl;
	}
	virtual void voice()
	{
		cout << "cat:voice" << endl;
	}
};



int main(void)
{
	animal *pa = new dog;
	cout << "----" << endl;
	pa->voice();
	cout << "----" << endl;
	delete pa;//删除一个基类指针指向的派生类对象，由于析构函数使用virtual修饰，触发了动态绑定，同时调用了派生类析构函数。
	cout << "----------------\n" << endl;

	pa = new cat;
	cout << "----" << endl;
	pa->voice();
	cout << "----" << endl;
	delete pa;

/*
	animal:animal() constructor
	dog:dog() constructor
	----
	dog:voice
	----
	dog:~dog() deconstructor			//同时调用了派生类的析构函数和基类的析构函数
	animal:~animal() deconstructor
	----------------
	
	animal:animal() constructor
	cat:cat() constructor
	----
	cat:voice
	----
	cat:~cat() deconstructor
	animal:~animal() deconstructor
*/
	return 0;
}






















































