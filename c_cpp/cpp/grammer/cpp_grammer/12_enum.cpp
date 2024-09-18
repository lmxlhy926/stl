
#include<iostream>
using namespace std;

//枚举类型：用户定义的若干枚举常量的集合。即整数变量的符号名称
//枚举和#define宏的区别：宏是由预处理器处理，枚举是由编译器处理(在宏之后被处理)

enum color {
	red,
	bule,
	green
};


int main()
{
	enum color colorexample;
	colorexample = red;		//只能赋值为red,bule,green. 否则编译报错！
	
	switch (colorexample)   //当常量使用
		{
			case green:
				cout << "the value of colorexample == " << colorexample << endl;
		}
	
	
	return 0;
}
































