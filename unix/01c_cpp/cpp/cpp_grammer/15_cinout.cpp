#include<iostream>
using namespace std;

/*
	标准输出流/标准输入流/标准错误流/标准日志流：
	cerr流来显示错误消息，其它的日志消息则使用clog流来输出。
*/

int main(void)
{

/*
cout: 与流插入运算符<<结合使用
	  c++编译器根据输出变量的数据类型，选择合适的流插入运算符。
	  <<运算符被重载来输出内置类型(整型，浮点型，double型，字符串和指针)的数据项。
*/
	char str[] = "hello c++";
	cout << "the value of str is:" << str << endl;

	
/*
cin:与流提取运算符>>结合使用
	c++编译器根据要输入值的数据类型，选择合适的流提取运算符来提取值，
	并把它存储在给定的变量中。
*/
	char name[50];
	cout <<"please input your name:";
	cin >> name;
	cout << "your name is :" << name << endl;


//cerr:cerr对象是非缓冲的，每个流插入到cerr都会立即输出。	
	char str1[] = "unable to read....";
	cerr << "error message :" << str1 << endl;

	
//clog:clog对象是缓冲的，直到缓冲填满或缓冲区刷新才会输出
	char str2[] = "clog---clog";
	clog << str2 << endl;


	
	return 0;
}







