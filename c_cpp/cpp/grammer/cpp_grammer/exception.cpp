#include <iostream>

class A
{
public:
	A()
	{
		std::cout << "A constructor" << std::endl;
	}
	~A()
	{
		std::cout << "A deconstructor" << std::endl;
	}
	
	void display()
	{
		std::cout << "---" << std::endl;
	}

};

void func1()
{
	try
	{
		A a;
		throw 3;

	}catch(int)
	{
		std::cout << "catch in func1" << std::endl;
	}

	return;
}

void func2()
{
	A b;
	func1();
	b.display();
	return;
}


int main(void)
{
	try
	{
		func2();
		std::cout << "in main" << std::endl;

	}catch(int)
	{
		std::cout << "catch in main" << std::endl;
	}


	return 0;
}




























