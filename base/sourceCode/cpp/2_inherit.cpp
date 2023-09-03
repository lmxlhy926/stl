#include<iostream>

using namespace std;
/*
继承：依据另一个类来定义一个类，使得创建和维护一个应用程序变得更容易
	  达到了重用代码功能和提高执行效率的效果
	  已有的类称为基类，新建的类称为派生类
	  
基类&派生类：	
	  一个类可以派生自多个类，即其可以从多个基类继承数据和函数。
	  使用一个类派生列表来指定基类，类派生列表以一个或多个基类命名
	  格式：class derived-class: access-specifier base-class，access-specifier base-class，....
	  		**如果未使用访问修饰符access-specifier,则默认为private.	

继承的三种方式：
   *public继承：      基类：public,protected,private --> 派生类：public,protected,private
	protected继承：基类：public,protected,private --> 派生类：protected,protected,private
	private继承：     基类：public,protected,private --> 派生类：private,private,private
	**几乎不使用protected或private继承，通常使用public继承。
**无论哪种继承方式：
	1.private成员只能被本类成员和友元访问，不能被派生类访问；
	2.protected成员可以被派生类访问

  		
访问控制和继承：
				pulbic	protected	private
	同一个类		  √        √           √ 
	派生类           √        √           ×
	外部的类          √        ×           ×
	
一个派生类继承了所有的基类方法，但是下列除外：
	基类的构造函数、析构函数、拷贝构造函数
	基类的重载运算符
	基类的友元函数


*/


class Parrent
{
public:
	int pa1;
protected:
	int pa2;
private:
	int pa3;

public:
	void setPa1(int a);
	void setPa2(int a);
	void setPa3(int a);
	int getPa1(void);
	int getPa2(void);
	int getPa3(void);
	
};

void Parrent::setPa1(int a)
{
	pa1 = a;
}
void Parrent::setPa2(int a)
{
	pa2 = a;
}
void Parrent::setPa3(int a)
{
	pa3 = a;
}
int Parrent:: getPa1(void)
{
	return pa1;
}
int Parrent:: getPa2(void)
{
	return pa2;
}
int Parrent:: getPa3(void)
{
	return pa3;
}




class Son: public Parrent
{
public:
	int sa1;
protected:
	int sa2;
private:
	int sa3;

public:
	void setSa1(int a);
	void setSa2(int a);
	void setSa3(int a);
	int getSa1(void);
	int getSa2(void);
	int getSa3(void);
};
void Son::setSa1(int a)
{
	sa1 = a;
}
void Son::setSa2(int a)
{
	sa2 = a;
}
void Son::setSa3(int a)
{
	sa3 = a;
}
int Son::getSa1(void)
{
	return sa1;
}
int Son::getSa2(void)
{
	return sa2;
}
int Son::getSa3(void)
{
	return sa3;
}



int main(void)
{
	Son son;
	son.setPa1(2);
	cout << son.getPa1() << endl;



	
	return 0;
}
















































