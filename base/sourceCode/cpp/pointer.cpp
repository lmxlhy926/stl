
#include <iostream>

using namespace std;

/*
定义：<数据类型><类名>::*<指针名>
初始化：<数据类型><类名>::*<指针名> = &<类名>::<非静态数据成员>

解引用：指定类的一个对象，然后通过对象来引用指针所指向的成员。
*/

/*
定义：<数据类型>(<类名>::*<指针名>)(<参数列表>)
赋值初始化：
<数据类型>(<类名>::*<指针名>)(<参数列表>) = &<类名>::<非静态成员函数>

与普通意义上的指针不一样。存放的是偏移量。
指向非静态成员函数时，必须使用类名做限定符，使用时必须使用类的实例做限定符。


*/

class A
{
public:
	string name;
	int a;

	A(string str, int n):name(str),a(n){}
	
};


class student
{
private:
	string name;
	int num;
public:
	student(string a, int b):name(a),num(b){}
	void dis()
	{
		cout << "name: " << name << " num: " << num << endl;
	}

};


int main(void)
{
//	A a("china", 10);
//	A b("japan", 20);

//	string A::*p;	//申明p为指针，指向类A中的一个string类型成员
//	p = &A::name;	//赋值：具体指向的是类A中的name成员
//	cout << b.*p << endl;	//使用时确定指向的到底是哪个对象的成员
//
//	A *pp = new A("russa", 30);	//pp为对象的指针
//	cout << pp->*p << endl;	//访问对象中的元素

	student a("liu", 10);
	student b("lll", 20);
	student *c = new student("sss", 30);

	void (student::*p)() = &student::dis;	//定义P为指向类student中形式为..的函数，指定指向为student中的dis函数.
	(a.*p)();
	(b.*p)();
	(c->*p)();

                                                                

	return 0;
}






















































