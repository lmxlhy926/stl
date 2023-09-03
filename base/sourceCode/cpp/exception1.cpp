
#include <iostream>



void func1(void)
{
	try
	{
		int a = 1;
		throw a;
		std::cout << "in func1" << std::endl;
	}catch(std::string)
	{
		std::cout << "catch in func1" << std::endl;
	}

	return;

}

void func2(void)
{
	try
	{
		func1();
		std::cout << "in func2 try" << std::endl;
	}catch(int)
	{
		std::cout << "catch in func2" << std::endl;
	}

	return;
}


void func3(void)
{
	try
	{
		func2();
		std::cout << "in func3 try" << std::endl;
	}catch(int)
	{
		std::cout << "catch in func3" << std::endl;
	}

	return;
}





int main(void)
{
	try
	{
		func1();
		std::cout << "in main try" << std::endl;

	}catch(int)
	{
		std::cout << "catch in main" << std::endl;
	}



	return 0;
}




