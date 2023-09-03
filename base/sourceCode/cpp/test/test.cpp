
#include <iostream>

using namespace std;

class A
{
public:
	A(int var1):a(var1){}
	
	void dis()
	{
		std::cout << this->a << std::endl;
	}
	
	int a;
};


class B:public A
{
public:
	B(int var1, int var2):A(var1)
	{
		B::a = var2;
	}
	
	void dis()
	{
		A::a = 100;
		A::dis();
		std::cout << this->a << std::endl;
	}
	
	int a;
};



int main(void)
{
	B b(1, 2);
	b.dis();
	
	return 0;
}










