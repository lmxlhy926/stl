#include<iostream>
#include<stdio.h>
using namespace std;

/*
*整型，浮点型，
*字符，字符串
*布尔值
*/

//宏定义的本质就是一个替换过程，由预编译器来处理。
#define LENGTH 10
#define WIDTH  20
#define NEWCHAR 'c'
#define NEWLINE '\n'

int main(void)
{

/*
整型：
	8进制： 0
	16进制：0x,0X
	10进制：不带前缀
	可以带后缀：u,l;U,L；大小写任意，顺序任意。
*/
	cout << 85    	<< endl;
	cout << 012  	<< endl;
	cout << 012ul   << endl;
	cout << 0x23 	<< endl;
	cout << 0X23ul  << endl;
	cout << 30u   	<< endl;
	cout << 30l   	<< endl;
	cout << 30ul    << endl;
	cout << 30U   	<< endl;
	cout << 30L   	<< endl;
	cout << 30UL    << endl;
	

//浮点型：带指数，不带指数（整数部分，小数点，小数部分，指数部分）
	cout << 3.14    <<endl;
	cout << 3.14f   <<endl;
	cout << 3.14d   <<endl;
	cout << 3.14e1  <<endl;
	cout << 3.14e2  <<endl;
	cout << 3.14e-1 <<endl;
	cout << .14e2   <<endl;

/*
字符和字符串的区别：
	字符：''
	字符串：""
*/
	cout << "hello world" 	<<endl;
	cout << "hello" "world"	<<endl;
	cout << NEWCHAR;
	cout << NEWLINE;
	printf("%c",NEWCHAR);
	printf("%c",NEWLINE);

//const修饰的变量
	const int a = 20;	 //const修饰的变量只能在定义时赋值。
	//a = 10;   //报错，a是read-only的，无法赋值。

	

	return 0;
}




