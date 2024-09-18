
#include <iostream>
#include "classtest.h"

using namespace std;

class model
{
private:
	string str;	
	int str_length;
	
public:
	model()	//空参构造函数
	{
		cout << "empty argument constructer" << endl;
	}
	
/*
*类的构造函数的初始化列表中的初始化顺序：只与声明顺序有关，与赋值顺序无关。
*此例中先声明str，后声明str_length.
*/
	model(string inputstring): str(inputstring), str_length(str.size())	//初始化列表
	{
		cout << "model(string inputstring)" << endl;
	}
	
	model(const model& another)
	{
		cout << "copy constructer" << endl;
	}
	
	~model()	//当对象超出作用域范围后，会自动调用析构函数。
	{
		cout << "deconstruct" << endl;
	}
	
	
};



int main(int argc, char *argv[])
{
	
	testfunc1();
	
	return 0;
}




void testfunc1()
{
	model m1 {"hello"};	
	func1(m1);	//调用拷贝构造函数将m1的值传递给形参
	return;
}

void testfunc2()
{
	model m1 {"hello"};	
	func2(m1);	//传递的是引用，不调用构造函数
	return;
}














