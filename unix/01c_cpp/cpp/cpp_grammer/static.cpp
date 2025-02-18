
#include <iostream>
using namespace std;

/*
static成员变量实现了同族类对象间信息共享。
static成员类外存储，求类的大小，并不包含在内。
static成员是命名空间属于类的全局变量，存储在data区rw段。
static成员使用时必须初始化，且只能类外初始化
可以通过类名访问，也可以通过对象访问。
*/

/*
静态成员函数的意义在于管理静态数据成员，完成对静态数据成员的封装。
静态成员函数只能访问静态数据成员。原因：非静态成员函数，在调用时
this指针被当做参数传进。而静态成员函数属于类，不属于对象，没有this指针。

*/



class school
{
public:
	static string lib;	//类内声明
	int lib1;

public:
	void add(string a)
	{
		lib += a;
	}

};

string school::lib = "china";	//类外定义，不能再加static修饰


class student
{
private:
	int score;
	static int sum;	//static修饰时，只能类外部初始化
	static int cout;

public:
	student(int n):score(n){}
	void total()
	{
		cout++;
		sum += score;
	}
	
	static int average()
	{
		return sum/cout;
	}

};

int student::sum = 0;	//static修饰时，只能类外部初始化
int student::cout = 0;


class A
{
private:
	const static int a = 2;	//const static修饰时，只能类内部初始化

public:
	void dis()
	{
		cout << a << endl;
	}

};



int main(void)
{
//	school a;
//	cout << sizeof(a) << endl;


//	student s[3] = {student(20), student(30), student(40)};
//	for(int i = 0; i < 3; i++)
//	{
//		s[i].total();
//	}
//	cout << student::average() << endl;

	A a;
	a.dis();


	
	return 0;
}








































